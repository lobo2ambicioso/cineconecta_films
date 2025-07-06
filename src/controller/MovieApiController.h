#ifndef MOVIE_API_CONTROLLER_H
#define MOVIE_API_CONTROLLER_H

#include <pistache/router.h>
#include <memory>
#include "../service/TmdbApiClient.h"
#include "../service/MovieService.h" // Nuevo servicio
#include "../models/MovieResult.h"

class MovieApiController {
public:
    MovieApiController(std::shared_ptr<TmdbApiClient> apiClient);
    void setupRoutes(Pistache::Rest::Router& router);

private:
    std::shared_ptr<TmdbApiClient> tmdbApiClient;
    std::shared_ptr<MovieService> movieService; // Nuevo

    void handleMovieSearch(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleMovieDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
};

#endif // MOVIE_API_CONTROLLER_H