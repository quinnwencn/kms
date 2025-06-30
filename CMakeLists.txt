cmake_minimum_required(VERSION 3.15)
project(KMS VERSION 0.1)
set(CXX_STANDARD_REQUIRED 17)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system thread beast)

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
    ${OpenSSL_LIBRARIES})