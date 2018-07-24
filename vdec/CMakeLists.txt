
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(test.desay.vdec)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

set(SVP_PATH "/work/project/RN2-SGMW-CN210S-Application/dist/release/svp")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(
    ${SVP_PATH}/include
)
include_directories("include")
include_directories("svpinclude")

link_directories(
    ${SVP_PATH}/lib
    ${SVP_PATH}/lib/cidana
)

add_definitions("-fPIC -Wno-deprecated") #-W -Wall
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GLIBCXX_USE_NANOSLEEP")

# 3. Project Build

set(TEST_NAME "svp.desay.vdec")

set(TEST_SRC_FILES
        src/General_StreamInput.cpp
		src/ScreenSaverPlayer.cpp 
        src/main.cpp )

add_executable(${TEST_NAME} ${TEST_SRC_FILES})

target_link_libraries(${TEST_NAME} svp_vdec-cidana PocoFoundation ilmControl ilmCommon pthread dl rt m)

# 4. Project Install

install(TARGETS ${TEST_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

