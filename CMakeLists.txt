
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 0. CMake Defs

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

# 1. Project Name

project (thirdparty)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)

include_directories("${CMAKE_INSTALL_INCLUDEDIR}")

include_directories("${CMAKE_INSTALL_INCLUDEDIR_SVA}")

include_directories("${CMAKE_INSTALL_INCLUDEDIR}/kud/client")

include_directories("${CMAKE_INSTALL_INCLUDEDIR}/kud/client/nb")

link_directories("${CMAKE_INSTALL_LIBDIR}")

link_directories("${CMAKE_INSTALL_LIBDIR_APPLOGIC}")

link_directories("${CMAKE_INSTALL_KUDLIBDIR}")

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_definitions("-fPIC -Wno-deprecated") #-W -Wall

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GLIBCXX_USE_NANOSLEEP")

# 3. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

set(CMAKE_PREFIX_PATH   "$ENV{SDKTARGETSYSROOT}/usr/lib/cmake")

find_package(CommonAPI 3.1.5 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)

find_package(CommonAPI-DBus 3.1.5 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)

include(FindPkgConfig)

pkg_check_modules(DBus REQUIRED dbus-1>=1.4)

set(CAPI_FIDL_DIR ${CMAKE_INSTALL_DATAROOTDIR}/api)

set(CAPI_CORE_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/capi-gen/core)

set(CAPI_DBUS_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/capi-gen/dbus)

#set(DATA_LIB_NAME "auxdata")

# 4. Sub Projects

#add_subdirectory(data)

#add_subdirectory(api)

#add_subdirectory(iflytek)

#add_subdirectory(libextnavi)

#add_subdirectory(asr-plugin)

#add_subdirectory(svudpclient)

#add_subdirectory(svudpserver)

#add_subdirectory(rn2-test)

add_subdirectory(NaviDemo)

#add_subdirectory(libextssplink)

add_subdirectory(SSPLinkDemo/ssplink)

add_subdirectory(vdec)

#add_subdirectory(libasrengine)

# 5. Project Install


