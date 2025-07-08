#ifndef CREW_MEMBER_H
#define CREW_MEMBER_H

#include <string>
#include <nlohmann/json.hpp>  // Biblioteca para manipulación de objetos JSON

// Estructura que representa a un miembro del equipo técnico de una película (crew)
struct CrewMember {
    int id;                 // ID único del miembro del equipo
    std::string name;       // Nombre del miembro del crew
    std::string job;        // Rol o función específica (por ejemplo, "Director", "Guionista")
    std::string department; // Departamento al que pertenece (por ejemplo, "Dirección", "Producción")
};

// Función que convierte un objeto CrewMember a una representación JSON
// Esta función es utilizada por la biblioteca nlohmann::json para la serialización automática
inline void to_json(nlohmann::json& j, const CrewMember& c) {
    j = {
        {"id", c.id},
        {"name", c.name},
        {"job", c.job},
        {"department", c.department}
    };
}

#endif // CREW_MEMBER_H