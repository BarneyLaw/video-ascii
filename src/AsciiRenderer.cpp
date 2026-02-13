#include "AsciiRenderer.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <thread>

namespace {
constexpr double kTerminalCellAspect = 0.5;  // terminal characters are usually taller than wide
}

AsciiRenderer::AsciiRenderer(Options options) : options_(std::move(options)) {}

cv::Size AsciiRenderer::computeOutputSize(const cv::Mat& bgrFrame) const {
  const int sourceW = bgrFrame.cols;
  const int sourceH = bgrFrame.rows;

  const int outW = std::max(1, options_.targetWidthChars);
  const double scaledHeight =
      (static_cast<double>(sourceH) / static_cast<double>(sourceW)) * static_cast<double>(outW) * kTerminalCellAspect;
  const int outH = std::max(1, static_cast<int>(std::round(scaledHeight)));

  return {outW, outH};
}

char AsciiRenderer::mapIntensityToChar(unsigned char intensity) const {
  const std::string& ramp = options_.asciiRamp;
  const size_t rampSize = ramp.size();

  if (rampSize == 0) {
    return '#';
  }

  double normalized = static_cast<double>(intensity) / 255.0;
  if (options_.invertBrightness) {
    normalized = 1.0 - normalized;
  }

  const size_t idx = std::min(rampSize - 1, static_cast<size_t>(std::round(normalized * static_cast<double>(rampSize - 1))));
  return ramp[idx];
}

std::string AsciiRenderer::ansiColor(unsigned char b, unsigned char g, unsigned char r, char c) const {
  std::ostringstream os;
  os << "\033[38;2;" << static_cast<int>(r) << ';' << static_cast<int>(g) << ';' << static_cast<int>(b) << 'm' << c
     << "\033[0m";
  return os.str();
}

std::string AsciiRenderer::frameToAscii(const cv::Mat& bgrFrame) const {
  cv::Mat resized;
  cv::resize(bgrFrame, resized, computeOutputSize(bgrFrame), 0, 0, cv::INTER_AREA);

  cv::Mat gray;
  cv::cvtColor(resized, gray, cv::COLOR_BGR2GRAY);

  std::ostringstream ascii;
  for (int y = 0; y < resized.rows; ++y) {
    for (int x = 0; x < resized.cols; ++x) {
      const unsigned char intensity = gray.at<unsigned char>(y, x);
      const char c = mapIntensityToChar(intensity);

      if (options_.useColor) {
        const cv::Vec3b bgr = resized.at<cv::Vec3b>(y, x);
        ascii << ansiColor(bgr[0], bgr[1], bgr[2], c);
      } else {
        ascii << c;
      }
    }
    ascii << '\n';
  }

  return ascii.str();
}

void AsciiRenderer::sleepUntil(std::chrono::steady_clock::time_point target) {
  std::this_thread::sleep_until(target);
}

bool AsciiRenderer::playVideo(const std::string& videoPath, std::ostream& out) const {
  cv::VideoCapture capture(videoPath);
  if (!capture.isOpened()) {
    return false;
  }

  double fps = capture.get(cv::CAP_PROP_FPS);
  if (fps <= 0.0 || !std::isfinite(fps)) {
    fps = 30.0;
  }
  const auto framePeriod = std::chrono::duration<double>(1.0 / fps);

  cv::Mat frame;
  auto nextFrameTime = std::chrono::steady_clock::now();

  while (capture.read(frame)) {
    out << "\033[H\033[J";  // clear screen + move cursor home
    out << frameToAscii(frame);
    out.flush();

    nextFrameTime += std::chrono::duration_cast<std::chrono::steady_clock::duration>(framePeriod);
    sleepUntil(nextFrameTime);
  }

  return true;
}
