
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

project(sv.g5r2.ssplink)

include(GNUInstallDirs)

# 1. ENV setup

# 1.1 Include/link path
include_directories("${CMAKE_INSTALL_INCLUDEDIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../hmi/SSPLinkHMI/src")
include_directories("${CMAKE_INSTALL_INCLUDEDIR}/rn2_ext")

link_directories("${CMAKE_INSTALL_LIBDIR}")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

# 1.2 C/CXX/LD Flags
add_definitions("-std=c++11 -fPIC -Wno-deprecated")

# 1.3 Qt5 ENV
set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH	"$ENV{SDKTARGETSYSROOT}/usr/lib/cmake")
set(OE_QMAKE_PATH_EXTERNAL_HOST_BINS "$ENV{OE_QMAKE_PATH_HOST_BINS}")

find_package(Qt5Qml)
include_directories(${Qt5Qml_INCLUDE_DIRS})
add_definitions(${Qt5Qml_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Qml_EXECUTABLE_COMPILE_FLAGS}")


find_package(Qt5Quick)
include_directories(${Qt5Quick_INCLUDE_DIRS})
add_definitions(${Qt5Quick_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Quick_EXECUTABLE_COMPILE_FLAGS}")


# 2. Build
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(BIN_NAME "ssplink")
set(SRC_FILES src/SVPNaviApp.cpp src/SVPNaviView.cpp ${RES_RCC})
add_executable(${BIN_NAME} ${SRC_FILES})
       
target_link_libraries(${BIN_NAME} SSPLinkHMI)      
target_link_libraries(${BIN_NAME} svp_app svp_appfw svp_uifw-qt Qt5Core Qt5Qml Qt5Quick  svp_basic pthread dl rt m
    extssplink)

# 3. Install (Install to current dir for pkg.sh)
install(TARGETS ${BIN_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
