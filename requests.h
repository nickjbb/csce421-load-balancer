/**
 * @file requests.h
 * @brief Defines the request struct used to represent incoming network requests.
 */

#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>

/**
 * @struct request
 * @brief Represents a network request with input/output IPs and required processing time.
 *
 * A request includes:
 * - the source IP (`ip_in`)
 * - the destination IP (`ip_out`)
 * - the number of time units required to process (`time_processed`)
 */
struct request {
    std::string ip_in;        ///< Source IP address of the request.
    std::string ip_out;       ///< Destination IP address of the request.
    int time_processed;       ///< Time required to process the request.
};

#endif