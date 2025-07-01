cmake_minimum_required(VERSION 3.10)
project(ServiceCPP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir headers
include_directories(/usr/local/include)

add_executable(ServiceCPP_Executable
    src/main.cpp
    src/db.cpp
    src/controller/LogroController.cpp
)

target_link_libraries(ServiceCPP_Executable
    PRIVATE
        pistache
        pqxx
)
