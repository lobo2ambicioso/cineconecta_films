cmake_minimum_required(VERSION 3.10)
project(CineConectaBackend CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluye headers necesarios
include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/src/include)        # Para httplib
include_directories(${CMAKE_SOURCE_DIR}/src)                # General del proyecto
include_directories(${CMAKE_SOURCE_DIR}/src/models)         # Modelos como MovieResult, Genre, etc.
include_directories(${CMAKE_SOURCE_DIR}/external/pistache/include)
include_directories(${CMAKE_SOURCE_DIR}/external/json/include)

# Buscar la librería de nlohmann JSON
find_package(nlohmann_json 3.11.3 REQUIRED)

# Define soporte para HTTPS en httplib
add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)

add_executable(cineconecta
    src/main.cpp
    src/service/TmdbApiClient.cpp
    src/service/MovieService.cpp         # ✅ nuevo archivo fuente
    src/controller/MovieApiController.cpp
)

# Enlaza las librerías necesarias
target_link_libraries(cineconecta
    pistache
    pqxx
    pq
    pthread
    nlohmann_json::nlohmann_json
    ssl
    crypto
)