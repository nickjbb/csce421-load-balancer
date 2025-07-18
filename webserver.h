/**
 * @file webserver.h
 * @brief Defines the Webserver class that handles processing of individual requests.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include "requests.h"

/**
 * @class Webserver
 * @brief Represents a single web server capable of processing network requests.
 *
 * Each server keeps track of a current request, its name, and when the request processing began.
 */
class Webserver {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes an unnamed web server with no active request.
     */
    Webserver() {
        name = "";
        start_time = 0;
        curr_request = request{"", "", 0};
        idle = true;
    }

    /**
     * @brief Constructs a Webserver with a specific name.
     * @param server_name Name to assign to this server.
     */
    Webserver(std::string server_name) {
        name = server_name;
        start_time = 0;
        curr_request = request{"", "", 0};
        idle = true;
    }

    /**
     * @brief Assigns a request to the server and sets the request's start time.
     * @param r The request to be processed.
     * @param curr_time The current simulation time.
     */
    void add_request(request r, int curr_time) {
        curr_request = r;
        start_time = curr_time;
    }

    /**
     * @brief Retrieves the current request being processed by the server.
     * @return The active request.
     */
    request get_curr_request() {
        return curr_request;
    }

    /**
     * @brief Retrieves the server's name.
     * @return The name of the server.
     */
    std::string get_name() {
        return name;
    }

    /**
     * @brief Retrieves the time when the current request started processing.
     * @return Start time of the current request.
     */
    int get_start_time() {
        return start_time;
    }

    /**
     * @brief Assigns a new name to the server.
     * @param new_name The new name to assign.
     */
    void set_name(std::string new_name) {
        name = new_name;
    }

    /**
     * @brief Determines whether the server has completed its current request.
     * @param curr_time The current simulation time.
     * @return True if the request has completed, false otherwise.
     */
    bool is_request_done(int curr_time) {
        return curr_time > (start_time + curr_request.time_processed);
    }

    /**
     * @brief Returns whether the server is currently idle.
     * @return true if the server is idle.
     */
    bool is_idle(){
        return idle;
    }

    /**
     * @brief Sets the idle status of the server.
     * @param i Whether the server should be marked as idle.
     */
    void set_idle(bool i){
        idle = i;
    }

private:
    request curr_request;   ///< The current request being processed.
    std::string name;       ///< Name of the web server.
    int start_time;         ///< Time when the current request began processing.
    bool idle;              ///< Whether the server is idle or busy.
};

#endif