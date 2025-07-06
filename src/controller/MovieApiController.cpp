#include "MovieApiController.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <pistache/http.h>
#include <pistache/mime.h>

using json = nlohmann::json;

MovieApiController::MovieApiController(std::shared_ptr<TmdbApiClient> tmdbApiClient)
    : tmdbApiClient(tmdbApiClient) {}

void MovieApiController::setupRoutes(Pistache::Rest::Router& router) {
    Pistache::Rest::Routes::Post(router, "/api/movies/search-proxy",
                                 Pistache::Rest::Routes::bind(&MovieApiController::handleMovieSearch, this));

    Pistache::Rest::Routes::Get(router, "/api/movies/:tmdbId/details-proxy",
                                Pistache::Rest::Routes::bind(&MovieApiController::handleMovieDetails, this));
}

void MovieApiController::handleMovieSearch(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    const auto MIME_JSON = Pistache::Http::Mime::MediaType::fromString("application/json");

    try {
        json request_body = json::parse(request.body());
        std::string query = request_body.value("query", "");
        std::string genre_id = request_body.value("genreId", "");
        std::optional<int> year = std::nullopt;
        if (request_body.contains("year") && request_body["year"].is_number()) {
            year = request_body["year"].get<int>();
        }

        std::optional<json> tmdb_response;
        if (!query.empty()) {
            tmdb_response = tmdbApiClient->searchMovies(query);
        } else {
            response.send(Pistache::Http::Code::Bad_Request,
                          R"({"error":"Debe proporcionar al menos un parámetro de búsqueda"})",
                          MIME_JSON);
            return;
        }

        if (tmdb_response && tmdb_response->contains("results") && (*tmdb_response)["results"].is_array()) {
            json results_for_java = json::array();
            for (const auto& movie_json : (*tmdb_response)["results"]) {
                long tmdbId = movie_json.value("id", 0L);
                std::string title = movie_json.value("title", "N/A");
                std::string releaseDate = movie_json.value("release_date", "");
                std::string year_str = (releaseDate.length() >= 4) ? releaseDate.substr(0, 4) : "N/A";
                std::string posterPath = "";
                if (movie_json.contains("poster_path") && movie_json["poster_path"].is_string()) {
                    posterPath = movie_json["poster_path"].get<std::string>();
                }
                std::string fullImageUrl = tmdbApiClient->buildFullImageUrl(posterPath);

                json movie_result = {
                    {"tmdbId", tmdbId},
                    {"title", title},
                    {"year", year_str},
                    {"imageUrl", fullImageUrl}
                };
                results_for_java.push_back(movie_result);
            }

            response.send(Pistache::Http::Code::Ok, results_for_java.dump(), MIME_JSON);
        } else {
            response.send(Pistache::Http::Code::Not_Found,
                          R"({"error":"No se encontraron resultados"})",
                          MIME_JSON);
        }

    } catch (const json::parse_error& e) {
        std::cerr << "Error al parsear JSON: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Bad_Request,
                      R"({"error":"JSON inválido"})", MIME_JSON);
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado en handleMovieSearch: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error,
                      R"({"error":"Error interno del servidor"})", MIME_JSON);
    }
}

void MovieApiController::handleMovieDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    const auto MIME_JSON = Pistache::Http::Mime::MediaType::fromString("application/json");

    try {
        std::string tmdbIdStr = request.param(":tmdbId").as<std::string>();
        if (!std::all_of(tmdbIdStr.begin(), tmdbIdStr.end(), ::isdigit)) {
            response.send(Pistache::Http::Code::Bad_Request, R"({"error":"ID inválido"})", MIME_JSON);
            return;
        }

        long tmdbId = std::stol(tmdbIdStr);
        auto movie_details_json = tmdbApiClient->getMovieDetails(tmdbId);
        auto credits_json = tmdbApiClient->getMovieCredits(tmdbId);

        if (!movie_details_json) {
            response.send(Pistache::Http::Code::Not_Found, R"({"error":"Película no encontrada"})", MIME_JSON);
            return;
        }

        json movie_data = *movie_details_json;
        json movie_result;

        movie_result["tmdbId"] = movie_data.value("id", 0L);
        movie_result["title"] = movie_data.value("title", "N/A");
        movie_result["description"] = movie_data.value("overview", "N/A");
        movie_result["vote_average"] = movie_data.value("vote_average", 0.0);

        std::string releaseDate = movie_data.value("release_date", "");
        movie_result["year"] = (releaseDate.length() >= 4) ? releaseDate.substr(0, 4) : "N/A";

        std::string posterPath = "";
        if (movie_data.contains("poster_path") && movie_data["poster_path"].is_string()) {
            posterPath = movie_data["poster_path"].get<std::string>();
        }
        movie_result["imageUrl"] = tmdbApiClient->buildFullImageUrl(posterPath);

        if (movie_data.contains("genres") && movie_data["genres"].is_array()) {
            std::string genresStr;
            for (const auto& genre : movie_data["genres"]) {
                if (!genresStr.empty()) genresStr += ", ";
                genresStr += genre.value("name", "");
            }
            movie_result["genres"] = genresStr;
        } else {
            movie_result["genres"] = "N/A";
        }

        std::string director = "N/A", writers = "N/A", actors = "N/A";
        if (credits_json) {
            if ((*credits_json).contains("crew") && (*credits_json)["crew"].is_array()) {
                for (const auto& member : (*credits_json)["crew"]) {
                    std::string job = member.value("job", "");
                    std::string name = member.value("name", "");
                    if (job == "Director" && director == "N/A") director = name;
                    if ((job == "Writer" || job == "Screenplay") && writers == "N/A") writers = name;
                }
            }

            if ((*credits_json).contains("cast") && (*credits_json)["cast"].is_array()) {
                std::string actorList;
                int count = 0;
                for (const auto& actor : (*credits_json)["cast"]) {
                    if (count++ >= 5) break;
                    if (!actorList.empty()) actorList += ", ";
                    actorList += actor.value("name", "");
                }
                if (!actorList.empty()) actors = actorList;
            }
        }

        movie_result["director"] = director;
        movie_result["writers"] = writers;
        movie_result["actors"] = actors;

        response.send(Pistache::Http::Code::Ok, movie_result.dump(), MIME_JSON);

    } catch (const std::exception& e) {
        std::cerr << "Error en handleMovieDetails: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error,
                      R"({"error":"Error interno del servidor"})", MIME_JSON);
    }
}