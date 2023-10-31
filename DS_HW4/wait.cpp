#include "wait.h"

//constructor

Wait::Wait(std::string x, std::string y, std::string t, unsigned q, std::string a, std::string n){
    item = x; //item ID
    client = y; //client ID
    timestamp = t; //time requested
    quantity = q; //quantity requested
    action = a; //rent or return
    name = n; //name of client
}

//getters

std::string Wait::get_client() const {
    return client;
}

std::string Wait::get_item() const {
    return item;
}

std::string Wait::get_timestamp() const {
    return timestamp;
}

unsigned Wait::get_quantity() const {
    return quantity;
}

std::string Wait::get_action() const{
    return action;
}

std::string Wait::get_name() const{
    return name;
}