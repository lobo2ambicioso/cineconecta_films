cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y tipo de lenguaje
project(CineConectaBackend CXX)

# Estándar mínimo de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------
# INCLUSIÓN DE DIRECTORIOS DE CABECERAS (HEADERS)
# --------------------------------------------
include_directories(/usr/local/include)  # Cabeceras externas

# Directorios internos del proyecto
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/models)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Dependencias externas (Pistache, nlohmann-json)
include_directories(${CMAKE_SOURCE_DIR}/external/pistache/include)
include_directories(${CMAKE_SOURCE_DIR}/external/json/include)

# --------------------------------------------
# DEPENDENCIAS EXTERNAS
# --------------------------------------------

# Busca e incluye la librería nlohmann_json instalada en el sistema
find_package(nlohmann_json 3.11.3 REQUIRED)

# Define que httplib se compile con soporte para SSL
add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)

# --------------------------------------------
# EJECUTABLE PRINCIPAL DEL SERVIDOR
# --------------------------------------------

# Define el ejecutable principal del backend
add_executable(cineconecta
    src/main.cpp
    src/service/TmdbApiClient.cpp
    src/service/MovieService.cpp
    src/controller/MovieApiController.cpp
)

# Enlaza las librerías necesarias al ejecutable
target_link_libraries(cineconecta
    pistache                # Framework HTTP
    pqxx pq                 # PostgreSQL (si planeas conectarte a BD)
    pthread                 # Concurrencia
    nlohmann_json::nlohmann_json  # JSON moderno
    ssl crypto              # Seguridad (para HTTPS)
)

# --------------------------------------------
# CONFIGURACIÓN PARA TESTS UNITARIOS (Google Test)
# --------------------------------------------

enable_testing()  # Activa soporte para pruebas

# Ejecutable para correr los tests unitarios
add_executable(run_tests
    tests/MovieServiceTests.cpp
    src/service/MovieService.cpp
    src/service/TmdbApiClient.cpp
)

# Rutas necesarias para los tests
target_include_directories(run_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/models
    ${CMAKE_SOURCE_DIR}/external/json/include
)

# Enlaza las librerías necesarias para testeo
target_link_libraries(run_tests
    gtest gtest_main         # Google Test
    pthread                  # Concurrencia
    ssl crypto               # HTTPS
    nlohmann_json::nlohmann_json
)

# Define el comando para correr los tests
add_test(NAME RunAllTests COMMAND run_tests)