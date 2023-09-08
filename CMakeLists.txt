cmake_minimum_required(VERSION 3.15)

project(demo)


set(OpenCV_DIR /home/xing/opencv-4.5.3/build)
# find_package(OpenCV REQUIRED)
find_package(OpenCV 4.5.3 REQUIRED)#寻找用来动态库的路径


# #打印一些信息
# message(STATUS "OpenCV_DIR = ${OpenCV_DIR}")
# message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
# message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

#添加头文件
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(zuoye ${PROJECT_SOURCE_DIR}/zuoye.cpp)

#链接库文件
target_link_libraries(zuoye  ${OpenCV_LIBS})

