cmake_minimum_required(VERSION 3.10)
project(ServiceCPP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir headers
include_directories(/usr/local/include)

# Agregar find_package para nlohmann_json
find_package(nlohmann_json 3.11.3 REQUIRED)

add_executable(ServiceCPP_Executable
    src/main.cpp
    src/db.cpp
    src/controller/LogroController.cpp
)

target_link_libraries(ServiceCPP_Executable 
    pistache 
    pqxx 
    pq 
    pthread 
    nlohmann_json::nlohmann_json
)
