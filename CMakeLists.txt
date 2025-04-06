cmake_minimum_required(VERSION 3.15)
project(KMS VERSION 0.1 LANGUAGE CXX)
set(CXX_STANDARD_REQUIRED 17)

find_package(Boost REQUIRED)

add_subdirectory(src)
