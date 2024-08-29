set(SYSROOT ${SG200X_SDK_PATH}/buildroot-2021.05/output/cvitek_CV181X_musl_riscv64/host/riscv64-buildroot-linux-musl/sysroot/)
link_directories("${CMAKE_CURRENT_LIST_DIR}")

component_register(
    COMPONENT_NAME main
    SRCS ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    PRIVATE_REQUIREDS video # cvi_rtsp

    INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/"
    SRCS "${CMAKE_CURRENT_LIST_DIR}/rtsp_demo.cpp"

    INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/rtsp/"
    INCLUDE_DIRS ${SYSROOT}/usr/include/
    INCLUDE_DIRS ${SYSROOT}/usr/include/UsageEnvironment/
    INCLUDE_DIRS ${SYSROOT}/usr/include/groupsock/
    INCLUDE_DIRS ${SYSROOT}/usr/include/liveMedia/
    INCLUDE_DIRS ${SYSROOT}/usr/include/BasicUsageEnvironment/
    LIBRARY_DIRS ${SYSROOT}/usr/lib/

    SRCS "${CMAKE_CURRENT_LIST_DIR}/rtsp/H264FramedLiveSource.cpp"
    SRCS "${CMAKE_CURRENT_LIST_DIR}/rtsp/H264LiveVideoServerMediaSubsession.cpp"
    SRCS "${CMAKE_CURRENT_LIST_DIR}/rtsp/announceURL.cpp"
    SRCS "${CMAKE_CURRENT_LIST_DIR}/rtsp/ma_transport_rtsp.cpp"

    PRIVATE_REQUIREDS liveMedia groupsock BasicUsageEnvironment UsageEnvironment ssl crypto
    PRIVATE_REQUIREDS sscma cares pthread
)
