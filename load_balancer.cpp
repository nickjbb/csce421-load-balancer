/**
 * @file load_balancer.cpp
 * @brief Main driver program for simulating a load balancer with multiple web servers.
 *
 * This file defines the main execution logic of a load balancer simulation. It initializes a set of servers,
 * generates an initial queue of requests, and runs the simulation for a user-specified number of clock cycles.
 * It also logs queue sizes, server statistics, and dropped requests.
 */

#include "load_balancer.h"
#include <cstdlib>
#include <iostream>

/**
 * @brief Generate a random request with randomized input/output IPs and processing time.
 * 
 * @return request A newly constructed request with pseudo-random properties.
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
 * Initializes the load balancer with a specified number of servers and runs the simulation
 * for the specified duration in clock cycles. Prints logs including queue sizes and dropped requests.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Exit code: 0 for success, 1 for incorrect usage.
 *
 * @usage
 * ./loadbalancer <num_servers> <run_time>
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
    std::cout << "Task time range: 0 to 10" << std::endl;
    std::cout << "Clock cycles: " << run_time << std::endl;
    std::cout << "Starting with " << num_servers << " servers..." << std::endl;

    // Run the simulation for the specified time
    while (lb.get_time() < run_time) {
        lb.simulate_requests(num_servers);
    }

    std::cout << "Simulation completed at time: " << lb.get_time() << std::endl;
    std::cout << "Ending queue size is: " << lb.get_queue_size() << std::endl;
    std::cout << "Ending number of servers is: " << lb.server_size() << std::endl;
    std::cout << "Number of deleted requests: " << lb.get_deleted_requests() << std::endl;

    return 0;
}
