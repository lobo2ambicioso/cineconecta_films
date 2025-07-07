#ifndef CREW_MEMBER_H
#define CREW_MEMBER_H

#include <string>
#include <nlohmann/json.hpp>

struct CrewMember {
    int id;
    std::string name;
    std::string job;
    std::string department;
};

inline void to_json(nlohmann::json& j, const CrewMember& c) {
    j = {
        {"id", c.id},
        {"name", c.name},
        {"job", c.job},
        {"department", c.department}
    };
}

#endif // CREW_MEMBER_H