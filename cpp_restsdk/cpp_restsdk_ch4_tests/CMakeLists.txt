cmake_minimum_required(VERSION 3.16)
project(cpp_restsdk_ch4_tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpprestsdk REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

add_executable(ch4_task_basic src/ch4_task_basic.cpp)
add_executable(ch4_then_chain src/ch4_then_chain.cpp)
add_executable(ch4_tce src/ch4_tce.cpp)
add_executable(ch4_cancel src/ch4_cancel.cpp)

foreach(t IN LISTS CMAKE_PROJECT_NAME)
endforeach()

foreach(t ch4_task_basic ch4_then_chain ch4_tce ch4_cancel)
  target_link_libraries(${t} PRIVATE cpprestsdk::cpprest OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
endforeach()
