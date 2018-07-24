
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(app)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

include_directories(
    ${CAPI_CORE_GEN_DIR}
    ${CAPI_DBUS_GEN_DIR}
    ${COMMONAPI_INCLUDE_DIRS}
    ${CMAKE_INSTALL_INCLUDEDIR}/rn2_ext
    
)

link_directories(
    ${COMMONAPI_LIBDIR}
)

# 3. Project Build

set(APP_NAME "navi")

set(APP_SRC_FILES src/SVPNaviApp.cpp
                  src/SVPNaviView.cpp
                  src/PositionMgr.cpp
                  src/SVPPcmPaSimple.cpp
    )

add_executable(${APP_NAME} ${APP_SRC_FILES})

target_link_libraries(${APP_NAME} CommonAPI PocoFoundation svp_app svp_appfw svp_uifw-gles GLESv2 pthread m rt dl 
	svp_basic svp_svchelper pulse-simple extnavi)


# 4. Project Install

install(TARGETS ${APP_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
