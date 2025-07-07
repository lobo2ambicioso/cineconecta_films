#ifndef MOVIE_SERVICE_H
#define MOVIE_SERVICE_H

// Inclusión del cliente que maneja las llamadas HTTP a la API de TMDb
#include "TmdbApiClient.h"

// Inclusión del modelo que representa una película procesada por el sistema
#include "../models/MovieResult.h"

// Librerías estándar necesarias para trabajar con std::optional y std::vector
#include <optional>
#include <vector>

/**
 * @class MovieService
 * @brief Servicio que gestiona la lógica relacionada con películas, utilizando datos obtenidos desde la API de TMDb.
 *
 * Esta clase proporciona métodos para buscar películas por título, género o año, 
 * así como para obtener los detalles completos de una película en particular.
 * Es un componente clave en la arquitectura del backend de CineConecta, 
 * ya que encapsula la lógica de negocio y el tratamiento de los datos de películas.
 */
class MovieService {
public:
    /**
     * @brief Constructor que recibe una instancia del cliente de la API de TMDb.
     * @param apiClient Puntero compartido al cliente encargado de realizar solicitudes a TMDb.
     */
    explicit MovieService(std::shared_ptr<TmdbApiClient> apiClient);

    /**
     * @brief Busca películas según el título, género o año.
     * @param query Título o parte del título de la película.
     * @param genreId Identificador del género (opcional).
     * @param year Año de lanzamiento (opcional).
     * @return Lista de resultados con información básica de cada película encontrada.
     */
    std::vector<MovieResult> searchMovies(const std::string& query, const std::string& genreId = "", std::optional<int> year = std::nullopt);

    /**
     * @brief Obtiene los detalles completos de una película por su ID de TMDb.
     * @param tmdbId Identificador numérico de la película en TMDb.
     * @return Objeto MovieResult con toda la información disponible o std::nullopt si no se encuentra.
     */
    std::optional<MovieResult> getMovieDetails(long tmdbId);

private:
    // Cliente HTTP para conectarse con la API de TMDb
    std::shared_ptr<TmdbApiClient> tmdbApiClient;

    // Métodos auxiliares privados para extraer campos específicos del JSON

    /**
     * @brief Extrae una cadena con los nombres de los géneros de una película.
     * @param genresArray JSON con el arreglo de géneros.
     * @return Géneros concatenados como cadena separada por comas.
     */
    std::string extractGenres(const nlohmann::json& genresArray);

    /**
     * @brief Extrae el nombre del director de una película a partir del array "crew".
     * @param crewArray JSON que contiene el equipo técnico de la película.
     * @return Nombre del director o "N/A" si no se encuentra.
     */
    std::string extractDirector(const nlohmann::json& crewArray);

    /**
     * @brief Extrae el nombre del guionista o responsable del guion.
     * @param crewArray JSON que contiene el equipo técnico.
     * @return Nombre del guionista o "N/A" si no se encuentra.
     */
    std::string extractWriters(const nlohmann::json& crewArray);

    /**
     * @brief Extrae una lista de hasta cinco actores principales.
     * @param castArray JSON que contiene el reparto principal.
     * @return Cadena con nombres de actores separados por comas o "N/A".
     */
    std::string extractActors(const nlohmann::json& castArray);
};

#endif // MOVIE_SERVICE_H