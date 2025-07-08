#ifndef CAST_MEMBER_H
#define CAST_MEMBER_H

#include <string>
#include <nlohmann/json.hpp>  // Biblioteca para trabajar con JSON en C++

// Estructura que representa a un miembro del elenco (actor o actriz)
struct CastMember {
    int id;                     // ID único del miembro del elenco
    std::string name;           // Nombre del actor o actriz
    std::string characterName;  // Nombre del personaje interpretado en la película
};

// Función para convertir un objeto CastMember a formato JSON
// Esto es útil para serializar la estructura como parte de una respuesta HTTP o archivo
inline void to_json(nlohmann::json& j, const CastMember& c) {
    j = {
        {"id", c.id},
        {"name", c.name},
        {"character", c.characterName}  // Se guarda con la clave "character" por compatibilidad
    };
}

#endif // CAST_MEMBER_H