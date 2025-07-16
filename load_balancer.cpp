
#include "load_balancer.h"
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

request generate_request(){
    request r;
    r.ip_in = "192.168." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
    r.ip_out = "10.0." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
    r.time_processed = (rand() % 10) + 1;
    return r;
}

int main(int argc, char* argv[]){
    if (argc < 3) {
        std::cerr << "Usage: ./loadbalancer <num_servers> <run_time>\n";
        return 1;
    }

    int num_servers = std::atoi(argv[1]);
    int run_time = std::atoi(argv[2]);

    Loadbalancer lb(num_servers);

    for (int i = 0; i < num_servers * 100; ++i){
        lb.add_request(generate_request());
    }

    while (lb.get_time() < run_time){
        lb.simulate_requests();
    }


    std::cout << "Simulation completed at time: " << lb.get_time() << std::endl;
    return 0;
}