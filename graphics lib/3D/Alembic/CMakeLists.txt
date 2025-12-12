cmake_minimum_required(VERSION 3.10)
project(AlembicOObjectTest)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Alembic库
find_package(Alembic REQUIRED)

# 创建可执行文件
add_executable(test_oobject test_oobject.cpp)

# 链接Alembic库
target_link_libraries(test_oobject 
    Alembic::Abc
    Alembic::AbcCoreHDF5
)

# 设置输出目录
set_target_properties(test_oobject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 添加编译选项
target_compile_options(test_oobject PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
) 