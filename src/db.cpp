#include <iostream>
#include <memory>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <string>
#include <pqxx/pqxx> // Incluye la librería pqxx

// Función para probar la conexión a PostgreSQL
void testConexionPostgreSQL() {
    try {
        // Construye la cadena de conexión.
        // Asegúrate de que el 'user' sea 'cine_conecta_admin@cineconectav2' si el simple 'cine_conecta_admin' no funciona.
        // Azure PostgreSQL a menudo requiere SSL. 'sslmode=require' es común.
        std::string conn_str = "host=cineconectav2.postgres.database.azure.com "
                               "port=5432 "
                               "dbname=cineconecta "
                               "user=cine_conecta_admin " // Prueba con 'cine_conecta_admin@cineconectav2' si falla
                               "password=cubito1234! "    // ¡ADVERTENCIA: NO USAR ASÍ EN PRODUCCIÓN!
                               "sslmode=require";        // Requerir SSL para Azure

        // Intenta establecer la conexión
        pqxx::connection C(conn_str);

        if (C.is_open()) {
            std::cout << "Conexión a la base de datos PostgreSQL de Azure exitosa: " << C.dbname() << std::endl;

            // Inicia una transacción para ejecutar una consulta
            pqxx::work W(C);

            // Ejecuta una consulta de prueba (por ejemplo, para obtener la versión del servidor)
            pqxx::result R = W.exec("SELECT version()");

            std::cout << "=== Resultados desde PostgreSQL ===\n";
            for (auto row : R) {
                std::cout << "- Versión de PostgreSQL: " << row[0].as<std::string>() << std::endl;
            }

            // Otra consulta de prueba: Si tienes una tabla 'logros' en PostgreSQL (con los mismos campos)
            // pqxx::result logros_res = W.exec("SELECT nombre FROM logros");
            // std::cout << "=== Logros desde PostgreSQL ===\n";
            // for (auto row : logros_res) {
            //     std::cout << "- Logro: " << row[0].as<std::string>() << std::endl;
            // }

            W.commit(); // Confirma la transacción
        } else {
            std::cerr << "X Error: No se pudo abrir la base de datos." << std::endl;
        }
    } catch (const pqxx::sql_error &e) {
        std::cerr << "X Error de PostgreSQL (SQL): " << e.what() << std::endl;
        std::cerr << " Query que causó el error: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "X Error de PostgreSQL: " << e.what() << std::endl;
    }
}