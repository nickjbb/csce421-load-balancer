
#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>

struct request {
    std::string ip_in;
    std::string ip_out;
    int time_processed;
};

#endif