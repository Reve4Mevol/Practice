###############################################################################
# @copyright  All Rights Reserved Copyright (C) CETC52
# @brief  	  CMake 工程模板
# @details    添加GLM库路径
# @author     JiangZhao
# @version    1.0.0
# @history    <Version> <Time>      <Description>  <Author>
#			  1.0.0 	2021/04/26  初始版本
#   										    by jz
###############################################################################

IF (WIN32)
    #    STRING(REPLACE "\\" "."  WINDOWS_HOME_PATH  "$ENV{USERPROFILE}")
    INCLUDE_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/glm/include
    )

ELSEIF (UNIX AND NOT APPLE)
    INCLUDE_DIRECTORIES(
            ${TOP_PROJECT_SOURCE_DIR}/thirdparty/glm/include
    )
ELSEIF (APPLE)
    INCLUDE_DIRECTORIES(
            #            D:/libBase_Release/inc
    )
ENDIF ()





