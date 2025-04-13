cmake_minimum_required(VERSION 3.15)
project(KMS VERSION 0.1)
set(CXX_STANDARD_REQUIRED 17)

find_package(Boost REQUIRED)

add_subdirectory(src)

add_executable(${PROJECT_NAME} src/main.cc)
target_include_directories(${PROJECT_NAME} PRIVATE include ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} kms)