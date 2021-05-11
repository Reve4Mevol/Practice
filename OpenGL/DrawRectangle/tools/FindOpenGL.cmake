###############################################################################
# @copyright  All Rights Reserved Copyright (C) ExplosionEngine
# @brief  	  CMake 工程模板
# @details    添加OpenGL库和OpenGL库路径
# @author     xx
# @version    1.0.0
# @history    <Version> <Time>      <Description>  <Author>
#			  1.0.0 	2021/04/26  初始版本
#   										    by xx
###############################################################################

IF (WIN32)
#    STRING(REPLACE "\\" "."  WINDOWS_HOME_PATH  "$ENV{USERPROFILE}")
    INCLUDE_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/include
    )
    LINK_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/lib
    )
    SET(EXTRA_LIBS glfw3 ${EXTRA_LIBS})

ELSEIF (UNIX AND NOT APPLE)

    INCLUDE_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/include
    )
    LINK_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/lib
    )
    SET(EXTRA_LIBS glfw ${EXTRA_LIBS})

ELSEIF (APPLE)
    INCLUDE_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/include
    )
    LINK_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/OpenGL/lib
    )
    SET(EXTRA_LIBS glfw ${EXTRA_LIBS})

ENDIF ()





