/**
 * @file load_balancer.h
 * @brief Defines the Loadbalancer class for distributing requests to web servers.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "requests.h"
#include "webserver.h"
#include <iostream>
#include <vector>
#include <queue>

/**
 * @class Loadbalancer
 * @brief Simulates a load balancer that assigns incoming requests to web servers and dynamically manages servers.
 */
class Loadbalancer {
public:
    /**
     * @brief Constructs a Loadbalancer with a given number of servers.
     * @param num_servers The number of web servers to initialize.
     */
    Loadbalancer(int num_servers) {
        sys_time = 0;
        for (int i = 0; i < num_servers; ++i) {
            Webserver w("Server_" + std::to_string(i));
            servers.push_back(w);
        }
        deleted_requests = 0;
    }

    /**
     * @brief Returns the current simulation time.
     * @return int Current system time.
     */
    int get_time() {
        return sys_time;
    }

    /**
     * @brief Adds a new request to the request queue.
     * @param r The request to add.
     */
    void add_request(request r) {
        request_queue.push(r);
    }

    /**
     * @brief Checks whether the request queue is empty.
     * @return true If the queue is empty.
     * @return false If there are pending requests.
     */
    bool is_queue_empty() {
        return request_queue.empty();
    }

    /**
     * @brief Increments the simulation time by 1.
     */
    void addTime() {
        sys_time++;
    }

    /**
     * @brief Returns the size of the request queue.
     * @return Size of the request queue.
     */
    int get_queue_size(){
        return request_queue.size();
    }

    /**
     * @brief Retrieves and removes the next request from the queue.
     * @return request The next request.
     * @note Advances time by one unit.
     */
    request get_request() {
        addTime();
        if (!is_queue_empty()) {
            request r = request_queue.front();
            request_queue.pop();
            return r;
        }
    }

    /**
     * @brief Returns the number of requests deleted due to firewall filtering.
     * @return int Number of deleted requests.
     */
    int get_deleted_requests(){
        return deleted_requests;
    }

    /**
     * @brief Returns the current number of servers.
     * @return int Number of servers.
     */
    int server_size(){
        return servers.size();
    }

    /**
     * @brief Dynamically adds a new web server to the pool.
     */
    void add_server(){
        Webserver s("Server_" + std::to_string(servers.size()));
        servers.push_back(s);
    }

    /**
     * @brief Applies a firewall rule that deletes requests from restricted IP ranges.
     * @param server Reference to the web server whose current request is to be filtered.
     */
    void firewall(Webserver& server){
        if (server.get_curr_request().ip_in == ""){
            return;
        }

        std::string ip_in = server.get_curr_request().ip_in.substr(8); // Skip "192.168."
        std::string ip_out = server.get_curr_request().ip_out.substr(5); // Skip "10.0."

        int third_num_in_pos = ip_in.find('.');
        std::string third_num_in = ip_in.substr(0, third_num_in_pos);
        int third_num = std::stoi(third_num_in);

        int third_num_out_pos = ip_out.find('.');
        std::string third_num_out = ip_out.substr(0, third_num_out_pos);
        third_num = std::stoi(third_num_out);

        if (third_num >= 200){
            std::cout << server.get_name() << " deleted request from " << server.get_curr_request().ip_in << " to " << server.get_curr_request().ip_out << " at time " << sys_time << std::endl;
            server.add_request(request{"", "", 0}, 0);
            server.set_idle(true);
            ++deleted_requests;
        }
    }

    /**
     * @brief Simulates one time step of request handling across all web servers.
     * 
     * - Idle servers take new requests from the queue.
     * - Servers completing their work return to idle state.
     * - Idle servers with no requests may be removed.
     * - A new server may be added if the load is high.
     * - Occasionally generates a new random request.
     * 
     * @param num_servers The maximum number of servers to maintain.
     */
    void simulate_requests(int num_servers) {
        request empty_request = {"", "", 0};
        std::vector<int> servers_to_remove;

        for (size_t i = 0; i < servers.size(); ++i) {
            Webserver& server = servers[i];

            firewall(server);

            if (server.get_curr_request().ip_in == "" && !is_queue_empty()) {
                request r = request_queue.front();
                request_queue.pop();
                std::cout << server.get_name() << " processed request from " << r.ip_in << " to " << r.ip_out << " at time " << sys_time << std::endl;
                server.add_request(r, sys_time);
                server.set_idle(false);
            }

            if (server.is_request_done(sys_time)) {
                if (server.get_curr_request().ip_in != ""){
                    std::cout << server.get_name() << " finished request from " << server.get_curr_request().ip_in << " to " << server.get_curr_request().ip_out << " at time " << sys_time << std::endl;
                    server.add_request(empty_request, sys_time);
                    server.set_idle(true);
                }
                if (!is_queue_empty()) {
                    request r = request_queue.front();
                    request_queue.pop();
                    std::cout << server.get_name() << " processed request from " << r.ip_in << " to " << r.ip_out << " at time " << sys_time << std::endl;
                    server.add_request(r, sys_time);
                    server.set_idle(false);
                }
            }

            if (server.is_idle() && is_queue_empty()){
                servers_to_remove.push_back(i);
            }
        }

        for (auto it = servers_to_remove.rbegin(); it != servers_to_remove.rend(); ++it){
            std::cout << servers.at(*it).get_name() << " is idle. Deleting..." << std::endl;
            servers.erase(servers.begin() + *it);
        }

        if (servers.size() < num_servers && !is_queue_empty()){
            add_server();
        }

        if (rand() % 5 == 0) {
            request new_request = generate_request();
            request_queue.push(new_request);
        }

        sys_time++;
    }

private:
    int sys_time;  ///< Current simulation time.
    std::vector<Webserver> servers;  ///< Pool of active webservers.
    std::queue<request> request_queue;  ///< Queue of pending requests.
    int deleted_requests; ///< Count of filtered/dropped requests by firewall.

    /**
     * @brief Generates a new request with random IPs and a random time duration.
     * @return request The generated request.
     */
    request generate_request() {
        request r;
        r.ip_in = "192.168." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
        r.ip_out = "10.0." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
        r.time_processed = (rand() % 10) + 1;
        return r;
    }
};

#endif