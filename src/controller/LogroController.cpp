#include "LogroController.h"
#include <nlohmann/json.hpp>
#include <pistache/router.h>
#include <pistache/http.h>
#include <pistache/endpoint.h>

using namespace Pistache;
using namespace Pistache::Rest;
using json = nlohmann::json;

void LogroController::setupRoutes(Rest::Router &router)
{
    Rest::Routes::Get(router, "/api/logros/test", [](const Rest::Request &req, Http::ResponseWriter response) -> Rest::Route::Result
                      {
        json logro = {
            {"id", 1},
            {"nombre", "Primer logro"},
            {"descripcion", "Este es el primer logro"},
            {"puntos", 100}
        };

        response.send(Http::Code::Ok, logro.dump(), MIME(Application, Json));
        return Rest::Route::Result::Ok; });
}