#include <pistache/endpoint.h>
#include <iostream>
#include <pistache/http.h>
#include <pistache/router.h>

#include "db.h"
#include "controller/LogroController.h"

using namespace Pistache;

int main() {
    // Verifica la conexión con PostgreSQL
    testConexionPostgreSQL();

    // Configura el servidor HTTP
    Address addr(Ipv4::any(), Port(5000));
    Http::Endpoint server(addr);

    Rest::Router router;
    LogroController controller;
    controller.setupRoutes(router);

    server.init();
    server.setHandler(router.handler());

    std::cout << "Servidor Pistache corriendo en http://localhost:5000\n";
    server.serve();
    
    return 0;
}