# 源文件（按你的实际代码）
set(SRC_SERVER src/server.cpp)
set(SRC_CLIENT src/client.cpp)

# 查找vsomeip库（系统中查找）
find_package(VSOMEIP REQUIRED)
include_directories(${VSOMEIP_INCLUDE_DIRS})

# 生成服务端可执行文件（输出到 bin/methods_server）
add_executable(methods_server ${SRC_SERVER})
target_link_libraries(methods_server ${VSOMEIP_LIBRARIES} pthread)

# 生成客户端可执行文件（输出到 bin/methods_client）
add_executable(methods_client ${SRC_CLIENT})
target_link_libraries(methods_client ${VSOMEIP_LIBRARIES} pthread)