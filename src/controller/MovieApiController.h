#ifndef MOVIE_API_CONTROLLER_H
#define MOVIE_API_CONTROLLER_H

#include <pistache/router.h>
#include <memory> // For std::shared_ptr
#include "../service/TmdbApiClient.h" // Go up one directory (from controller) then into service/

class MovieApiController {
public:
    // Constructor that takes a shared_ptr to TmdbApiClient
    MovieApiController(std::shared_ptr<TmdbApiClient> apiClient);

    // Method to set up routes for the router
    void setupRoutes(Pistache::Rest::Router& router);

private: // <--- Aquí es donde debes añadir las declaraciones
    std::shared_ptr<TmdbApiClient> tmdbApiClient;

    // DECLARACIONES DE LOS MÉTODOS MANEJADORES DE RUTAS
    void handleMovieSearch(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleMovieDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    // You might have other private members or methods here
    // Example: Pistache::Http::Endpoint::Binding binding;
};

#endif // MOVIE_API_CONTROLLER_H