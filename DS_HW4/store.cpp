#include <iostream>
#include <list>
#include <string>
#include <iterator>
#include <fstream>
#include "customer.h"
#include "inventory.h"
#include "wait.h"

std::list<Customer> sort_customers(std::list<Customer>&customers){
    std::list<Customer>::iterator y;
    std::list<Customer>::iterator x;
    std::list<Customer> final;
    bool inserted;

    for(y=customers.begin();y!= customers.end();y++){
        if (final.empty()){
            final.push_back(*y);
        }/*If the list is empty, throw customer in.*/

        else{
            inserted = false;
            for(x=final.begin();x!=final.end();x++){
                if ((*y).get_ID() < (*x).get_ID()){
                    final.insert(x,(*y));
                    inserted = true;
                    break;
                }/*inserts the objects in the right location based on x iterator*/
            }

            if (!inserted) final.push_back(*y);
        }/*If final is not empty, find the correct placement for the
        customer ID using insert. Break in order to avoid duplicates.*/
        
    }
    return final;
}/*sort customers based on their customer ID's, smallest first.*/

std::list<Inventory> sort_inventory(std::list<Inventory>&inventory){
    std::list<Inventory>::iterator y;
    std::list<Inventory>::iterator x;
    std::list<Inventory> final;

    for(y=inventory.begin();y!= inventory.end();y++){
        if (final.empty()){
            final.push_back(*y);
        }/*If the list is empty, throw item in.*/

        else{
            bool inserted = false;
            for(x=final.begin();x!=final.end();x++){
                if ((*y).get_ID() < (*x).get_ID()){
                    final.insert(x,(*y));
                    inserted = true;
                    break;
                }/*sorts by lesser ID. Uses insert to find correct placement.*/
            }

            if (!inserted){
                final.push_back(*(y));
            }/*pushes to the back when it is greater than all elements.*/

        }/*If final is not empty, find the correct placement for the
        item ID using insert. Break in order to avoid duplicates.*/
        
    }
    return final;
}/*sort inventory items based on their ID's, smallest first.*/

std::list<Wait> sort_wait(std::list<Wait>&wait){
    std::list<Wait>::iterator y;
    std::list<Wait>::iterator x;
    std::list<Wait> final;

    for(y=wait.begin();y!= wait.end();y++){
        if (final.empty()){
            final.push_back(*y);
        }/*If the list is empty, throw wait object in.*/

        else{
            bool inserted = false;
            for(x=final.begin();x!=final.end();x++){
                if ((*y).get_client() < (*x).get_client()){
                    final.insert(x,(*y));
                    inserted = true;
                    break;
                }/*sorts by lesser ID. Uses insert to find correct placement.*/
            }

            if (!inserted){
                final.push_back(*(y));
            }/*pushes to the back when it is greater than all elements.*/

        }/*If final is not empty, find the correct placement for the
        wait object using insert. Break in order to avoid duplicates.*/
        
    }
    return final;
}/*sort wait objects based on their customer ID's, smallest first.*/

void parse_inventory(std::ifstream& inventory_input,std::list<Inventory>& inventory){
    std::string temp; //used to store inventory ID
    unsigned int q; //used to store the quantity
    std::string n; //used to store the name

    while (inventory_input){
        bool failed = false; //used to check if items are valid entries

        inventory_input >> temp;
        inventory_input >> q;
        inventory_input >> n;

        if (inventory_input.eof()) break;
        else if (failed) std::cerr << "Invalid inventory ID " + temp + " found in the inventory file." << std::endl;
        else if (temp[0]!='T') {
            std::cerr << "Invalid inventory ID " + temp + " found in the inventory file." << std::endl;
            failed = true;
        }
        else if (q==0) {
            failed = true;
            std::cerr << "Invalid inventory ID " + temp + " found in the inventory file." << std::endl;
        }
        /*if the first letter was not T, the quantity of item was 0, or the 4 numbers are not valid, print to cerr.*/

        if (!failed) inventory.push_back(Inventory(temp,n,q));
        /*No need to check for repeat tools. Just pushback every valid tool in the list.*/
    }
}/*parses through the entire customer file and pushbacks valid inventory items*/

void parse_customer(std::ifstream& customer_input,std::list<Customer>& customers,
std::list<Wait>& request,std::list<Inventory>&inventory){

    std::string temp; //stores customer ID
    std::string action; //stores rent or return
    std::string timestamp; //stores timestamp
    unsigned int q; //stores quantity rented or returned
    std::string item_ID; //stores item ID
    std::string name; //stores name of customer

    while (customer_input){

        bool exist = false; 
        //used to check if a customer already exists in the customer list

        bool failed = false; 
        /*used to check valididty of customer ID.
        Also used to check validity of customer's request.*/

        bool in_inv = false; 
        //used to find if customer's requested item is valid

        customer_input >> temp >> action >> timestamp;
        customer_input >> q >> item_ID >> name;

        std::list<Inventory>::iterator i;
        for (i=inventory.begin();i!=inventory.end();i++){
            if ((*i).get_ID()==item_ID){
                in_inv=true;
                break;
            }
        }//checks if item is in in the inventory

        std::list<Wait>::iterator w;
        bool rent = false;
        for (w=request.begin();w!=request.end();w++){
            if (item_ID == (*w).get_item()){
                if (temp == (*w).get_client()){
                    if ((*w).get_action()=="rent"){
                        rent = true;
                        break;
                    }
                }
            }
        }/*checks if client previously rented the item*/

        if (customer_input.eof()) break;

        else if (!in_inv) {
            std::cerr << "Customer " << temp << " attempted to return item " << item_ID 
            << " which is not in the inventory." << std::endl;
            failed = true;
        }/*if not in the inventory, print out to cerr*/

        else if (temp[0]!='C') {
            std::cerr << "Invalid customer information found for ID " 
            + temp + " in the customer file." << std::endl;
            failed = true;
        }
        //if the first letter fails the C test, then write to cerr.
        
        else if (!rent && action=="return"){
            std::cerr << "Customer " << temp << " attempted to return item " 
            << item_ID << " which she/he did not rent." << std::endl;
            failed = true;
        }/*if person is returning but did not rent, they will be cerr'd.*/

        else{
            std::list<Customer>::iterator x;
            x=customers.begin();
            if (x==customers.end()) customers.push_back(Customer(temp,name));
            for (x=customers.begin();x!=customers.end();++x){
                if (temp==(*x).get_ID()) exist = true;
            }
            if (!exist) customers.push_back(Customer(temp,name));
            /*iterate through the customer list. Check if the customer already exists.
            If they already exist, don't add them, but if they don't, do.*/
        }

        if (!failed) request.push_back(Wait(item_ID,temp,timestamp,q,action,name));
        /*Add the requests into a request list to be sorted and parsed. Only adds
        them if they did not cause an error.*/
    }
}

void return_protocol(bool exist, std::list<Inventory>&inventory,const std::string& item_ID,
std::list<Wait>::iterator &x, const unsigned request_quantity, const std::string &customer_ID,
std::list<Wait>&requestlist,std::list<Wait>&waitlist,std::list<Customer>&customers,
std::list<Wait>&rentlist,std::string& timestamp){

    bool rented = false;//used to check if item was previously rented
    std::list<Inventory>::iterator y;

    for (y=inventory.begin();y!=inventory.end();++y){
        if (item_ID==((*y).get_ID())) {
            exist = true;
            break;
        }
    }
    //uses exist to check if item exists in the inventory
    
    std::list<Wait>::iterator w;
    for (w=rentlist.begin();w!=rentlist.end();w++){
        if ((*w).get_client()==customer_ID){
            if ((*w).get_item()==item_ID){
                rented = true;
                break;
            }
        }
    }/*check if the client rented the item in the past*/

    if (exist && rented){
        std::list<Inventory>::iterator stored;
        for (y=inventory.begin();y!=inventory.end();++y){
            if (item_ID==((*y).get_ID())){
                (*y).set_quantity((*y).get_quantity()+request_quantity);
                stored = y;
                break;
            }
        }/*if the item is in the inventory and it was rented, store the
        correct item object into stored. Also adjusts the available
        quantity based on the quantity returned.*/

        std::list<Wait>::iterator temp;
        bool next;
        temp=waitlist.begin();

        while (temp!=waitlist.end()){
            next = true;
            if ((*temp).get_item()==item_ID){
                if ((*stored).get_quantity()>=(*temp).get_quantity()){
                    (*stored).set_quantity((*stored).get_quantity()-(*temp).get_quantity());
                    rentlist.push_back(*temp);
                    temp = waitlist.erase(temp);
                    next = false;
                }/*If a request in the waitlist is found, fulfill it if possible. If
                request is fulfilled, remove the waitlist request. 
                Add it into the rent list.*/
            }
            if (next) ++temp; //used to prevent skipping iterators.
        }/*If the item is detected in the inventory, add the quantity returned.
    Check to see if any waitlists can be fulfilled.*/
        
        temp = rentlist.begin();
        while (temp!=rentlist.end()){
            next = true;
            if ((*temp).get_client()==customer_ID){
                if ((*temp).get_item()==item_ID){
                    if ((*temp).get_quantity()<=request_quantity){
                        temp = rentlist.erase(temp);
                        next = false;
                    }
                }/*Checks if they are same person returning the same item,
                and then removes the rent request since the person 
                returned their requested quantity.*/
            }
            if (next) temp++;//used to prevent skipping iterators
        }
    }
}

void fullfill(std::list<Wait>&requestlist, std::list<Inventory>&inventory, std::list<Customer>&customers, 
std::list<Wait>&waitlist,std::list<Wait>&rentlist){

    std::list<Wait>::iterator x;//used to iterate through requestlist
    std::string action;
    unsigned int request_quantity;
    std::string item_ID;
    std::string customer_ID;
    std::string timestamp;
    std::string name;
    std::list<Inventory>::iterator item;//used to keep track of correct item
    std::list<Customer>::iterator customer;//used to keep track of correct customer
    std::list<Inventory>::iterator y;//used to iterate through inventory
    std::list<Customer>::iterator z;//used to iterate through customers
    bool exist; //used to verify customer's returned item's existence

    for (x=requestlist.begin();x!=requestlist.end();++x){
        
        exist = false; // determines if a item exists in the inventory
        request_quantity = (*x).get_quantity(); //how much is rented / returned
        action = (*x).get_action(); //return or rent?
        item_ID = (*x).get_item(); //ID of item that starts with T
        customer_ID = (*x).get_client(); //ID of customer that starts with C
        timestamp = (*x).get_timestamp();//time of request
        name = (*x).get_name();//get client's name

        for (y=inventory.begin();y!=inventory.end();++y){
            if ((*y).get_ID()==item_ID) {
                item = y;
                break;
            }
        }/*Matches the inventory item from the request item ID. Now we have the
        correct inventory item.*/

        for (z=customers.begin();z!=customers.end();++z){
            if ((*z).get_ID()==customer_ID) {
                customer = z;
                break;
            }
        }/*Matches the customer ID from the request client ID. Now we have the
        correct client.*/

        if (action=="return"){
            return_protocol(exist, inventory, item_ID, x, request_quantity, 
            customer_ID,requestlist,waitlist,customers,rentlist,timestamp);
            /*returns the item and adds to the quantity available if it exists. Check the if the waitlist can be
            fulfilled. If item doesn't exist, reject it.*/
        }

        else if (action=="rent"){
            if ((*item).get_quantity()>=request_quantity){
                (*item).set_quantity((*item).get_quantity()-request_quantity);
                rentlist.push_back(Wait(item_ID,customer_ID,timestamp,request_quantity,action,name));
                /*if able to fulfill the request, do so. Throw request into rent list.*/
            }
            else{
                waitlist.push_back(Wait(item_ID,customer_ID,timestamp,request_quantity,action,name));
            }/*If not able to fulfill the request, throw their request in to the waitlist.*/

        }
    }
}

void write_customer(std::list<Customer>& customers, std::list<Wait>& waitlist, 
std::list<Wait>& rentlist,std::ofstream& output){

    std::list<Customer>::iterator x;
    for (x=customers.begin();x!=customers.end();x++){
        output << (*x).get_ID() << " " << (*x).get_name() << std::endl;
        std::list<Wait>::iterator w;
        bool print = true;
        /*prints ID and name first*/

        for (w=rentlist.begin();w!=rentlist.end();w++){
            if ((*w).get_client()==(*x).get_ID()){
                if (print) {
                    output << "Rentals: ";
                    print = false;    
                }
                output << (*w).get_item() << " ("<<(*w).get_quantity()<<") ";
            }
        }/*uses print to make sure "Rentals: " only prints once. Print out
        subsequent item ID and quantity. Only prints when there are rentals.*/

        print = true;//reuse for same purpose, but for "Pending: ".

        for (w=waitlist.begin();w!=waitlist.end();w++){
            if ((*w).get_client()==(*x).get_ID()){
                if (print) {
                    output << "Pending: ";
                    print = false;    
                }
                output << (*w).get_item() << " ("<<(*w).get_quantity()<<") ";
            }/*prints out "Pending: " once. Prints out subsequent item ID and
            quantity. Only prints when there are pending requests.*/
        }
        output << std::endl << std::endl;
    }
    output.close();
}/*for each item in customers list, print out their rentals and pending orders.*/

void write_inventory(std::list<Inventory>& inventory, std::list<Wait>& waitlist, 
std::list<Wait>& rentlist,std::ofstream& output){
    std::list<Inventory>::iterator x;
    
    for (x=inventory.begin();x!=inventory.end();x++){
        std::list<Wait>::iterator w;
        bool print = true;

        output << (*x).get_ID() << " " << (*x).get_quantity() << " available ";
        output << (*x).get_name() << std::endl;
        //outputs the item ID, the quantity, and name.

        for (w=rentlist.begin();w!=rentlist.end();w++){
            if ((*w).get_item()==(*x).get_ID()){
                if (print){
                    print = false;
                    output << "Rental Customers: ";
                }
                output << (*w).get_client() << " " << (*w).get_name();
                output << " (" << (*w).get_quantity() << ") "; 
            }/*uses print to print Rental Customers once. Prints out subsequent
            client ID's, client names, and quantities. Only print Rental Customers
            if there are existing rental customers.*/
        }

        if (!print) output << std::endl;
        print = true;

        for (w=waitlist.begin();w!=waitlist.end();w++){
            if ((*w).get_item()==(*x).get_ID()){
                if (print){
                    print = false;
                    output << "Pending Customers: ";
                }
                output << (*w).get_client() << " " << (*w).get_name();
                output << " (" << (*w).get_quantity() << ") "; 
            }
        }/*prints out pending customers once. Prints out subsequent client ID's, 
        client names, and pending quantities. Only prints if there are existing
        pending orders.*/

        output << std::endl;
        if (!print) output << std::endl;
    }
}/*for each item in inventory, print out rentals and pending rentals, the quantity, 
as well as renters and their information.*/

void erase_customer(std::list<Customer>& customers,std::list<Wait>& waitlist,std::list<Wait>& rentlist){
    bool exist = false;
    bool next = true;

    std::list<Customer>::iterator x=customers.begin();
    while (x!=customers.end()){
        next = true;
        exist = false;
        std::list<Wait>::iterator w;

        for (w=waitlist.begin();w!=waitlist.end();w++){
            
            if ((*w).get_client()==(*x).get_ID()){ 
                exist = true;
                break;
            }
        }/*searches the wait list for client ID.*/

        for (w=rentlist.begin();w!=rentlist.end();w++){
            if ((*w).get_client()==(*x).get_ID()){
                 exist = true;
                 break;
                }
        }/*searches the rent list for client ID.*/

        if (!exist){
            next=false;
            x=customers.erase(x);
            /*if client ID is not found in either rentlist or
            waitlist, remove them from the customers list.*/
        }
        if (next) x++; //next prevents skipping of iterators
    }
}/*Verifies for every single customer that they are still active.*/


//main starts here


int main(int argc, char * argv[]){
    if (argc != 5){
        std::cerr << "Incorrect amount of arguments." << std::endl;
        return 1;
    }

    std::ifstream inventory_input(argv[1]);
    if (!inventory_input){
        std::cerr << "File does not exist for reading." << std::endl;
        return 1;
    }

    std::ifstream customer_input(argv[2]);
    if (!customer_input){
        std::cerr << "File does not exist for reading." << std::endl;
        return 1;
    }

    std:: ofstream inventory_output(argv[3]);
    if (!inventory_output){
        std::cerr << "File does not exist for writing." <<std::endl;
        return 1;
    }

    std:: ofstream customer_output(argv[4]);
    if (!inventory_output){
        std::cerr << "File does not exist for writing." <<std::endl;
        return 1;
    } /*standard error checking.*/

    std::list<Inventory> inventory; //contains all tools and items
    std::list<Customer> customers; //contains the unique customers
    std::list<Wait> requestlist; //contains the requests to be fulfilled
    std::list<Wait> waitlist; //contains the requests that couldn't be fulfilled
    std::list<Wait> rentlist; //list of current rentals

    parse_inventory(inventory_input,inventory);
    /*takes in each tool or item with name, ID, and quantity.
    Puts them into a list of tool objects.*/

    parse_customer(customer_input,customers,requestlist,inventory);
    /*Takes in each request and throws them into requestlist.
    Creates a list of unique customers by ID.*/

    std::list<Customer>sorted_customers = sort_customers(customers);
    std::list<Inventory>sorted_inventory = sort_inventory(inventory);
    /*Sort inventory and customers based on ID's, smallest first.*/

    fullfill(requestlist, sorted_inventory, sorted_customers, waitlist,rentlist);
    /*goes through the request list and accepts returned items or rents out items.
    Customers have their requests get put on the wait list if it cannot be fulfilled.
    The wait list is checked in time stamp order after a return as well.*/

    std::list<Wait>sorted_rentlist = sort_wait(rentlist);
    std::list<Wait>sorted_waitlist = sort_wait(waitlist);
    /*Sort rentlist and waitlist based on customer ID*/

    erase_customer(sorted_customers,waitlist,rentlist);
    /*erases all non active customers*/

    write_customer(sorted_customers, sorted_waitlist, sorted_rentlist,customer_output);
    /*outputs to customer information to customer output file*/

    write_inventory(sorted_inventory, sorted_waitlist, sorted_rentlist,inventory_output);
    /*outputs inventory information to inventory output file*/
}