#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "word.h"
#include <algorithm>
#include <cmath>

void recursive_finder(std::vector<std::string>& temp,std::vector<std::vector<std::string>>& solutions,
const std::vector<Word>& positive, const std::vector<Word>& negative,unsigned& position,
const std::string& one_or_all, std::ofstream& output); 
/*function declaration*/

bool word_sort(const Word& w1, const Word& w2){
    return (w1.get_word().length()>w2.get_word().length());
    //sort the words by greatest length
}

void parse_file(std::ifstream& puzzle,unsigned& num_col, unsigned& num_row,std::vector<Word>& words){
    
    puzzle >> num_col >> num_row; /*capture first two as the dimensions of the box*/

    std::string temp_type, temp_word;
    while(puzzle){
        puzzle >> temp_type >> temp_word;
        words.push_back(Word(temp_type,temp_word));
        /*save each word and whether it should be in the solution box or not*/
    }
    if (words[words.size()-1].get_word()==words[words.size()-2].get_word()) words.pop_back();
    /*Due to inconsistent file formatting, this will remove the duplicated word at the end if it exists*/
}

std::vector<std::string> create_grid(unsigned num_row, unsigned num_col){
    std::vector<std::string> x;
    for (unsigned i = 0; i<num_row; i++){
        std::string temp = "";
        for (unsigned j = 0; j<num_col; j++){
            temp += "0";
        }
        x.push_back(temp);
    }
    return x;
}/*create a grid that uses the num rows and columns provided of string 0's*/   

void split_words(const std::vector<Word>& words,std::vector<Word>& positive,std::vector<Word>& negative){
    for (unsigned x = 0; x<words.size();x++){
        if (words[x].get_type()=="+") positive.push_back(words[x]);
        if (words[x].get_type()=="-") negative.push_back(words[x]);
    }
}/*throw words into positive or negative based on their type.*/

bool check_direction(int x, int y, const std::vector<std::string>& grid, std::string word, std::string direction){

    int word_size = word.size(); 
    int row_size = grid.size();
    int col_size = grid[0].size();
    //to avoid integer overflow for some of my if statements

    if (x<0 || x>=row_size) return false;
    if (y<0 || y>=col_size) return false;
    /*small fix to bounds issue*/

    if (direction == "R"){
        if (y+word_size<=col_size){
            for (int i=y,j=0;j<word_size;i++,j++){
                if (grid[x][i]!='0' && grid[x][i]!=word[j]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted horizontally to the right*/

    if (direction == "L"){
        if (y-word_size+1>=0){
            for (int i=y,j=0;j<word_size;j++,i--){
                if (grid[x][i]!='0' && grid[x][i]!=word[j]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted horizontally to the left*/

    if (direction == "D"){
        if (x+word_size<=row_size){
            for (int i=x,j=0;j<word_size;i++,j++){
                if (grid[i][y]!='0' && grid[i][y]!=word[j]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted vertically down*/

    if (direction == "U"){
        if (x-word_size+1>=0){
            for (int i=x,j=0;j<word_size;j++,i--){
                if (grid[i][y]!='0' && grid[i][y]!=word[j]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted vertically up*/

    if (direction == "DR"){
        if (x+word_size<=row_size && y+word_size<=col_size){
            for (int i=x,j=y,k=0;k<word_size;k++,i++,j++){
                if (grid[i][j]!='0' && grid[i][j]!=word[k]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted downward right*/

    if (direction == "DL"){
        if (x+word_size<=row_size && y-word_size+1>=0){
            for (int i=x,j=y,k=0;k<word_size;k++,i++,j--){
                if (grid[i][j]!='0' && grid[i][j]!=word[k]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted downward left*/

    if (direction == "UL"){
        if (x-word_size+1>=0 && y-word_size+1>=0){
            for (int i=x,j=y,k=0;k<word_size;k++,i--,j--){
                if (grid[i][j]!='0' && grid[i][j]!=word[k]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted upward left*/

    if (direction == "UR"){
        if (x-word_size+1>=0 && y+word_size<=col_size){
            for (int i=x,j=y,k=0;k<word_size;k++,i--,j++){
                if (grid[i][j]!='0' && grid[i][j]!=word[k]) return false;
            }
            return true;
        }
    }/*check to see if word can be inputted upward right*/

    return false;
}

void place_word(int x,int y, std::vector<std::string>& grid,std::string word, std::string direction){

    int word_size = word.size(); 

    if (direction == "UL"){
        for (int i=x,j=y,k=0;k<word_size;k++,i--,j--){
            grid[i][j]=word[k];
        }
    }/*place word going up and to the left*/

    if (direction == "U"){
        for (int i=x,j=0;j<word_size;j++,i--){
            grid[i][y]=word[j];
        }
    }/*place word going from bottom to up*/

    if (direction == "UR"){
        for (int i=x,j=y,k=0;k<word_size;k++,i--,j++){
            grid[i][j]=word[k];
        }
    }/*place word from bottom to up, and from left to right*/

    if (direction == "L"){
        for (int i=y,j=0;j<word_size;j++,i--){
                grid[x][i]=word[j];
            }
    }/*place word going from right side to left*/

    if (direction == "R"){
        for (int i=y,j=0;j<word_size;j++,i++){
                grid[x][i]=word[j];
            }
    }/*place word normally horizontally*/

    if (direction == "DL"){
        for (int i=x,j=y,k=0;k<word_size;k++,i++,j--){
            grid[i][j]=word[k];
        }
    }/*place word going from up to down, right to left*/

    if (direction == "D"){
        for (int i=x,j=0;j<word_size;i++,j++){
            grid[i][y]=word[j];
        }
    }/*place word vertically from up to down*/

    if (direction == "DR"){
        for (int i=x,j=y,k=0;k<word_size;k++,i++,j++){
            grid[i][j]=word[k];
        }
    }/*place word from up to down, from left to right*/
}

void print_grid(const std::vector<std::string>& grid){
    for (unsigned x = 0; x<grid.size();x++){
        for (unsigned y = 0; y<grid[0].size();y++){
            std::cout << grid[x][y];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}/*print contents of the grid into the terminal. Mostly used for testing purposes.*/

void output_grid(std::ofstream& output,const std::vector<std::string>& grid){
    output << "Board:" << std::endl;
    for (unsigned x = 0; x<grid.size();x++){
        output << "  ";
        for (unsigned y = 0; y<grid[0].size();y++){
            output << grid[x][y];
        }
        if (x!=grid.size()) output << std::endl;
    }
}/*Modified version of print_grid that prints to an output file. Has correct formatting.*/

bool check_negative(const std::vector<std::string>& grid, const std::vector<Word>& negative){
    std::vector<int> x = {-1,-1,-1,0,0,1,1,1};
    std::vector<int> y = {-1,0,1,-1,1,-1,0,1};
    /*Vectors are used to indicate direction.
    Directions: UL,U,UR,L,R,DL,D,DR*/

    if (grid.size()==0) return false; //if there is no grid, return false;

    int row_size = grid.size();
    int col_size = grid[0].size();
    /*conversion is needed to not compare unsigned ints and ints*/

    for (unsigned n=0;n<negative.size();n++){
        //loop through every single unwanted word

        std::string no = negative[n].get_word();

        int word_size = no.size(); 
        /*used to verify if a negative word was found*/

        for (unsigned i=0;i<grid.size();i++){
            for (unsigned j=0;j<grid[0].size();j++){
            //loop through every index of the grid

                if (grid[i][j]!=no[0]) continue;
                //check to see if first letter at that index matches

                for (unsigned d=0;d<8;d++){

                    int row_direction = i+x[d];
                    int col_direction = j+y[d];
                    /*this will access all 8 directions from the 2 direction vectors.*/

                    int k; 
                    /*must initialize k here in order for it to be used in scope 
                    with the if statement that determines if a word is found*/

                    for (k=1;k<word_size;k++){//start at 1, already checked first letter of word
                        
                        if (row_direction<0 || row_direction>=row_size) break;
                        if (col_direction<0 || col_direction>=col_size) break;
                        /*Avoid out of bounds*/

                        if (grid[row_direction][col_direction]!=no[k]) break;
                        //break if letters no longer match in current direction

                        row_direction+=x[d];
                        col_direction+=y[d];
                        /*keep moving in that direction since letters currently match*/
                    }

                    if (k==word_size) return true;
                    /*If the word was found, value of k is equal to the length of the word.
                    This is because the loop was never interrupted and k reached the end of
                    the loop without ever mismatching a letter. Return true if the word is found.*/
                }
            }
        }
        
    }
    return false; //if the word was never found, return false
}

void duplicate_solution(std::vector<std::string>& grid, const std::string& order){

    /*reflection names are based on reflections on a cartesian plane*/

    if (order=="x-reflect"||order=="xy-reflect"){
        reverse(grid.begin(),grid.end());
    }/*reflect order of strings only*/

    if (order=="y-reflect"||order=="xy-reflect"){
        for (unsigned i=0;i<grid.size();i++){
            reverse(grid[i].begin(),grid[i].end());
        }
    }/*reflect order of each string's characters*/

    /*selecting xy-reflect will use both methods*/
}

void solution_finder(unsigned row, unsigned col, const std::vector<Word>& positive, 
const std::vector<Word>& negative, std::ofstream& output, 
const std::vector<std::string>&grid,std::string one_or_all){

    bool no_solution = false;
    std::string first_word = positive[0].get_word();

    double temp_row = row;
    double temp_col = col;
    /*required in order to obtain the correct values for the ceil*/

    int row_bound = ceil(temp_row/2);
    int col_bound = ceil(temp_col/2);
    /*used to avoid duplicate solutions. Fill in the base word only in the selected quadrant.
    After going through the algorithm, rotate and reflect to obtain the 3 other solutions.*/

    std::vector<std::string> temp;
    unsigned position; //used to recursively go through the positive words vector to place words

    std::vector<std::vector<std::string>> solutions;
    /*vector of solutions to be printed*/

    for (int x=0; x<row_bound;x++){
        for (int y=0;y<col_bound;y++){
            /*go through every index of the upper left quadrant*/

            if (check_direction(x,y,grid,first_word,"UL")){
                temp = grid;
                place_word(x,y,temp,first_word,"UL");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the upper left direction*/

            if (check_direction(x,y,grid,first_word,"U")){
                temp = grid;
                place_word(x,y,temp,first_word,"U");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the up direction*/

            if (check_direction(x,y,grid,first_word,"UR")){
                temp = grid;
                place_word(x,y,temp,first_word,"UR");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the upper right direction*/

            if (check_direction(x,y,grid,first_word,"L")){
                temp = grid;
                place_word(x,y,temp,first_word,"L");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the left direction*/

            if (check_direction(x,y,grid,first_word,"R")){
                temp = grid;
                place_word(x,y,temp,first_word,"R");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the right direction*/

            if (check_direction(x,y,grid,first_word,"DL")){
                temp = grid;
                place_word(x,y,temp,first_word,"DL");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the downward left direction*/

            if (check_direction(x,y,grid,first_word,"D")){
                temp = grid;
                place_word(x,y,temp,first_word,"D");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the downward direction*/

            if (check_direction(x,y,grid,first_word,"DR")){
                temp = grid;
                place_word(x,y,temp,first_word,"DR");
                position = 0;
                recursive_finder(temp,solutions,positive,negative,position,one_or_all,output);
            }/*recursively check placement and attempt to place in the downward right direction*/
        }
    }/*These are the base cases for all the possible ways to place the 
    first word in the first quadrant*/

    if (solutions.size()==0) no_solution = true;
    /*if the solution vector is empty, that means there is no solution*/

    if (no_solution){
        output << "No solution found." << std::endl;
        return;
    }
    else{
        std::vector<std::string> bee;
        unsigned limit = solutions.size();
        for (unsigned i=0;i<limit;i++){
            
            bee = solutions[i];
            duplicate_solution(bee,"x-reflect");
            solutions.push_back(bee);
            /*pushback reflection over x axis*/

            bee = solutions[i];
            duplicate_solution(bee,"y-reflect");
            solutions.push_back(bee);
            /*pushback reflection over y axis*/

            bee = solutions[i];
            duplicate_solution(bee,"xy-reflect");
            solutions.push_back(bee);
            /*pushback reflection over both*/
        }
        
        sort(solutions.begin(),solutions.end());
        solutions.erase(std::unique(solutions.begin(),solutions.end()),solutions.end());
        /*sort solutions so that all duplicates are adjacent. Then use std::unique to erase
        every redundant solution.*/

        output << solutions.size() << " solution(s)" << std::endl;
        for (unsigned i=0;i<solutions.size();i++){
            output_grid(output,solutions[i]);
        }
    }
}

void alphabet_masher(std::vector<std::string>&temp,std::vector<std::vector<std::string>>& solutions,
const std::vector<Word>& negative){

    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    //entirety of the alphabet

    if (check_negative(temp,negative)) return;
    //reject potential solution if negative words are found
    
    bool is_filled = true;
    for (unsigned i = 0; i<temp.size();i++){
        for (unsigned j=0;j<temp[0].size();j++){
            if (temp[i][j]=='0'){
                is_filled = false;
                break;
            }
        }
        if (!is_filled) break;
    }/*checks to see if grid is actually filled now*/

    if (is_filled){
        solutions.push_back(temp);
        return;
    }/*if it is indeed filled and has no negative words,
    push back the grid into the solutions vector*/

    for (unsigned i=0;i<temp.size();i++){
        for (unsigned j=0; j<temp[0].size();j++){
            /*go through the entire grid's indexes*/
            
            if (temp[i][j]=='0'){

                for (unsigned k=0;k<26;k++){
                    /*go through entire alphabet*/

                    temp[i][j] = alphabet[k];
                    /*set the zero into a letter*/
                    
                    alphabet_masher(temp,solutions,negative); 
                    /*call the function recursively until grid meets a base case*/
                }

                temp[i][j] = '0';
                /*reset the space after the recursion is over*/
            }
        }
    }    
}

void recursive_finder(std::vector<std::string>& temp,std::vector<std::vector<std::string>>& solutions,
const std::vector<Word>& positive, const std::vector<Word>& negative,unsigned& position,
const std::string& one_or_all, std::ofstream& output){

    position++; //since position is set to 0, we will always start with the second word in the vector

    if (one_or_all == "one_solution" && solutions.size()==1){
        output_grid(output,solutions[0]);
        exit(0);
    }/*exclusive to one solution. Just ends the program if one solution is present.*/

    if (check_negative(temp,negative)){
        return;
    }/*if negative words are found, dismiss potential solution*/
    
    bool is_filled = true;
    for (unsigned i=0; i<temp.size();i++){
        for (unsigned j=0; j<temp[0].size();j++){
            if (temp[i][j]=='0'){
                is_filled = false;
                break;
            }
            if (!is_filled) break;
        }
    }/*check to see if the grid is filled completely*/

    if (position==positive.size() && is_filled){
        solutions.push_back(temp);
        return;
    }/*if no negative words were found AND all words are used up as well as all space, 
    push back into solution set*/

    if (position==positive.size() && !is_filled){
        alphabet_masher(temp,solutions,negative);
        return;
    }

    if (one_or_all == "one_solution" && solutions.size()==1){
        output_grid(output,solutions[0]);
        exit(0);
    }/*exclusive to one solution. Just ends the program if one solution is present.*/
    
    std::string word = positive[position].get_word();
    std::vector<std::string> recursive_temp;

    for (unsigned i=0;i<temp.size();i++){
        for (unsigned j=0;j<temp[0].size();j++){
        /*loop through every index*/

            unsigned temp_pos;

            if (check_direction(i,j,temp,word,"UL")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"UL");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in up left direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"U")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"U");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word up direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"UR")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"UR");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in up right direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"L")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"L");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in left direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"R")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"R");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in right direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"DL")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"DL");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in down left direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"D")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"D");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in down direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/

            if (check_direction(i,j,temp,word,"DR")){
                recursive_temp = temp;
                temp_pos = position;
                place_word(i,j,recursive_temp,word,"DR");
                recursive_finder(recursive_temp,solutions,positive,negative,temp_pos,one_or_all,output);
            } /*check if able to place word in down right direction, place if possible, then recursively
            call the function again to attempt to place again in other directions until grid is filled
            or you run out of words. Will pushback if solution is valid.*/
        }
    }
}


/*main starts here*/


int main(int argc, char* argv[]){
    if (argc!=4){
        std::cerr << "Incorrect amount of arguments given." <<std::endl;
        return 1;
    }
    std::ifstream puzzle(argv[1]);
    if (!puzzle){
        std::cerr << "File does not exist for reading." << std::endl;
        return 1;
    }
    /*standard error checking.*/
    
    std::ofstream output(argv[2]);

    unsigned num_col,num_row; //bounds of your grid
    std::vector<Word> words; //vector of words to consider (words to include and exclude)

    parse_file(puzzle, num_col, num_row, words);
    //this gets the dimensions of the box, and the words to be added and avoided

    std::vector<std::string> grid = create_grid(num_row, num_col);
    //this will create the necessary grid to store the words

    std::vector<Word> positive;
    std::vector<Word> negative;
    split_words(words,positive,negative);
    /*Put every single word parsed into either vector of words to be added,
    or into the vector of words to be avoided.*/

    sort(positive.begin(),positive.end(),word_sort);
    sort(negative.begin(),negative.end(),word_sort);
    /*sort both positive and negative words for consistency*/

    std::string option = argv[3];

    if (option == "one_solution" || option == "all_solutions"){
        solution_finder(num_row,num_col,positive,negative,output,grid,option);
    }/*one solution or all solutions is considered later*/
    else{
        std::cerr <<"Invalid argument for solution type." <<std::endl;
    }/*avoid incorrect argument*/

    puzzle.close();
    output.close();
}