#ifndef GENRE_H
#define GENRE_H

#include <string>
#include <nlohmann/json.hpp>

struct Genre {
    int id;
    std::string name;
};

inline void to_json(nlohmann::json& j, const Genre& g) {
    j = {
        {"id", g.id},
        {"name", g.name}
    };
}

#endif // GENRE_H