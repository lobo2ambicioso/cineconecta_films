#ifndef PERSON_CREW_CREDIT_H
#define PERSON_CREW_CREDIT_H

#include <string>
#include <nlohmann/json.hpp>

struct PersonCrewCredit {
    long id;
    std::string job;
    std::string department;
    std::string title;
    std::string releaseDate;
    std::string posterPath;
};

inline void to_json(nlohmann::json& j, const PersonCrewCredit& p) {
    j = {
        {"id", p.id},
        {"job", p.job},
        {"department", p.department},
        {"title", p.title},
        {"release_date", p.releaseDate},
        {"poster_path", p.posterPath}
    };
}

#endif // PERSON_CREW_CREDIT_H