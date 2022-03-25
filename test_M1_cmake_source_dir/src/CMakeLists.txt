# expected: CMAKE_SOURCE_DIR = ${PROJECT_DIR}
# actual:   CMAKE_SOURCE_DIR = ${PROJECT_DIR}/src
message(STATUS "In src directory, CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}")

add_executable(testbed testbed.cpp)
target_include_directories(testbed PUBLIC ${CMAKE_SOURCE_DIR})
