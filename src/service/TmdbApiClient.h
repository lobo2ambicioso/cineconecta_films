#ifndef TMDB_API_CLIENT_H
#define TMDB_API_CLIENT_H

#include <string>
#include <nlohmann/json.hpp>
#include <optional>
#include <httplib.h> // Necesario para httplib::Client

using json = nlohmann::json;

class TmdbApiClient {
public:
    // Constructor que acepta la clave API directamente
    TmdbApiClient(const std::string& apiKey);

    // Cargar la configuración de imágenes
    bool loadConfiguration();

    // Métodos para interactuar con la API de TMDb
    std::optional<json> searchMovies(const std::string& query);
    std::optional<json> getMovieDetails(long tmdbId);
    std::optional<json> getMovieImages(long tmdbId);
    std::optional<json> getMovieCredits(long tmdbId); // 👈 AÑADIDO AQUÍ

    // Construcción de URL completa de imagen
    std::string buildFullImageUrl(const std::string& relativePath, const std::string& size = "w342");

private:
    httplib::Client cli;
    std::string tmdbApiKey;
    std::string tmdbBaseUrl;
    std::string tmdbImageBaseUrl;
    json imageSizes;

    std::optional<json> makeTmdbGetRequest(const std::string& path, const httplib::Params& params = {});
};

#endif // TMDB_API_CLIENT_H