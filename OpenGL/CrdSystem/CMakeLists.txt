cmake_minimum_required(VERSION 3.15)
project(CrdSystem)

SET(TOP_PROJECT_SOURCE_DIR  ${PROJECT_SOURCE_DIR})
SET(TOP_PROJECT_BINARY_DIR  ${PROJECT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES ${SOURCE_FILES}
        main.cpp glad.c stb_image.h Shader.cpp Shader.h)

INCLUDE(tools/FindOpenGL.cmake)
INCLUDE(tools/FindGLM.cmake)
file(COPY ${TOP_PROJECT_SOURCE_DIR}/container.jpg DESTINATION ${TOP_PROJECT_BINARY_DIR})
file(COPY ${TOP_PROJECT_SOURCE_DIR}/shidizai.jpg DESTINATION ${TOP_PROJECT_BINARY_DIR})
file(COPY ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/lib/glfw3.lib DESTINATION ${TOP_PROJECT_BINARY_DIR})

add_executable(CrdSystem ${SOURCE_FILES})
target_link_libraries(CrdSystem ${EXTRA_LIBS})