#ifndef MOVIE_RESULT_H
#define MOVIE_RESULT_H

#include <string>
#include <nlohmann/json.hpp>

struct MovieResult {
    long tmdbId;
    std::string title;
    std::string year;
    std::string imageUrl;
    std::string description;
    std::string genres;
    std::string director;
    std::string writers;
    std::string actors;
    int generalRating;
};

inline void to_json(nlohmann::json& j, const MovieResult& m) {
    j = {
        {"tmdbId", m.tmdbId},
        {"title", m.title},
        {"year", m.year},
        {"imageUrl", m.imageUrl},
        {"description", m.description},
        {"genres", m.genres},
        {"director", m.director},
        {"writers", m.writers},
        {"actors", m.actors},
        {"generalRating", m.generalRating}
    };
}

#endif // MOVIE_RESULT_H

