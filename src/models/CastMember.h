#ifndef CAST_MEMBER_H
#define CAST_MEMBER_H

#include <string>
#include <nlohmann/json.hpp>

struct CastMember {
    int id;
    std::string name;
    std::string characterName;
};

inline void to_json(nlohmann::json& j, const CastMember& c) {
    j = {
        {"id", c.id},
        {"name", c.name},
        {"character", c.characterName}
    };
}

#endif // CAST_MEMBER_H