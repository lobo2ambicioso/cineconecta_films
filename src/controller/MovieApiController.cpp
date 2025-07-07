#include "MovieApiController.h"
#include <nlohmann/json.hpp>      // Para trabajar con objetos JSON
#include <pistache/http.h>        // Para gestionar peticiones y respuestas HTTP
#include <pistache/mime.h>        // Para establecer tipos MIME
#include <iostream>               // Para salida por consola
#include <algorithm>              // Para funciones como std::all_of

using json = nlohmann::json;     // Alias para facilitar uso de JSON

// Constructor: recibe un cliente TMDb y construye el servicio MovieService
MovieApiController::MovieApiController(std::shared_ptr<TmdbApiClient> tmdbApiClient)
    : tmdbApiClient(tmdbApiClient),
      movieService(std::make_shared<MovieService>(tmdbApiClient)) {}

// Configura las rutas expuestas por la API
void MovieApiController::setupRoutes(Pistache::Rest::Router& router) {
    // Ruta POST para buscar películas por parámetros
    Pistache::Rest::Routes::Post(router, "/api/movies/search-proxy",
        Pistache::Rest::Routes::bind(&MovieApiController::handleMovieSearch, this));

    // Ruta GET para obtener detalles de una película por su tmdbId
    Pistache::Rest::Routes::Get(router, "/api/movies/:tmdbId/details-proxy",
        Pistache::Rest::Routes::bind(&MovieApiController::handleMovieDetails, this));
}

// Manejador de la ruta /api/movies/search-proxy (POST)
void MovieApiController::handleMovieSearch(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    const auto MIME_JSON = Pistache::Http::Mime::MediaType::fromString("application/json");

    // 📨 Captura el cuerpo crudo recibido y lo muestra en consola (debug)
    std::string rawBody = request.body();
    std::cout << "📩 Raw JSON recibido: " << rawBody << std::endl;

    json request_body;
    try {
        request_body = json::parse(rawBody);  // Intenta parsear el JSON
    } catch (const json::parse_error& e) {
        std::cerr << "❌ Error de parsing JSON: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Bad_Request,
                      R"({"error":"JSON inválido al parsear"})", MIME_JSON);
        return;
    }

    try {
        // Obtiene los parámetros del cuerpo JSON
        std::string query = request_body.value("query", "");
        std::string genre_id = request_body.value("genreId", "");
        std::optional<int> year = std::nullopt;

        // Verifica si se incluye el año y lo convierte
        if (request_body.contains("year") && request_body["year"].is_number()) {
            year = request_body["year"].get<int>();
        }

        // Llama al servicio MovieService para buscar películas
        auto movieResults = movieService->searchMovies(query, genre_id, year);

        // ⚠️ Si no se encontraron resultados
        if (movieResults.empty()) {
            std::cerr << "⚠️ No se encontraron resultados válidos desde MovieService." << std::endl;
            response.send(Pistache::Http::Code::Not_Found,
                          R"({"error":"No se encontraron resultados válidos"})", MIME_JSON);
            return;
        }

        // ✅ Si hay resultados, los devuelve como JSON
        json responseBody = movieResults;
        response.send(Pistache::Http::Code::Ok, responseBody.dump(), MIME_JSON);

    } catch (const std::exception& e) {
        // Manejo de errores internos
        std::cerr << "❌ Error inesperado en handleMovieSearch: " << e.what() << std::endl;
        std::string errorMsg = std::string(R"({"error":"Error interno del servidor: )") + e.what() + R"("})";
        response.send(Pistache::Http::Code::Internal_Server_Error, errorMsg, MIME_JSON);
    }
}

// Manejador de la ruta /api/movies/:tmdbId/details-proxy (GET)
void MovieApiController::handleMovieDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    const auto MIME_JSON = Pistache::Http::Mime::MediaType::fromString("application/json");

    try {
        // Extrae el parámetro tmdbId desde la URL
        std::string tmdbIdStr = request.param(":tmdbId").as<std::string>();

        // Verifica que sea un número entero válido
        if (!std::all_of(tmdbIdStr.begin(), tmdbIdStr.end(), ::isdigit)) {
            response.send(Pistache::Http::Code::Bad_Request, R"({"error":"ID inválido"})", MIME_JSON);
            return;
        }

        // Convierte a entero y obtiene los detalles
        long tmdbId = std::stol(tmdbIdStr);
        auto result = movieService->getMovieDetails(tmdbId);

        // Si no se encuentra la película
        if (!result.has_value()) {
            response.send(Pistache::Http::Code::Not_Found, R"({"error":"Película no encontrada"})", MIME_JSON);
            return;
        }

        // ✅ Retorna los detalles de la película como JSON
        response.send(Pistache::Http::Code::Ok, json(result.value()).dump(), MIME_JSON);

    } catch (const std::exception& e) {
        // Manejo de errores generales
        std::cerr << "Error en detalle: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"error":"Error interno del servidor"})", MIME_JSON);
    }
}