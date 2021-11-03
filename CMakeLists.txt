cmake_minimum_required(VERSION 3.5)

project(aeroqual)

add_subdirectory (libs/countedString)

add_executable(aeroqual aeroqual.c)

target_link_libraries (aeroqual LINK_PUBLIC countedString)