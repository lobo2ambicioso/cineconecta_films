#ifndef TMDB_PERSON_SEARCH_RESPONSE_H
#define TMDB_PERSON_SEARCH_RESPONSE_H

#include <vector>
#include "TmdbPersonResult.h"

struct TmdbPersonSearchResponse {
    int page;
    std::vector<TmdbPersonResult> results;
    int totalPages;
    int totalResults;
};

#endif // TMDB_PERSON_SEARCH_RESPONSE_H