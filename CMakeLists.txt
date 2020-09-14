cmake_minimum_required(VERSION 3.12)
project(my_redis C)

set(CMAKE_C_STANDARD 99)
add_definitions(-DDEBUG)

add_library(my-redis my-redis.c my-redis.h hash.c hash.h rbtree.c rbtree.h)

add_executable(main main.c)
target_link_libraries(main my-redis)