#ifndef TMDB_API_CLIENT_H
#define TMDB_API_CLIENT_H

// Inclusiones necesarias
#include <string>
#include <nlohmann/json.hpp>   // Para manejar respuestas JSON
#include <optional>            // Para valores opcionales seguros
#include <httplib.h>           // Cliente HTTP

// Alias para usar 'json' directamente
using json = nlohmann::json;

/**
 * @class TmdbApiClient
 * @brief Cliente HTTP para conectarse a la API pública de TMDb (The Movie Database).
 *
 * Esta clase encapsula todas las llamadas necesarias para buscar películas,
 * obtener detalles, créditos, imágenes y construir URLs completas de imágenes.
 * También permite realizar búsquedas por género o año, y cargar la configuración
 * general de la API como los tamaños de imagen disponibles.
 */
class TmdbApiClient {
public:
    /**
     * @brief Constructor que inicializa el cliente con la clave API de TMDb.
     * @param apiKey Clave API proporcionada por TMDb.
     */
    TmdbApiClient(const std::string& apiKey);

    /**
     * @brief Carga la configuración de la API (base URL de imágenes, tamaños, etc.).
     * @return true si la configuración fue cargada correctamente, false en caso contrario.
     */
    bool loadConfiguration();

    // -------------------------------
    // Métodos públicos para consumo de API
    // -------------------------------

    /**
     * @brief Busca películas por nombre.
     * @param query Título o parte del título.
     * @return JSON con resultados de búsqueda o nullopt si falla.
     */
    std::optional<json> searchMovies(const std::string& query);

    /**
     * @brief Obtiene detalles completos de una película.
     * @param tmdbId ID de la película en TMDb.
     * @return JSON con los detalles o nullopt si falla.
     */
    std::optional<json> getMovieDetails(long tmdbId);

    /**
     * @brief Obtiene las imágenes asociadas a una película.
     * @param tmdbId ID de la película en TMDb.
     * @return JSON con las imágenes o nullopt si falla.
     */
    std::optional<json> getMovieImages(long tmdbId);

    /**
     * @brief Obtiene los créditos (reparto y equipo técnico) de una película.
     * @param tmdbId ID de la película en TMDb.
     * @return JSON con créditos o nullopt si falla.
     */
    std::optional<json> getMovieCredits(long tmdbId);

    /**
     * @brief Descubre películas filtradas por género.
     * @param genreId ID del género.
     * @return JSON con resultados o nullopt.
     */
    std::optional<json> discoverByGenre(const std::string& genreId);

    /**
     * @brief Descubre películas por año de lanzamiento.
     * @param year Año numérico (ej. 2023).
     * @return JSON con resultados o nullopt.
     */
    std::optional<json> discoverByYear(int year);

    /**
     * @brief Construye una URL completa para acceder a una imagen.
     * @param relativePath Ruta relativa de la imagen (devuelta por la API).
     * @param size Tamaño de la imagen (ej. "w342", "original").
     * @return URL completa para acceder a la imagen.
     */
    std::string buildFullImageUrl(const std::string& relativePath, const std::string& size = "w342");

private:
    // -------------------------------
    // Atributos internos
    // -------------------------------

    httplib::Client cli;          // Cliente HTTP configurado para acceder a TMDb
    std::string tmdbApiKey;       // Clave API
    std::string tmdbBaseUrl;      // URL base de la API REST
    std::string tmdbImageBaseUrl; // URL base para imágenes (fallback)
    json imageSizes;              // Configuración de tamaños de imagen obtenida dinámicamente

    /**
     * @brief Método privado que encapsula todas las solicitudes GET a la API.
     * @param path Ruta del endpoint (ej. "/movie/123").
     * @param params Parámetros de la solicitud.
     * @return JSON parseado o nullopt si falla.
     */
    std::optional<json> makeTmdbGetRequest(const std::string& path, const httplib::Params& params = {});
};

#endif // TMDB_API_CLIENT_H