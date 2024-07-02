cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "secret")
set(CMAKE_CXX_FLAGS "-g")
project(${PROJECT_NAME})

########################################################################################################################
# import openssl
set(_openssl_path "/home/shecannotsee/Desktop/sheer_third_party/libraries/openssl")
set(openssl_include_dir "${_openssl_path}/include")
set(openssl_lib_dir "${_openssl_path}/lib")
set(openssl_link_libs ssl crypto)
########################################################################################################################

########################################################################################################################
# import nlohmann_json
set(_nlohmann_json_path "/home/shecannotsee/Desktop/sheer_third_party/libraries/nlohmann_json")
set(nlohmann_json_include_dir "${_nlohmann_json_path}/include")
set(nlohmann_json_lib_dir)
set(nlohmann_json_link_libs)
########################################################################################################################

########################################################################################################################
# import FTXUI
set(_FTXUI_path "/home/shecannotsee/Desktop/sheer_third_party/libraries/FTXUI")
set(FTXUI_include_dir "${_FTXUI_path}/include")
set(FTXUI_lib_dir "${_FTXUI_path}/lib")
set(FTXUI_link_libs ftxui-component ftxui-dom ftxui-screen)
########################################################################################################################

# include
include_directories(
        ${openssl_include_dir}
        ${nlohmann_json_include_dir}
        ${FTXUI_include_dir}
)
# lib
link_directories(
        ${openssl_lib_dir}
        ${FTXUI_lib_dir}
)

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*")
add_executable(${PROJECT_NAME} ${SRC})

# link
target_link_libraries(${PROJECT_NAME} "-pthread"
        ${openssl_link_libs}
        ${FTXUI_link_libs}
)
