cmake_minimum_required(VERSION 3.17)
project(DURKA)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# add_executable(LABA_7 big_integer.h big_integer.cpp big_integer_tests.cpp)
# add_executable(LABA_7 big_integer.h big_integer.cpp big_integer_tests.cpp equation_solver.h equation_solver.cpp equation_solver_tests.cpp)
# add_executable(LABA_8 bidirectional_list.h bidirectional_list_tests.cpp)
# add_executable(LABA_8 shared_ptr_tests.cpp shared_ptr.h weak_ptr.h weak_ptr_tests.cpp)
add_executable(DURKA bidirectional_list.h person.h employee.h patient.h doctor.h clinic.cpp staff_manager.cpp clinic_tests.cpp clinic.h staff_manager.h appointment_manager.h appointment_manager.cpp doctor.cpp clinic_listener.h patient.cpp testing_tool.h)
target_link_libraries(DURKA gtest gtest_main)
