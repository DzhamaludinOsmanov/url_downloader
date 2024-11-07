cmake_minimum_required(VERSION 3.29.8)
project(url_downloader)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(include)

add_executable(url_downloader src/main.cpp src/downloader.cpp)
target_link_libraries(url_downloader ZLIB::ZLIB CURL::libcurl)