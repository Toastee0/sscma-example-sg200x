set(SYSROOT $ENV{reCamera_sysroot})

component_register(
    COMPONENT_NAME main
    SRCS ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    SRCS ${CMAKE_CURRENT_LIST_DIR}/announceURL.cpp

    INCLUDE_DIRS ${SYSROOT}/usr/include/
    INCLUDE_DIRS ${SYSROOT}/usr/include/UsageEnvironment/
    INCLUDE_DIRS ${SYSROOT}/usr/include/groupsock/
    INCLUDE_DIRS ${SYSROOT}/usr/include/liveMedia/
    INCLUDE_DIRS ${SYSROOT}/usr/include/BasicUsageEnvironment/
    LIBRARY_DIRS ${SYSROOT}/usr/lib/

    PRIVATE_REQUIREDS liveMedia groupsock BasicUsageEnvironment UsageEnvironment ssl crypto
)
