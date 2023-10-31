#include "word.h"

Word::Word(std::string x, std::string w){
    type = x;
    word = w;
}

std::string Word::get_type() const{
    return type;
}

std::string Word::get_word() const{
    return word;
}