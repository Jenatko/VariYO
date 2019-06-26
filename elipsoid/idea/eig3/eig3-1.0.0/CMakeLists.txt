
cmake_minimum_required(VERSION 3.0)

project (eig3)

add_executable(eig3 main.c eig3.c)

target_link_libraries(eig3 m)
