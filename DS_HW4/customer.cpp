#include "customer.h"

//constructor

Customer::Customer(std::string x, std::string n){
    name = n;
    ID = x;
}

//getters

std::string Customer::get_ID() const{
    return ID;
}

std::string Customer::get_name() const{
    return name;
}