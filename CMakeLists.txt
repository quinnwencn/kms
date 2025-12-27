cmake_minimum_required(VERSION 3.15)
project(KMS VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED TRUE)

find_package(OpenSSL REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(Boost REQUIRED system thread beast log log_setup)
find_package(fmt REQUIRED)
find_package(CLI11 REQUIRED)

#add_library(logger INTERFACE ${CMAKE_SOURCE_DIR}/src/logger/logger.cc)
#target_include_directories(logger INTERFACE
#        ${CMAKE_SOURCE_DIR}/include/kms
#        ${Boost_INCLUDE_DIRS})
#target_link_libraries(logger INTERFACE fmt::fmt)

add_subdirectory(src)

add_executable(${PROJECT_NAME} src/main.cc)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${Boost_INCLUDE_DIRS}
    ${OpenSSL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    kms
    pthread
    Boost::system
    Boost::thread
    ${OpenSSL_LIBRARIES}
    tomlplusplus::tomlplusplus
    fmt::fmt
    CLI11::CLI11)
