#include "MovieApiController.h"
#include <nlohmann/json.hpp>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

MovieApiController::MovieApiController(std::shared_ptr<TmdbApiClient> tmdbApiClient)
    : tmdbApiClient(tmdbApiClient),
      movieService(std::make_shared<MovieService>(tmdbApiClient)) {}

void MovieApiController::setupRoutes(Pistache::Rest::Router& router) {
    Pistache::Rest::Routes::Post(router, "/api/movies/search-proxy",
        Pistache::Rest::Routes::bind(&MovieApiController::handleMovieSearch, this));

    Pistache::Rest::Routes::Get(router, "/api/movies/:tmdbId/details-proxy",
        Pistache::Rest::Routes::bind(&MovieApiController::handleMovieDetails, this));
}

void MovieApiController::handleMovieSearch(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    const auto MIME_JSON = Pistache::Http::Mime::MediaType::fromString("application/json");

    // 🔍 Registrar el cuerpo crudo recibido
    std::string rawBody = request.body();
    std::cout << "📩 Raw JSON recibido: " << rawBody << std::endl;

    json request_body;
    try {
        request_body = json::parse(rawBody);
    } catch (const json::parse_error& e) {
        std::cerr << "❌ Error de parsing JSON: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Bad_Request,
                      R"({"error":"JSON inválido al parsear"})", MIME_JSON);
        return;
    }

    try {
        std::string query = request_body.value("query", "");
        std::string genre_id = request_body.value("genreId", "");
        std::optional<int> year = std::nullopt;

        if (request_body.contains("year") && request_body["year"].is_number()) {
            year = request_body["year"].get<int>();
        }

        auto movieResults = movieService->searchMovies(query, genre_id, year);

        // 🛡️ Validar si no se encontraron resultados
        if (movieResults.empty()) {
            std::cerr << "⚠️ No se encontraron resultados válidos desde MovieService." << std::endl;
            response.send(Pistache::Http::Code::Not_Found,
                          R"({"error":"No se encontraron resultados válidos"})", MIME_JSON);
            return;
        }

        // ✅ Respuesta exitosa
        json responseBody = movieResults;
        response.send(Pistache::Http::Code::Ok, responseBody.dump(), MIME_JSON);

    } catch (const std::exception& e) {
        std::cerr << "❌ Error inesperado en handleMovieSearch: " << e.what() << std::endl;
        std::string errorMsg = std::string(R"({"error":"Error interno del servidor: )") + e.what() + R"("})";
        response.send(Pistache::Http::Code::Internal_Server_Error, errorMsg, MIME_JSON);
    }
}

void MovieApiController::handleMovieDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    const auto MIME_JSON = Pistache::Http::Mime::MediaType::fromString("application/json");

    try {
        std::string tmdbIdStr = request.param(":tmdbId").as<std::string>();
        if (!std::all_of(tmdbIdStr.begin(), tmdbIdStr.end(), ::isdigit)) {
            response.send(Pistache::Http::Code::Bad_Request, R"({"error":"ID inválido"})", MIME_JSON);
            return;
        }

        long tmdbId = std::stol(tmdbIdStr);
        auto result = movieService->getMovieDetails(tmdbId);

        if (!result.has_value()) {
            response.send(Pistache::Http::Code::Not_Found, R"({"error":"Película no encontrada"})", MIME_JSON);
            return;
        }

        response.send(Pistache::Http::Code::Ok, json(result.value()).dump(), MIME_JSON);

    } catch (const std::exception& e) {
        std::cerr << "Error en detalle: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"error":"Error interno del servidor"})", MIME_JSON);
    }
}