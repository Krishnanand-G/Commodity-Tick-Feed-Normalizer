cmake_minimum_required(VERSION 3.14)
project(CommodityTickFeedNormalizer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Main library for parser
add_library(ParserLib src/Parser.cpp)
target_include_directories(ParserLib PUBLIC src)

# Main executable
add_executable(TickNormalizer src/main.cpp)
target_link_libraries(TickNormalizer PRIVATE ParserLib)

# Enable testing
enable_testing()
add_subdirectory(test)
