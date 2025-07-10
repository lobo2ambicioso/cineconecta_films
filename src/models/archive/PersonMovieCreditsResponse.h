#ifndef PERSON_MOVIE_CREDITS_RESPONSE_H
#define PERSON_MOVIE_CREDITS_RESPONSE_H

#include <vector>
#include "PersonCastCredit.h"
#include "PersonCrewCredit.h"

struct PersonMovieCreditsResponse {
    long id;
    std::vector<PersonCastCredit> movieCastCredits;
    std::vector<PersonCrewCredit> movieCrewCredits;
};

#endif // PERSON_MOVIE_CREDITS_RESPONSE_H