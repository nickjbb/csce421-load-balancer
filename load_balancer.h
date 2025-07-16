
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "requests.h"
#include "webserver.h"
#include <iostream>
#include <vector>
#include <queue>

class Loadbalancer{
    public:
        Loadbalancer(int num_servers){
            sys_time = 0;
            for (int i = 0; i < num_servers; ++i){
                Webserver w("Server_" + std::to_string(i));
                servers.push_back(w);
            }
        }

        int get_time(){
            return sys_time;
        }
        void add_request(request r){
            request_queue.push(r);
            addTime();
        }
        bool is_queue_empty(){
            return request_queue.empty();
        }
        void addTime(){
            sys_time++;
        }

        request get_request(){
            addTime();
            if (!is_queue_empty()){
                request r = request_queue.front();
                request_queue.pop();
                return r;
            }
        }

        void simulate_requests(){

            for (auto& server: servers) {
                if (server.is_request_done(sys_time)){
                    if (!is_queue_empty()){
                        request r = request_queue.front();
                        request_queue.pop();
                        std::cout << server.get_name() << " processed request from " << r.ip_in << " to " << r.ip_out << " at time " << sys_time << std::endl;
                        server.add_request(r, sys_time);
                    }
                }
            }

            if (rand() % 3 == 0){
                request new_request = generate_request();
                request_queue.push(new_request);
            }

            sys_time++;
        }

    private:
        int sys_time;
        std::vector<Webserver> servers;
        std::queue<request> request_queue;

        request generate_request(){
            request r;
            r.ip_in = "192.168." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
            r.ip_out = "10.0." + std::to_string(rand() % 256) + "." + std::to_string(rand() % 256);
            r.time_processed = (rand() % 10) + 1;
            return r;
        }
};


#endif