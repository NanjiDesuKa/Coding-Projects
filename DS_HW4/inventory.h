#ifndef _inventory_h_
#define _inventory_h_
#include <iostream>
#include <string>

class Inventory{
    public:
        //constructor
        Inventory(std::string x, std::string n, unsigned q);

        //getters
        std::string get_ID() const;
        unsigned get_quantity() const;
        std::string get_name() const;

        //setters
        void set_quantity(unsigned x);

    private:
        std::string ID;
        unsigned quantity;
        std::string name;
};

#endif