#pragma once

#include <opencv2/opencv.hpp>

#include <chrono>
#include <ostream>
#include <string>

class AsciiRenderer {
 public:
  struct Options {
    int targetWidthChars = 120;
    bool useColor = false;
    bool invertBrightness = false;
    std::string asciiRamp = " .,:;irsXA253hMHGS#9B&@";
  };

  explicit AsciiRenderer(Options options = {});

  // Convert a BGR frame to a printable ASCII frame while preserving aspect ratio.
  std::string frameToAscii(const cv::Mat& bgrFrame) const;

  // Stream a whole video to the provided output stream.
  // Returns false if the video cannot be opened.
  bool playVideo(const std::string& videoPath, std::ostream& out) const;

 private:
  Options options_;

  cv::Size computeOutputSize(const cv::Mat& bgrFrame) const;
  char mapIntensityToChar(unsigned char intensity) const;
  std::string ansiColor(unsigned char b, unsigned char g, unsigned char r, char c) const;
  static void sleepUntil(std::chrono::steady_clock::time_point target);
};
