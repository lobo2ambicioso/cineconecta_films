#ifndef MOVIE_SERVICE_H
#define MOVIE_SERVICE_H

#include "TmdbApiClient.h"
#include "../models/MovieResult.h"
#include <optional>
#include <vector>

class MovieService {
public:
    explicit MovieService(std::shared_ptr<TmdbApiClient> apiClient);

    std::vector<MovieResult> searchMovies(const std::string& query, const std::string& genreId = "", std::optional<int> year = std::nullopt);
    std::optional<MovieResult> getMovieDetails(long tmdbId);

private:
    std::shared_ptr<TmdbApiClient> tmdbApiClient;

    std::string extractGenres(const nlohmann::json& genresArray);
    std::string extractDirector(const nlohmann::json& crewArray);
    std::string extractWriters(const nlohmann::json& crewArray);
    std::string extractActors(const nlohmann::json& castArray);
};

#endif // MOVIE_SERVICE_H