#ifndef MOVIE_RESULT_H
#define MOVIE_RESULT_H

#include <string>
#include <nlohmann/json.hpp>  // Biblioteca para manipular objetos JSON

// Estructura que representa la información procesada de una película
// después de combinar datos de detalles, créditos y géneros.
struct MovieResult {
    long tmdbId;              // ID de la película en The Movie Database (TMDb)
    std::string title;        // Título de la película
    std::string year;         // Año de estreno (extraído de release_date)
    std::string imageUrl;     // URL completa del póster de la película
    std::string description;  // Descripción o sinopsis
    std::string genres;       // Lista de géneros separados por coma (ej. "Acción, Aventura")
    std::string director;     // Nombre del director principal
    std::string writers;      // Lista de guionistas separados por coma
    std::string actors;       // Lista de actores principales separados por coma
    double generalRating;     // Calificación promedio de la película (vote_average)
};

// Función para convertir un objeto MovieResult a JSON
// Utilizada por la biblioteca nlohmann::json al serializar respuestas
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
