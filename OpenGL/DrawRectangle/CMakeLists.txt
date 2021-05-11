# 设置最小cmake版本要求
cmake_minimum_required(VERSION 3.1)
# 设置项目名
SET(TOP_PROJECT_NAME DrawRectangle)

# 设置C++语法
set(CMAKE_CXX_STANDARD 14)

PROJECT(${TOP_PROJECT_NAME})

# 设置TOP_PROJECT_SOURCE_DIR为项目最上层路径,如果有多个CMakeList时每一层的PROJECT_SOURCE_DIR可能不一样，所有相当于定义个根目录宏
SET(TOP_PROJECT_SOURCE_DIR  ${PROJECT_SOURCE_DIR})
SET(TOP_PROJECT_BINARY_DIR  ${PROJECT_BINARY_DIR})

# 指定程序和库的输出路径
SET(TOP_PROJECT_OUTPUT_PATH ${TOP_PROJECT_BINARY_DIR})

# 用到哪个库就包含对应找库的cmake文件
INCLUDE(tools/FindOpenGL.cmake)

# Enable/Disable Rpath support
IF (1)
    SET(CMAKE_SKIP_BUILD_RPATH TRUE)                # 编译是不使用RPATH
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)       # 编译时不使用安装RPATH
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)    # 安装的执行文件不加上RPAH
ELSE ()
    SET(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)       # 使能ORIGIN
    SET(CMAKE_BUILD_RPATH "\${ORIGIN}:\${ORIGIN}/lib:\${ORIGIN}/../lib") # 编译是时加上RPATH，可以使用CMAKE_BUILD_WITH_INSTALL_RPATH代替
    SET(CMAKE_SKIP_BUILD_RPATH FALSE)             # 编译时加上RPATH
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)      # 编译时使用安装时RPATH
    SET(CMAKE_INSTALL_RPATH "\${ORIGIN}:\${ORIGIN}/lib:\${ORIGIN}/../lib:${PROJECT_SOURCE_DIR}/lib")         # 安装时RPATH
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)   # 安装的执行文件加上RPATH
ENDIF ()

# 每次reload Cmake时拷贝库文件到output路径
FILE(COPY "${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/lib/glfw3.dll" DESTINATION "${TOP_PROJECT_OUTPUT_PATH}")

# 编译的文件添加到 source_files，也可以在子目录添加CmakeList，最外层CmakeList通过ADD_SUBDIRECTORY(src/xxx)选择启用哪些目录
SET(SOURCE_FILES ${SOURCE_FILES}
        src/main.cpp src/glad.c
        )

# 编可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

# 链接程序与外部库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${EXTRA_LIBS})