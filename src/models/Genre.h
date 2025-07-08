#ifndef GENRE_H
#define GENRE_H

#include <string>
#include <nlohmann/json.hpp>  // Biblioteca para manejo de objetos JSON

// Estructura que representa un género cinematográfico (por ejemplo, "Acción", "Drama")
struct Genre {
    int id;              // Identificador único del género (según TMDb)
    std::string name;    // Nombre del género
};

// Función que convierte un objeto Genre a un objeto JSON
// Es utilizada automáticamente por la biblioteca nlohmann::json
inline void to_json(nlohmann::json& j, const Genre& g) {
    j = {
        {"id", g.id},
        {"name", g.name}
    };
}

#endif // GENRE_H