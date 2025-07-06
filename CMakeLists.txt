cmake_minimum_required(VERSION 3.10)
project(ServiceCPP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir headers de sistema y de proyecto
include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/src/include) # Asegúrate de que httplib.h esté aquí

# Agregar find_package para nlohmann_json
find_package(nlohmann_json 3.11.3 REQUIRED)

# Definir soporte para OpenSSL en httplib (necesario para HTTPS)
add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)

add_executable(ServiceCPP_Executable
    src/main.cpp
    src/db.cpp
    src/controller/LogroController.cpp
	src/service/TmdbApiClient.cpp         # NUEVO: Servicio para llamadas a TMDb
    src/controller/MovieApiController.cpp # NUEVO: Controlador para manejar peticiones de Java
)

target_link_libraries(ServiceCPP_Executable 
    pistache 
    pqxx 
    pq 
    pthread 
    nlohmann_json::nlohmann_json
	# Librerías OpenSSL para HTTPS con httplib
    # Asegúrate de que estas librerías estén instaladas en tu sistema
    # En sistemas basados en Debian/Ubuntu: sudo apt-get install libssl-dev
    # En sistemas basados en RedHat/Fedora: sudo yum install openssl-devel
    # En Windows, necesitarás compilar OpenSSL o usar una distribución como vcpkg
    ssl
    crypto
)
