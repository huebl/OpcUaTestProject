# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# prebuild
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
configure_file(
    "${PROJECT_SOURCE_DIR}/TestProject/Build/BuildConfig.h.in"
    "${PROJECT_BINARY_DIR}/BuildConfig.h"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/TestProject/linux/etc/init/TestProject.conf.in"
    "${PROJECT_BINARY_DIR}/TestProject.conf"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/TestProject/linux/etc/init.d/TestProject.in"
    "${PROJECT_BINARY_DIR}/TestProject"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/TestProject/linux/usr/lib/systemd/system/TestProject.service.in"
    "${PROJECT_BINARY_DIR}/TestProject.service"
)


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# build 
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
include_directories(
    ${OPCUASTACK_INSTALL_PREFIX}/usr/include/OpcUaStack${OPCUASTACK_VERSION_MAJOR}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${Boost_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

file(
    GLOB 
    TestProject_SRC 
    ${PROJECT_SOURCE_DIR}/TestProject/*/*.cpp
)

add_library(
    TestProject SHARED
    ${TestProject_SRC}
)

target_link_libraries(
    TestProject
    ${pthread}
    ${OPCUASTACK_LIBRARY_CORE}
    ${OPCUASTACK_LIBRARY_CLIENT}
    ${OPCUASTACK_LIBRARY_SERVER}
    ${OPCUASTACK_LIBRARY_PUBSUB}
    ${Boost_LIBRARIES}
)

if (WIN32)
    target_link_libraries(TestProject wsock32 ws2_32)

    if (MINGW)
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
    endif()
endif()

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# install
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

file (GLOB CONFIG_FILES ${PROJECT_SOURCE_DIR}/TestProject/Config/*.xml)
install(TARGETS TestProject DESTINATION ${INSTALL_PREFIX}/lib)

if (WIN32)
    install(FILES ${CONFIG_FILES} DESTINATION ${INSTALL_PREFIX}/../etc/OpcUaStack/TestProject) 
    install(FILES ${PROJECT_SOURCE_DIR}/TestProject/linux/log/README DESTINATION ${INSTALL_PREFIX}/../var/log/OpcUaStack/TestProject)
    install(DIRECTORY DESTINATION ${INSTALL_PREFIX}/../var/log/OpcUaStack/TestProject)
else()    
    install(FILES ${CONFIG_FILES} DESTINATION ${INSTALL_PREFIX}/../etc/OpcUaStack/TestProject) 
    install(FILES ${PROJECT_SOURCE_DIR}/TestProject/linux/log/README DESTINATION /var/log/OpcUaStack/TestProject)
    install(DIRECTORY DESTINATION /var/log/OpcUaStack/TestProject)

    install(PROGRAMS ${PROJECT_BINARY_DIR}/TestProject DESTINATION /etc/init.d)
    install(FILES ${PROJECT_BINARY_DIR}/TestProject.conf DESTINATION /etc/init)
    install(FILES ${PROJECT_BINARY_DIR}/TestProject.service DESTINATION /usr/lib/systemd/system)
endif()

if (CPACK_BINARY_DEB)
    set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/linux/script/postinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/linux/script/preinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/linux/script/prerm"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/linux/script/postrm")
endif()

if (CPACK_BINARY_RPM)
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TestProject/linux/script/postinst)
    set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TestProject/linux/script/prerm)
endif()
