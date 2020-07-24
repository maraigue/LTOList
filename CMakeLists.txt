cmake_minimum_required(VERSION 3.15)

project(LTOList)

set(INCLUDE_DIRS ./catch2)
set(LIST_OF_TESTS "")

file(GLOB examples LTOList_example_*.cpp)
file(GLOB tests LTOList_test_*.cpp)
include_directories(${INCLUDE_DIRS})

foreach(exec IN LISTS examples)
  get_filename_component(example_name_we ${exec} NAME_WE)   # script
  add_executable(${example_name_we} ${exec})
endforeach()

foreach(exec IN LISTS tests)
  get_filename_component(example_name_we ${exec} NAME_WE)   # script
  add_executable(${example_name_we} ${exec})
  add_custom_target(
    test_${example_name_we}
    COMMAND ./${example_name_we}
    DEPENDS ${example_name_we}
  )
  set(LIST_OF_TESTS ${LIST_OF_TESTS} test_${example_name_we})
endforeach()

add_custom_target(
  test
  DEPENDS ${LIST_OF_TESTS}
)
