cmake_minimum_required(VERSION 3.16)
project(video_ascii LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_executable(video-ascii
  src/main.cpp
  src/AsciiRenderer.cpp
)

target_include_directories(video-ascii
  PRIVATE
    include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(video-ascii PRIVATE ${OpenCV_LIBS})

if(MSVC)
  target_compile_options(video-ascii PRIVATE /W4)
else()
  target_compile_options(video-ascii PRIVATE -Wall -Wextra -Wpedantic)
endif()
