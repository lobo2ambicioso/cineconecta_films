#ifndef PERSON_CAST_CREDIT_H
#define PERSON_CAST_CREDIT_H

#include <string>
#include <nlohmann/json.hpp>

struct PersonCastCredit {
    long id;
    std::string title;
    std::string releaseDate;
    std::string posterPath;
    std::string characterName;
};

inline void to_json(nlohmann::json& j, const PersonCastCredit& p) {
    j = {
        {"id", p.id},
        {"title", p.title},
        {"release_date", p.releaseDate},
        {"poster_path", p.posterPath},
        {"character", p.characterName}
    };
}

#endif // PERSON_CAST_CREDIT_H