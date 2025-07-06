#include "TmdbApiClient.h"
#include <iostream>
#include <stdexcept>

TmdbApiClient::TmdbApiClient(const std::string& apiKey)
    : cli("https://api.themoviedb.org"),
      tmdbApiKey(apiKey),
      tmdbBaseUrl("https://api.themoviedb.org/3"),
      tmdbImageBaseUrl("https://image.tmdb.org/t/p/")
{
    if (tmdbApiKey.empty()) {
        std::cerr << "ADVERTENCIA: La clave API de TMDb proporcionada está vacía." << std::endl;
    } else {
        std::cout << "TMDB_API_KEY cargada desde el constructor (hardcoded)." << std::endl;
    }

    cli.enable_server_certificate_verification(true);

    if (!loadConfiguration()) {
        std::cerr << "ERROR: No se pudo cargar la configuración de imágenes de TMDb al inicio." << std::endl;
    }
}

std::optional<json> TmdbApiClient::makeTmdbGetRequest(const std::string& path, const httplib::Params& params) {
    httplib::Headers headers;
    httplib::Params all_params = params;
    all_params.emplace("api_key", tmdbApiKey);
    all_params.emplace("language", "es-ES");

    std::string full_url_for_logging = tmdbBaseUrl + path;
    if (!all_params.empty()) {
        full_url_for_logging += "?";
        for (const auto& p : all_params) full_url_for_logging += p.first + "=" + p.second + "&";
        full_url_for_logging.pop_back();
    }
    std::cout << "Realizando petición a TMDb: " << full_url_for_logging << std::endl;

    if (auto res = cli.Get((tmdbBaseUrl + path).c_str(), all_params, headers)) {
        if (res->status == 200) {
            try {
                return json::parse(res->body);
            } catch (const json::parse_error& e) {
                std::cerr << "Error al parsear respuesta TMDb: " << e.what() << "\n" << res->body << std::endl;
                return std::nullopt;
            }
        } else {
            std::cerr << "Respuesta TMDb con error: " << res->status << " - " << res->body << std::endl;
            return std::nullopt;
        }
    } else {
        std::cerr << "Fallo en conexión con TMDb: " << httplib::to_string(res.error()) << std::endl;
        return std::nullopt;
    }
}

bool TmdbApiClient::loadConfiguration() {
    std::cout << "Cargando configuración de TMDb..." << std::endl;
    auto config_json = makeTmdbGetRequest("/configuration");
    if (config_json && config_json->contains("images")) {
        imageSizes = (*config_json)["images"];
        std::cout << "Configuración de imágenes cargada exitosamente." << std::endl;
        return true;
    }
    return false;
}

std::optional<json> TmdbApiClient::searchMovies(const std::string& query) {
    httplib::Params params;
    params.emplace("query", query);
    return makeTmdbGetRequest("/search/movie", params);
}

std::optional<json> TmdbApiClient::getMovieDetails(long tmdbId) {
    std::string path = "/movie/" + std::to_string(tmdbId);
    return makeTmdbGetRequest(path);
}

std::optional<json> TmdbApiClient::getMovieCredits(long tmdbId) {
    std::string path = "/movie/" + std::to_string(tmdbId) + "/credits";
    return makeTmdbGetRequest(path);
}

std::optional<json> TmdbApiClient::getMovieImages(long tmdbId) {
    std::string path = "/movie/" + std::to_string(tmdbId) + "/images";
    return makeTmdbGetRequest(path);
}

std::string TmdbApiClient::buildFullImageUrl(const std::string& relativePath, const std::string& size) {
    if (relativePath.empty()) return "";
    if (imageSizes.empty() || !imageSizes.contains("secure_base_url")) {
        return tmdbImageBaseUrl + size + relativePath;
    }
    return imageSizes["secure_base_url"].get<std::string>() + size + relativePath;
}

std::optional<json> TmdbApiClient::discoverByGenre(const std::string& genreId) {
    httplib::Params params;
    params.emplace("with_genres", genreId);
    return makeTmdbGetRequest("/discover/movie", params);
}

std::optional<json> TmdbApiClient::discoverByYear(int year) {
    httplib::Params params;
    params.emplace("primary_release_year", std::to_string(year));
    return makeTmdbGetRequest("/discover/movie", params);
}