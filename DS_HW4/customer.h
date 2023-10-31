#ifndef _customer_h_
#define _customer_h_
#include <iostream>
#include <list>
#include <string>

class Customer{
    public:
        //constructors
        Customer();
        Customer(std::string x, std::string n);
        //getters
        std::string get_ID() const;
        std::string get_name() const;

    private:
        std::string name;
        std::string ID;
};

#endif