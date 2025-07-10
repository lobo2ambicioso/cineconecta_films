#include "MovieService.h"
#include <iostream>

MovieService::MovieService(std::shared_ptr<TmdbApiClient> apiClient) : tmdbApiClient(apiClient) {}

std::vector<MovieResult> MovieService::searchMovies(const std::string& query, const std::string& genreId, std::optional<int> year) {
    std::optional<nlohmann::json> tmdb_response;

    if (!query.empty()) {
        tmdb_response = tmdbApiClient->searchMovies(query);
    } else if (!genreId.empty()) {
        tmdb_response = tmdbApiClient->discoverByGenre(genreId);
    } else if (year.has_value()) {
        tmdb_response = tmdbApiClient->discoverByYear(year.value());
    }

    std::vector<MovieResult> results;

    if (tmdb_response && tmdb_response->contains("results") && (*tmdb_response)["results"].is_array()) {
        for (const auto& movie_json : (*tmdb_response)["results"]) {
            MovieResult result;

            // Validar cada campo individualmente por tipo
            result.tmdbId = movie_json.value("id", 0L);

            result.title = movie_json.contains("title") && movie_json["title"].is_string()
                           ? movie_json["title"].get<std::string>()
                           : "N/A";

            if (movie_json.contains("release_date") && movie_json["release_date"].is_string()) {
                std::string releaseDate = movie_json["release_date"].get<std::string>();
                result.year = releaseDate.length() >= 4 ? releaseDate.substr(0, 4) : "N/A";
            } else {
                result.year = "N/A";
            }

            if (movie_json.contains("poster_path") && movie_json["poster_path"].is_string()) {
                std::string posterPath = movie_json["poster_path"].get<std::string>();
                result.imageUrl = tmdbApiClient->buildFullImageUrl(posterPath);
            } else {
                result.imageUrl = "";
            }

            results.push_back(result);
        }
    }

    return results;
}

std::optional<MovieResult> MovieService::getMovieDetails(long tmdbId) {
    auto movie_json = tmdbApiClient->getMovieDetails(tmdbId);
    if (!movie_json) return std::nullopt;

    auto credits_json = tmdbApiClient->getMovieCredits(tmdbId);

    MovieResult result;
    result.tmdbId = movie_json->value("id", 0L);

    result.title = movie_json->contains("title") && (*movie_json)["title"].is_string()
                   ? (*movie_json)["title"].get<std::string>()
                   : "N/A";

    result.description = movie_json->contains("overview") && (*movie_json)["overview"].is_string()
                         ? (*movie_json)["overview"].get<std::string>()
                         : "N/A";

    result.generalRating = movie_json->contains("vote_average") && (*movie_json)["vote_average"].is_number()
                           ? (*movie_json)["vote_average"].get<double>()
                           : 0.0;

    if (movie_json->contains("release_date") && (*movie_json)["release_date"].is_string()) {
        std::string releaseDate = (*movie_json)["release_date"].get<std::string>();
        result.year = releaseDate.length() >= 4 ? releaseDate.substr(0, 4) : "N/A";
    } else {
        result.year = "N/A";
    }

    if (movie_json->contains("poster_path") && (*movie_json)["poster_path"].is_string()) {
        result.imageUrl = tmdbApiClient->buildFullImageUrl((*movie_json)["poster_path"]);
    } else {
        result.imageUrl = "";
    }

    result.genres = extractGenres(*movie_json);

    if (credits_json) {
        const auto& credits = *credits_json;

        if (credits.contains("crew") && credits["crew"].is_array()) {
            result.director = extractDirector(credits["crew"]);
            result.writers = extractWriters(credits["crew"]);
        } else {
            result.director = "N/A";
            result.writers = "N/A";
        }

        if (credits.contains("cast") && credits["cast"].is_array()) {
            result.actors = extractActors(credits["cast"]);
        } else {
            result.actors = "N/A";
        }
    } else {
        result.director = "N/A";
        result.writers = "N/A";
        result.actors = "N/A";
    }

    return result;
}
// Métodos privados
std::string MovieService::extractGenres(const nlohmann::json& genresArray) {
    if (!genresArray.contains("genres")) return "N/A";
    std::string genresStr;
    for (const auto& genre : genresArray["genres"]) {
        if (!genresStr.empty()) genresStr += ", ";
        genresStr += genre.value("name", "");
    }
    return genresStr;
}

std::string MovieService::extractDirector(const nlohmann::json& crewArray) {
    for (const auto& member : crewArray) {
        if (member.value("job", "") == "Director") {
            return member.value("name", "N/A");
        }
    }
    return "N/A";
}

std::string MovieService::extractWriters(const nlohmann::json& crewArray) {
    for (const auto& member : crewArray) {
        std::string job = member.value("job", "");
        if (job == "Writer" || job == "Screenplay") {
            return member.value("name", "N/A");
        }
    }
    return "N/A";
}

std::string MovieService::extractActors(const nlohmann::json& castArray) {
    std::string actorList;
    int count = 0;
    for (const auto& actor : castArray) {
        if (count++ >= 5) break;
        if (!actorList.empty()) actorList += ", ";
        actorList += actor.value("name", "");
    }
    return actorList.empty() ? "N/A" : actorList;
}