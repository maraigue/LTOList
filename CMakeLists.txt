cmake_minimum_required(VERSION 3.15)

project(LTOList)

file(GLOB examples LTOList_example_*.cpp)

foreach(example IN LISTS examples)
  get_filename_component(example_name_we ${example} NAME_WE)   # script
  add_executable(${example_name_we} ${example})
endforeach()
