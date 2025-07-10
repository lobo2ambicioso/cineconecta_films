#include <pistache/endpoint.h>
#include <iostream>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>
#include <string>

#include "service/TmdbApiClient.h"
#include "controller/MovieApiController.h"

using namespace Pistache;

int main()
{

    // Configura el servidor HTTP
    Address addr(Ipv4::any(), Port(5000));
    Http::Endpoint server(addr);

    Rest::Router router;

    // API Key de TMDb
    std::string tmdb_api_key = "33d6f9d20e39e104025fe947df34c197";
    auto tmdbApiClient = std::make_shared<TmdbApiClient>(tmdb_api_key);

    // Inicializa el controlador de películas
    MovieApiController movieApiController(tmdbApiClient);
    movieApiController.setupRoutes(router);

    // Inicializa servidor y configura el handler
    auto opts = Http::Endpoint::options().threads(1);
    server.init(opts);

    auto routerHandler = router.handler(); // Previene errores de binding
    server.setHandler(routerHandler);

    std::cout << "🎬 Servidor CineConecta corriendo en http://localhost:5000\n";
    server.serve();

    return 0;
}