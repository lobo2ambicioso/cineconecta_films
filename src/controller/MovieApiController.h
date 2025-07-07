#ifndef MOVIE_API_CONTROLLER_H
#define MOVIE_API_CONTROLLER_H

// Incluye el router de Pistache (para manejar rutas HTTP)
#include <pistache/router.h>
#include <memory>

// Inclusión de servicios necesarios
#include "../service/TmdbApiClient.h"     // Cliente para consumir la API de TMDb
#include "../service/MovieService.h"      // Servicio que abstrae la lógica de negocio de películas
#include "../models/MovieResult.h"        // Modelo de datos para resultados de películas

/**
 * @class MovieApiController
 * @brief Controlador REST que expone endpoints relacionados con películas.
 *
 * Esta clase actúa como intermediario entre las solicitudes HTTP del cliente
 * (por ejemplo, frontend de CineConecta) y los servicios internos del backend,
 * particularmente MovieService. Utiliza Pistache como framework HTTP y define
 * las rutas necesarias para buscar películas y obtener detalles específicos.
 */
class MovieApiController {
public:
    /**
     * @brief Constructor que recibe una instancia del cliente TMDb.
     * @param apiClient Cliente HTTP para acceder a la API de TMDb.
     */
    MovieApiController(std::shared_ptr<TmdbApiClient> apiClient);

    /**
     * @brief Configura las rutas REST en el router proporcionado.
     * @param router Objeto de router de Pistache para registrar los endpoints.
     */
    void setupRoutes(Pistache::Rest::Router& router);

private:
    // Cliente TMDb compartido
    std::shared_ptr<TmdbApiClient> tmdbApiClient;

    // Servicio de películas que encapsula la lógica de negocio
    std::shared_ptr<MovieService> movieService;

    /**
     * @brief Manejador para la búsqueda de películas.
     * Ruta: POST /api/movies/search-proxy
     * Extrae los parámetros del cuerpo JSON (query, genreId, year).
     */
    void handleMovieSearch(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief Manejador para obtener detalles de una película por ID.
     * Ruta: GET /api/movies/:tmdbId/details-proxy
     */
    void handleMovieDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
};

#endif // MOVIE_API_CONTROLLER_H