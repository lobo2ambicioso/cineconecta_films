# Especifica la versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y lenguaje principal
project(CineConectaBackend CXX)

# Establece el estándar de C++ que usará el proyecto (C++17 obligatorio)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------
# INCLUSIÓN DE DIRECTORIOS DE CABECERAS (HEADERS)
# --------------------------------------------

# Ruta para cabeceras del sistema
include_directories(/usr/local/include)

# Cabeceras del proyecto (servicios, controladores, etc.)
include_directories(${CMAKE_SOURCE_DIR}/src)                

# Cabeceras para librería httplib (HTTP cliente)
include_directories(${CMAKE_SOURCE_DIR}/src/include)        

# Modelos de datos como MovieResult, Genre, etc.
include_directories(${CMAKE_SOURCE_DIR}/src/models)         

# Librerías externas: Pistache (framework HTTP en C++)
include_directories(${CMAKE_SOURCE_DIR}/external/pistache/include)

# Librerías externas: nlohmann/json (manejo de JSON en C++)
include_directories(${CMAKE_SOURCE_DIR}/external/json/include)

# --------------------------------------------
# DEPENDENCIAS EXTERNAS
# --------------------------------------------

# Requiere la versión 3.11.3 o superior de la librería nlohmann_json
find_package(nlohmann_json 3.11.3 REQUIRED)

# Habilita soporte para conexiones HTTPS en httplib
add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)

# --------------------------------------------
# DEFINICIÓN DEL EJECUTABLE Y ARCHIVOS FUENTE
# --------------------------------------------

add_executable(cineconecta
    src/main.cpp                           # Punto de entrada del servidor HTTP
    src/service/TmdbApiClient.cpp          # Cliente para consumir API de TMDb
    src/service/MovieService.cpp           # Lógica de negocio del sistema
    src/controller/MovieApiController.cpp  # Controlador de rutas REST
)

# --------------------------------------------
# ENLACE DE LIBRERÍAS NECESARIAS PARA EL PROYECTO
# --------------------------------------------

target_link_libraries(cineconecta
    pistache                                # Framework HTTP para endpoints REST
    pqxx                                    # Cliente C++ para PostgreSQL (si se usa en futuro)
    pq                                      # Cliente PostgreSQL en C
    pthread                                 # Hilos para concurrencia
    nlohmann_json::nlohmann_json           # Librería de manejo de JSON
    ssl                                     # Librería OpenSSL para HTTPS
    crypto                                  # Librería de funciones criptográficas
)