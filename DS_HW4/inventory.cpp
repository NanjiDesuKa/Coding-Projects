#include "inventory.h"

//constructor
Inventory::Inventory(std::string x, std::string n, unsigned q){
    ID = x;
    name = n;
    quantity = q;
}

//getters
 std::string Inventory::get_ID() const {
    return ID;
}

unsigned Inventory::get_quantity() const {
    return quantity;
}

std::string Inventory::get_name() const {
    return name;
}

//setters
void Inventory::set_quantity(unsigned x) {
    quantity = x;
}