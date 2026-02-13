#include "AsciiRenderer.hpp"

#include <iostream>
#include <string>

namespace {
void printUsage(const char* executable) {
  std::cerr << "Usage: " << executable
            << " <video_path> [--width N] [--color] [--invert] [--chars <ramp>]\n";
}
}  // namespace

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  std::string videoPath = argv[1];
  AsciiRenderer::Options options;

  for (int i = 2; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--color") {
      options.useColor = true;
    } else if (arg == "--invert") {
      options.invertBrightness = true;
    } else if (arg == "--width" && i + 1 < argc) {
      options.targetWidthChars = std::stoi(argv[++i]);
    } else if (arg == "--chars" && i + 1 < argc) {
      options.asciiRamp = argv[++i];
    } else {
      std::cerr << "Unknown/invalid option: " << arg << "\n";
      printUsage(argv[0]);
      return 1;
    }
  }

  AsciiRenderer renderer(options);
  if (!renderer.playVideo(videoPath, std::cout)) {
    std::cerr << "Failed to open video: " << videoPath << "\n";
    return 2;
  }

  return 0;
}
