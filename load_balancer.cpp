/**
 * @file load_balancer.cpp
 * @brief Main driver program for simulating a load balancer.
 */

#include "load_balancer.h"
#include <cstdlib>
#include <iostream>

/**
 * @brief Generate a random request with random IPs and processing time.
 * 
 * @return request A new request with randomized input/output IP and processing time.
 */
request generate_request() {
    request r;
    r.ip_in = "192.168." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
    r.ip_out = "10.0." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
    r.time_processed = (rand() % 10) + 1;
    return r;
}

/**
 * @brief Entry point of the load balancer simulation program.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Exit status.
 * 
 * Usage:
 * @code
 * ./loadbalancer <num_servers> <run_time>
 * @endcode
 */
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./loadbalancer <num_servers> <run_time>\n";
        return 1;
    }

    int num_servers = std::atoi(argv[1]);
    int run_time = std::atoi(argv[2]);

    Loadbalancer lb(num_servers);

    // Fill the queue with an initial load
    for (int i = 0; i < num_servers * 100; ++i) {
        lb.add_request(generate_request());
    }

    std::cout << "Starting queue size is: " << lb.get_queue_size() << std::endl;

    // Run the simulation for the specified time
    while (lb.get_time() < run_time) {
        lb.simulate_requests(num_servers);
    }

    std::cout << "Simulation completed at time: " << lb.get_time() << std::endl;
    std::cout << "Ending queue size is: " << lb.get_queue_size() << std::endl;
    return 0;
}