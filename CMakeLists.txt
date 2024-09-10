cmake_minimum_required(VERSION 3.0)
project(mprpc)

#配置编译选项
set(CMAKE_CXX_FLAGS,${CMAKE_CXX_FLAGS} -g)
#设置可执行可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_BUILD_TYPE,"Debug")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#把指定路径下的所有源文件名字放入变量名
#aux_source_directory( )
link_directories(&{PROJECT_SOURCE_DIR}/lib)
#配置头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(${PROJECT_SOURCE_DIR}/src/include)
add_subdirectory(src)
add_subdirectory(example)