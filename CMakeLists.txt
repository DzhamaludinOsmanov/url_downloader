cmake_minimum_required(VERSION 3.29)
project(url_downloader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CURL_INCLUDE_DIR "C:/vcpkg/installed/x64-windows/include") 
set(CURL_LIBRARY "C:/vcpkg/installed/x64-windows/lib/libcurl.lib")

# Указываем директории с заголовочными файлами
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    )

add_executable(url_downloader src/main.cpp src/downloader.cpp)

target_include_directories(url_downloader PRIVATE ${CURL_INCLUDE_DIR}) 
target_link_libraries(url_downloader PRIVATE ${CURL_LIBRARY})