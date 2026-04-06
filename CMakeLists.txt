# 最低CMake版本（兼容子模块）
cmake_minimum_required(VERSION 3.10)
# 顶层项目名（自定义，如 practice_all）
project(practice_all)

# ===================== 核心配置：统一输出目录 =====================
# 可执行文件输出目录（根目录/bin）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# 库文件输出目录（根目录/lib，如有库模块时生效）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# 多配置构建（如VS）也强制统一输出目录
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} ${CMAKE_SOURCE_DIR}/lib)
endforeach()

# ===================== 统一编译标准 =====================
# 全局C++11标准，子模块自动继承
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# # 全局编译警告（可选，强制规范）
# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     add_compile_options(-Wall -Wextra)
# endif()


add_subdirectory(parctice)
add_subdirectory(test)
