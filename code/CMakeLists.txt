cmake_minimum_required(VERSION 3.10)
project(weatherApp C)

set(CMAKE_C_STANDARD 99)

add_executable(weatherApp main.c libs/mongoose.c)
target_compile_options(weatherApp PRIVATE -Os -ffunction-sections -fdata-sections)
target_link_libraries(weatherApp PRIVATE -static -Wl,--gc-sections -s)