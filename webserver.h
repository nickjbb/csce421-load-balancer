
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include "requests.h"

class Webserver {

    public:
        Webserver(){
            name = "";
            start_time = 0;
            curr_request = request{"", "", 0};
        };

        Webserver(std::string server_name){
            start_time = 0;
            name = server_name;
        };

        void add_request(request r, int curr_time){
            curr_request = r;
            start_time = curr_time;
        };

        request get_curr_request(){
            return curr_request;
        };

        std::string get_name(){
            return name;
        }

        int get_start_time(){
            return start_time;
        }

        void set_name(std::string new_name){
            name = new_name;
        }
        
        bool is_request_done(int curr_time){
            return curr_time > (start_time + curr_request.time_processed);
        }

    private:
        request curr_request;
        std::string name;
        int start_time;

};

#endif