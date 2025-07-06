#include <pistache/endpoint.h>
#include <iostream>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>
#include <string>

#include "db.h"
#include "controller/LogroController.h"
#include "service/TmdbApiClient.h"
#include "controller/MovieApiController.h"

using namespace Pistache;

int main() {
    // Verifica la conexión con PostgreSQL
    testConexionPostgreSQL();

    // Configura el servidor HTTP
    Address addr(Ipv4::any(), Port(5000));
    Http::Endpoint server(addr);

    Rest::Router router;

    // Controlador de logros
    LogroController controller;
    controller.setupRoutes(router);

    // Controlador de películas con API Key TMDb
    std::string tmdb_api_key = "33d6f9d20e39e104025fe947df34c197";
    std::shared_ptr<TmdbApiClient> tmdbApiClient = std::make_shared<TmdbApiClient>(tmdb_api_key);
    MovieApiController movieApiController(tmdbApiClient);
    movieApiController.setupRoutes(router);

    // Inicializa servidor y configura el handler correctamente
    auto opts = Http::Endpoint::options().threads(1);
    server.init(opts);

    // ⚠️ Solución al bug: usar variable intermedia
    auto routerHandler = router.handler();         // 👈 ESTA LÍNEA PREVIENE EL BUG
    server.setHandler(routerHandler);              // ✅ ASÍ se reconoce correctamente POST, GET, etc.

    std::cout << "Servidor Pistache corriendo en http://localhost:5000\n";
    server.serve();

    return 0;
}