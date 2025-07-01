#ifndef LOGRO_CONTROLLER_H
#define LOGRO_CONTROLLER_H

#include <pistache/http.h>
#include <pistache/router.h>

class LogroController {
public:
    void setupRoutes(Pistache::Rest::Router& router);
};

#endif