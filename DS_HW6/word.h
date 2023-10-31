#ifndef __word_h_
#define __word_h_
#include <string>
#include <iostream>
#include <vector>

class Word{
    public:
        /*constructor*/
        Word();
        Word(std::string x, std::string w);

        /*getters*/
        std::string get_type() const;
        std::string get_word() const;

    private:
        std::string type; //include or exclude word
        std::string word; //the actual word
};

#endif