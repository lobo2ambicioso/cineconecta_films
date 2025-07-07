#ifndef TMDB_PERSON_RESULT_H
#define TMDB_PERSON_RESULT_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "MovieResult.h"

struct TmdbPersonResult {
    long id;
    std::string name;
    double popularity;
    std::string profilePath;
    std::string knownForDepartment;
    std::vector<MovieResult> knownFor;
};

#endif // TMDB_PERSON_RESULT_H