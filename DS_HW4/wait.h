#ifndef _wait_h_
#define _wait_h_
#include <iostream>
#include <string>

class Wait{
    public:
        //constructor
        Wait(std::string x, std::string y, std::string t, unsigned q, std::string a, std::string n);

        //getters
        std::string get_item() const;
        std::string get_client() const;
        std::string get_timestamp() const;
        unsigned get_quantity() const;
        std::string get_action() const;
        std::string get_name() const;

    private:
        std::string item;
        std::string client;
        std::string timestamp;
        unsigned quantity;
        std::string action;
        std::string name;
};

#endif