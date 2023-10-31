#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

struct Variables{
    int value;
    char name;
};/*used to keep track of variables*/

int calc_type(char line[]){

    if (strchr(line, '/')!=NULL||strchr(line, '*')!=NULL||strchr(line,'%')!=NULL) return 2; // mult/division/mod
    else if (strchr(line,'+')!=NULL) return 1;  // add

    for (unsigned i = 0; i<strlen(line)-1;i++){
        if (line[i]=='-' && line[i+1]==' ') return 1; //sub
    }

    
    return 3; //assignment operator
    

}/*return different integer values based on operation*/


void print_comment(char line[]){
    printf("# ");
    printf("%s",line);
    if (strchr(line,'\n')==NULL) printf("\n");
}/*print parsed line as a comment in MIPS. Compensate for missing new line for last line of code*/

char* convert_binary(int num){
    
    int x = num;
    int r;
    static char temp[257];
    strcpy(temp,"");

    while (x!=0){
        r = x%2;
        x = x/2;
        if (r!=0) strcat(temp,"1");
        else strcat(temp,"0"); 
    }

    int bin_size = strlen(temp);

    for (int i = 0; i<bin_size/2;i++){
        char y = temp[i];
        temp[i] = temp[bin_size-1-i];
        temp[bin_size-1-i] = y;
    }/*reverse string to get correct binary format*/

    return temp;

}/*return a string that is the int in binary*/

bool check_power2(int num){
    
    int temp; //remainder
    int current = abs(num);

    while (true){
        temp = current%2;
        if (temp == 1 && current!=1) return false;
        current = current/2;
        if (current == 0) return true;
    }
}/*check if int is a pure value of base 2*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////



void execute(int id, struct Variables s[], bool s_used[], bool t_used[],const char line[],int *level){

    if (id == 3){/*if assigning a value to a variable*/

        for (unsigned int i = 0; i < 8; i++){/*iterate through all registers*/

            if (!s_used[i]){/*find next available register*/

                s_used[i] = true; /*register is now set to used*/
                s[i].name = line[0];
                char temp[8];
                sscanf(line,"%s = %d;",temp,&s[i].value);
                printf("li $s%d,%d\n",i,s[i].value);
                break;

            }/*save the variable name and value and print out the load immediate MIPS code*/
        }
    }



    if (id == 1){/*if line contains addition and/or subtraction*/

        char first[257]; //first register
        strcpy(first,"");
        char second[257]; //second register
        strcpy(second,"");

        char function[257]; //add or sub (includes addi and subi)
        char destination[257]; //register where result is stored
        strcpy(destination,"");

        int index = 4; //skip assignment portion

        while (true){

            bool t_free = false;
            for (unsigned i = 0; i<10;i++){
                if (!t_used[i]) t_free = true;   
            }/*check if entire temp register set is used up*/

            if (!t_free){
                for (unsigned i=0;i<10;i++){
                    t_used[i] = false;
                }
            }/*clear temp registers if all are used up*/

            if (strcmp(first,"")!=0&&strcmp(second,"")!=0){ /*if two items are to be computed*/

                bool t_free = false;
                for (unsigned i = 0; i<10;i++){
                    if (!t_used[i]) t_free = true;   
                }/*check if entire temp register set is used up*/

                if (!t_free){
                    for (unsigned i=0;i<10;i++){
                        t_used[i] = false;
                    }
                }/*clear temp registers if all are used up*/

                    if (line[index]==';'||line[index+1]==';'){/*if there are no more items to be computed*/

                        for (int i = 0; i < 8; i++){/*look for next available save register*/

                            if (!s_used[i]){
                                
                                char y[257];
                                sprintf(y,"%d",i); 
                                strcpy(destination,"");
                                strcat(destination,"s");
                                strcat(destination,y);
                                //convert int value to string and set destination to i th save register

                                s_used[i]=true; //set register as being used
                                s[i].name = line[0]; //set variable as register

                                if (isdigit(second[0])&&strcmp(function,"sub")==0){
                                    strcpy(function,"add");
                                    char z[700];
                                    strcpy(z,"-");
                                    strcat(z,second);
                                    strcpy(second,z);
                                }/*special case: if second value is an positive int 
                                and operation is sub,change sub to add and int to negative*/

                                if (second[0]=='-'&&strcmp(function,"sub")==0){
                                    strcpy(function,"add");
                                    char z[700];
                                    strcpy(z,"-");
                                    strcat(z,second);
                                    strcpy(second,z);
                                }/*special case: if sub with negative int, change sub to add and int to positive*/

                                if (second[0]=='-'||isdigit(second[0])) strcat(function,"i");
                                //if int is involved, set add to addi or sub to subi

                                if (isdigit(second[0])||second[0]=='-') printf("%s $%s,$%s,%s\n",function,destination,first,second);
                                else printf("%s $%s,$%s,$%s\n",function,destination,first,second);
                                /*print out operation, destination, and registers / integers*/

                                break;
                            }
                        }
                    }
                    else{/*if not the last item to be computed*/
                        for (int i = 0; i < 10; i++){/*look for next available temp register*/

                            if (!t_used[i]){
                                
                                char y[257];
                                sprintf(y,"%d",i);
                                strcpy(destination,"");
                                strcat(destination,"t");
                                strcat(destination,y);
                                t_used[i]=true;
                                /*change int to string, and set destination to i th t register*/

                                if (isdigit(second[0])&&strcmp(function,"sub")==0){
                                    strcpy(function,"add");
                                    char z[700];
                                    strcpy(z,"-");
                                    strcat(z,second);
                                    strcpy(second,z);
                                }/*special case: if sub with positive int, change sub to add and int to negative*/

                                if (second[0]=='-'&&strcmp(function,"sub")==0){
                                    strcpy(function,"add");
                                    char z[700];
                                    strcpy(z,"-");
                                    strcat(z,second);
                                    strcpy(second,z);
                                }/*special case: if sub with negative int, change sub to add and int to positive*/

                                if (second[0]=='-'||isdigit(second[0])) strcat(function,"i");
                                /*if int value is involved, add i to operation*/

                                if (isdigit(second[0])||second[0]=='-') printf("%s $%s,$%s,%s\n",function,destination,first,second);
                                else printf("%s $%s,$%s,$%s\n",function,destination,first,second);
                                /*print operation, destination, and registers / int values*/

                                bool exist = false;
                                for (unsigned i = 0; i<8;i++){
                                    if (s[i].name == line[0]){
                                        exist = true;
                                        break;
                                    }
                                }/*check if current variable is already saved*/
                                if (!exist){
                                    for (unsigned i = 0; i<8; i++){
                                        if (!s_used[i]){
                                            s[i].name = line[0];
                                            break;
                                        }
                                    }
                                }/*if not saved, save it*/

                                strcpy(first,destination);
                                strcpy(destination,"");
                                strcpy(second,"");
                                /*reset second value and destination. Set previous destination to current first.*/

                                break;
                            }
                        }
                    }
                }

            if (line[index]==' '){
                index++;
                continue;
            }/*skip spaces*/

            if (line[index]==';') break;
            //reached end of line
            
            if (isalpha(line[index])){/*if a variable is detected*/

                for (unsigned int i = 0; i < 8; i++){

                    if (s[i].name == line[index]){/*find matching save register*/

                        char y[2];

                        if (strcmp(first,"")==0){
                            sprintf(y,"%d", i);
                            strcat(first,"s");
                            strcat(first,y);
                        }/*store in first if empty*/

                        else  {
                            sprintf(y,"%d", i);
                            strcat(second,"s");
                            strcat(second,y);
                        }/*else store in second*/

                        break;
                    }
                }

                index++;
                continue;
                
            }

            else if (!isalpha(line[index]) && !isdigit(line[index])){/*if an operator is detected*/
                if (line[index] == '+'){
                    strcpy(function,"add");
                }
                else if (line[index] == '-') strcpy(function,"sub");

                index++;
            }/*set function to add or sub*/

            else if(isdigit(line[index])) {/*if a digit is detected*/
                char y[2];

                while (true){

                    if (!isdigit(line[index])) break;
                    y[0] = line[index];
                    y[1] = '\0';
                    if (strcmp(first,"")==0)strcat(first,y);
                    else strcat(second,y);
                    index++;
                }/*capture entire int and set to first if empty and second if first is occupied*/
            }
        }
    }




    if (id == 2){/*if line contains multiplication or division*/

        bool exist = false;
        for (unsigned i = 0; i<8;i++){
            if (s[i].name==line[0]){
                exist = true;
                break;
            }
        }/*make sure assignment variable exists*/
        if (!exist){
            for (unsigned i = 0; i<8;i++){
                if (!s_used[i]){
                    s[i].name = line[0];
                    break;
                }
            }
        }/*if not, set it to next free register*/

        char first[257]; //first register
        strcpy(first,"");
        char second[257]; //second register
        strcpy(second,"");

        char function[257]; //mult or div
        char destination[257]; //register where result is stored
        strcpy(destination,"");

        int index = 4; //skip assignment portion

        while (true){

            bool t_free = false;
            for (unsigned i = 0; i<10;i++){
                if (!t_used[i]) t_free = true;   
            }/*check if entire temp register set is used up*/

            if (!t_free){
                for (unsigned i=0;i<10;i++){
                    t_used[i] = false;
                }
            }/*clear temp registers if all are used up*/
            
            bool exist = false;

            for (unsigned i = 0; i<8;i++){
                if (s[i].name == line[0]){
                    exist = true;
                    break;
                }
            }/*check if variable is already declared and has a register saved for it.*/
            if (!exist){
                for (unsigned i = 0; i<8;i++){
                    if (!s_used[i]){
                        s[i].name = line[0];
                        break;
                    }
                }
            }/*if no register has been given, set first free register to variable*/

            if (strcmp(first,"")!=0&&strcmp(second,"")!=0){ /*if two items are to be computed*/

                bool t_free = false;
                for (unsigned i = 0; i<10;i++){
                    if (!t_used[i]) t_free = true;   
                }/*check if entire temp register set is used up*/

                if (!t_free){
                    for (unsigned i=0;i<10;i++){
                        t_used[i] = false;
                    }
                }/*clear temp registers if all are used up*/

                if (line[index]==';'||line[index+1]==';'){/*if there are no more items to be computed*/

                    int k;
                    for (unsigned s = 0; s<8;s++){
                        if (!s_used[s]) {
                            k = s;
                            break;
                        }
                    }/*find next available save register*/

                    if (second[0]=='-'||isdigit(second[0])){/*if an int is involved*/

                        if (strcmp(function,"mod")==0){/*if mod by constant*/
                            int z;
                            for (unsigned i = 0; i<10; i++){
                                if (!t_used[i]){
                                    z = i;
                                    t_used[i] = true;
                                    break;
                                }
                            }
                            int g;
                            for (unsigned i = 0; i<8; i++){
                                if (line[0] == s[i].name){
                                    g=i;
                                    break;
                                }
                            }/*find matching register to assignment variable*/
                            printf("li $t%d,%s\n",z,second);
                            printf("div $%s,$t%d\n",first,z);
                            printf("mfhi $s%d\n",g);
                            s_used[g] = true;
                            strcpy(second,"");
                            strcpy(first,"");
                            strcat(first,"t");
                            char y[8];
                            sprintf(y,"%d",z);
                            strcat(first,y);
                        }

                        else if (strcmp(function,"div")==0){/*if dividing by a constant*/

                            int x; //keep track of variable register
                            int number = atoi(second); //convert string format to int for constant
                            char binary[64];
                            strcpy(binary,convert_binary(number));//binary representation as a string

                            for (int i = 0; i<8;i++){
                            
                                if (i == first[1]-48){/*compute variable register based on ASCII value*/
                                    x = i;
                                    break;
                                }
                            }/*find ith save register that variable is in*/

                            if (strcmp(second,"-1")==0){
                                printf("sub $s%d,$zero,$s%d\n",k,x);
                                s_used[k] = true;
                                strcpy(second,"");
                            }/*if constant is -1*/

                            else if (strcmp(second,"1")==0){
                                printf("move $s%d,$s%d\n",k,x);
                                s_used[k] = true;
                                strcpy(second,"");
                            }/*if constant is 1*/
                            
                            else if (check_power2(number)){ /*if constant is a pure power of 2*/
                                if (first[0]=='t') printf("bltz $%s,L%d\n",first,*(level));
                                else printf("bltz $s%d,L%d\n",x,*(level));

                                int samt; //shift amount

                                for (unsigned i = 0; i < strlen(binary)-2;){ /*go through binary string but ignore 2^0*/
                                    if (binary[i]=='0') continue; //skip all OFF signals
                                    else if (binary[i]=='1'){
                                        samt = strlen(binary)-i-1;
                                        break;
                                    }
                                }/*determines shift amount*/

                                if (first[0] == 't') printf("srl $s%d,$%s,%d\n",k,first,samt);
                                else printf("srl $s%d,$s%d,%d\n",k,x,samt);
                                if (second[0]=='-') printf("sub $s%d,$zero,$s%d\n",k,k); //if constant is negative, set value to negative
                                printf("j L%d\n",*(level)+1);
                                printf("L%d:\n",*(level));
                                
                                int temp; //store next free temp register

                                for (unsigned i = 0; i<10; i++){
                                    if (!t_used[i]){
                                        temp = i;
                                        t_used[i] = true;
                                        break;
                                    }
                                }
                                printf("li $t%d,%s\n",temp,second);
                                if (first[0] == 't') printf("div $%s,$t%d\n",first,temp);
                                else printf("div $s%d,$t%d\n",x,temp);
                                printf("mflo $s%d\n",k);
                                printf("L%d:\n",1 + *(level));

                                s_used[k] = true;
                                s[k].name = line[0];
                                *level = *(level) + 2; //increase level
                            }

                            else{/*not a pure power of 2*/
                            
                                for (int i = 0; i < 10; i++){/*look for next available temp register*/

                                    if (!t_used[i]){
                                        
                                        char y[257];
                                        sprintf(y,"%d",i); 
                                        strcpy(destination,"");
                                        strcat(destination,"s");
                                        strcat(destination,y);
                                        //convert register value to string and set destination to i th save register

                                        s_used[k]=true; //set register as being used
                                        s[k].name = line[0];

                                        printf("li $t%d,%s\n",i,second);
                                        printf("%s $%s,$t%d\nmflo $s%d\n",function,first,i,k);
                                        /*print out operation, destination, and registers / integers*/

                                        strcpy(first,destination);
                                        strcpy(destination,"");
                                        strcpy(second,"");

                                        break;
                                    }
                                }
                            }

                        }

                        else if (strcmp(function,"mult")==0){ /*if multiplcation with int*/

                            if (strcmp(second, "0")==0){
                                for (unsigned i = 0; i<8;i++){
                                    if (!s_used[i]){
                                        printf("li $s%d,0\n",i);
                                        break;
                                    }
                                }
                                strcpy(second,"");
                                continue;
                            }/*if multiplied by 0, simply load value into last register*/
                            
                            if (strcmp(second,"1")==0){
                                for (unsigned i = 0; i<10;i++){
                                    if (!t_used[i]){
                                        t_used[i] = true;
                                        printf("move $t%d,$%s\n",i,first);

                                        for (unsigned y = 0; y<8;y++){
                                            if (!s_used[y]){
                                                printf("move $s%d,$t%d\n",y,i);
                                                s_used[y] = true;
                                                s[y].name = line[0];
                                                strcpy(second,"");
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                continue;
                            }/*special case for register multiplying 1*/

                             if (strcmp(second,"-1")==0){
                                for (unsigned i = 0; i<10;i++){
                                    if (!t_used[i]){
                                        t_used[i] = true;
                                        printf("move $t%d,$%s\n",i,first);

                                        for (unsigned y = 0; y<8;y++){
                                            if (!s_used[y]){
                                                printf("sub $s%d,$zero,$t%d\n",y,i);
                                                s_used[y] = true;
                                                s[y].name = line[0];
                                                strcpy(second,"");
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                continue;
                            }/*special case for register multiplying -1*/

                            char binary[64];
                            strcpy(binary, convert_binary(atoi(second))); //convert int to string of binary
                            
                            char spot1[257];
                            strcpy(spot1,"");
                            /*serve similar purpose to first and second*/

                            for (unsigned m = 0; m<strlen(binary); m++){/*go through binary string*/

                                if (binary[m]=='0'&& m==strlen(binary)-1){
                                    if (second[0]=='-'){
                                        printf("sub $s%d,$zero,%s\n",k,spot1);
                                    }
                                    else{
                                        printf("move $s%d,%s\n",k,spot1);
                                    }
                                }/*move registers for even numbers*/
                                
                                if (binary[m]=='0') continue; //skip OFF signals in binary

                                if (m==strlen(binary)-1){
                                    printf("add %s,%s,$%s\n",spot1,spot1,first);

                                    if (second[0]=='-'){
                                        printf("sub $s%d,$zero,%s\n",k,spot1);
                                    }
                                    else{
                                        printf("move $s%d,%s\n",k,spot1);
                                    }
                                    break;
                                }/*simply add the 2^0 * n if ON*/

                                int samt = strlen(binary)-m-1; //shift amount

                                if (strcmp(spot1,"")==0){/*if no temp registers have been filled*/
                                    
                                    for (unsigned i = 0;i<10;i++){/*find unused temp register*/
                                        
                                        if (!t_used[i]){
                                            
                                            t_used[i] = true;

                                            printf("sll $t%d,$%s,%d\n",i,first,samt);
                                            
                                            for (int b = 0; b<10; b++){/*find new unused temp register*/
                                                
                                                if (!t_used[b]){
                                                    printf("move $t%d,$t%d\n",b,i);
                                                    t_used[i] = false; //set original temp register to unused
                                                    char temp[8];
                                                    sprintf(temp,"%d",b);
                                                    strcpy(spot1,"$t");
                                                    strcat(spot1,temp); /*copy current temp register to spot1*/
                                                    break;
                                                }
                                            }  
                                            break;
                                        }
                                    }
                                    
                                }

                                else{   /*1 temp register active*/
                    
                                    for (unsigned i = 0; i<10; i++){
                                        if (!t_used[i]){
                                            printf("sll $t%d,$%s,%d\n",i,first,samt);
                                            t_used[i] = true;
                                            for (unsigned y = 0; y<10;y++){
                                                if (!t_used[y]){
                                                    printf("add $t%d,$t%d,$t%d\n",y,y,i);
                                                    t_used[i] = false;
                                                    break;
                                                }
                                            }/*find available temp registers to add and shift*/
                                            break;
                                        }
                                    }/*add sums of powers of 2 into temp register*/
                                }
                            }

                            strcpy(second,"");
                            char temp[257];
                            sprintf(temp,"$s%d",k);
                            strcpy(first,"");
                            strcat(first, temp);
                        }/*new value computed, second is now empty while first contains register*/
                    }
                    else{ /*operation between registers*/
                        for (int i = 0; i < 8; i++){/*look for next available save register*/

                            if (!s_used[i]){
                                
                                char y[257];
                                sprintf(y,"%d",i); 
                                strcpy(destination,"");
                                strcat(destination,"s");
                                strcat(destination,y);
                                //convert int value to string and set destination to i th save register

                                s_used[i]=true; //set register as being used
                                s[i].name = line[0];

                                if (strcmp(function,"mod")==0) printf("div $%s,$%s\nmfhi $%s\n",first,second,destination);
                                else printf("%s $%s,$%s\nmflo $%s\n",function,first,second,destination);
                                /*print out operation, destination, and registers / integers*/

                                strcpy(first,destination);
                                strcpy(destination,"");
                                strcpy(second,"");

                                break;
                            }
                        }
                    }

                }

                else{/*if not the last item to be computed*/

                    int k;
                    for (unsigned s = 0; s<10;s++){
                        if (!t_used[s]) {
                            k = s;
                            break;
                        }
                    }/*find next available temp register*/

                    if (second[0]=='-'||isdigit(second[0])){/*if an int is involved*/

                        if (strcmp(function,"mod")==0){/*if mod by a constant*/

                            int j;
                            for (unsigned s = 0; s<10;s++){
                                if (!t_used[s]) {
                                    j = s;
                                    t_used[s] = true;
                                    break;
                                }
                            }/*find next available temp register*/

                            printf("li $t%d,%s\n",j,second);
                            printf("div $%s,$t%d\n",first,j);
                            printf("mfhi $t%d\n",j+1);

                            t_used[j+1] = true;
                            
                            strcpy(second,"");
                            strcpy(first,"");
                            strcat(first,"t");
                            char y[8];
                            sprintf(y,"%d",j+1);
                            t_used[j+1] = true;
                            strcat(first,y);
                        }

                        else if (strcmp(function,"div")==0){/*if dividing by a constant*/

                            int x; //keep track of variable register
                            int number = atoi(second); //convert string format to int for constant
                            char binary[64];
                            strcpy(binary,convert_binary(number));//binary representation as a string

                            for (int i = 0; i<8;i++){
                            
                                if (i == first[1]-48){/*compute variable register based on ASCII value*/
                                    x = i;
                                    break;
                                }
                            }/*find ith save register that variable is in*/

                            if (strcmp(second,"-1")==0){
                                printf("sub $t%d,$zero,$s%d\n",k,x);
                                strcpy(first,"t");
                                char y[257];
                                sprintf(y,"%d",k);
                                strcat(first,y);
                                s_used[k] = true;
                                strcpy(second,"");
                            }/*if constant is -1*/

                            else if (strcmp(second,"1")==0){
                                printf("move $t%d,$s%d\n",k,x);
                                s_used[k] = true;
                                strcpy(first,"t");
                                char y[257];
                                sprintf(y,"%d",k);
                                strcat(first,y); 
                                strcpy(second,"");
                            }/*if constant is 1*/
                            
                            else if (check_power2(number)){ /*if constant is a pure power of 2*/
                                if (first[0]=='t') printf("bltz $%s,L%d\n",first,*(level));
                                else printf("bltz $s%d,L%d\n",x,*(level));

                                int samt; //shift amount

                                for (unsigned i = 0; i < strlen(binary)-2;){ /*go through binary string but ignore 2^0*/
                                    if (binary[i]=='0') continue; //skip all OFF signals
                                    else if (binary[i]=='1'){
                                        samt = strlen(binary)-i-1;
                                        break;
                                    }
                                }/*determines shift amount*/

                                if (first[0] == 't') printf("srl $t%d,$%s,%d\n",k,first,samt);
                                else printf("srl $t%d,$s%d,%d\n",k,x,samt);
                                if (second[0]=='-') printf("sub $t%d,$zero,$t%d\n",k,k); //if constant is negative, set value to negative
                                printf("j L%d\n",*(level)+1);
                                printf("L%d:\n",*(level));
                                
                                int temp; //store next free temp register

                                for (unsigned i = 0; i<10; i++){
                                    if (!t_used[i]){
                                        temp = i;
                                        t_used[i] = true;
                                        break;
                                    }
                                }
                                printf("li $t%d,%s\n",temp,second);
                                if (first[0] == 't') printf("div $%s,$t%d\n",first,temp);
                                else printf("div $s%d,$t%d\n",x,temp);
                                printf("mflo $t%d\n",k);
                                printf("L%d:\n",1 + *(level));

                                s_used[k] = true;
                                s[k].name = line[0];
                                *level = *(level) + 2; //increase level
                            }

                            else{/*not a pure power of 2*/
                            
                                for (int i = 0; i < 10; i++){/*look for next available temp register*/

                                    if (!t_used[i]){

                                        s_used[k]=true; //set register as being used
                                        s[k].name = line[0];

                                        int new_temp;
                                        printf("li $t%d,%s\n",i,second);

                                        for (unsigned y = 0; i<10;i++){
                                            if (!t_used[y]){
                                                y++;
                                                t_used[y-1] = true;
                                                t_used[y] = true;
                                                new_temp = y;
                                                break;
                                            }
                                        }
                                        printf("%s $%s,$t%d\nmflo $t%d\n",function,first,i,new_temp);
                                        /*print out operation, destination, and registers / integers*/

                                        char y[257];
                                        sprintf(y,"%d",new_temp); 
                                        strcpy(destination,"");
                                        strcat(destination,"t");
                                        strcat(destination,y);
                                        //convert register value to string and set destination to i the temp register

                                        strcpy(first,destination);
                                        strcpy(destination,"");
                                        strcpy(second,"");

                                        break;
                                    }
                                }
                            }

                        }

                        else if (strcmp(function,"mult")==0){ /*if multiplcation with int*/

                            if (strcmp(second, "0")==0){
                                printf("li $%s,0\n",first);
                                strcpy(second,"");
                                continue;
                            }/*if multiplied by 0, simply load value into last register*/
                            
                            else if (strcmp(second,"1")==0){
                                for (unsigned i = 0; i<10;i++){
                                    if (!t_used[i]){
                                        t_used[i] = true;
                                        printf("move $t%d,$%s\n",i,first);

                                        for (unsigned y = 0; y<10;y++){
                                            if (!t_used[y]){
                                                printf("move $t%d,$t%d\n",y,i);
                                                t_used[y] = true;
                                                strcpy(second,"");
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                continue;
                            }/*special case for register multiplying 1*/

                            else if (strcmp(second,"-1")==0){
                                for (unsigned i = 0; i<10;i++){
                                    if (!t_used[i]){
                                        t_used[i] = true;
                                        printf("move $t%d,$%s\n",i,first);

                                        for (unsigned y = 0; y<10;y++){
                                            if (!t_used[y]){
                                                printf("sub $t%d,$zero,$t%d\n",y,i);
                                                t_used[y] = true;
                                                strcpy(second,"");
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                continue;
                            }/*special case for register multiplying -1*/

                            char binary[64];
                            strcpy(binary, convert_binary(atoi(second))); //convert int to string of binary
                            
                            char spot1[257];
                            strcpy(spot1,"");
                            /*serve similar purpose to first and second*/

                            int n;//next unused temp register
                            for (unsigned i = 0; i< 10; i++){
                                if (!t_used[i]){
                                    n = i;
                                    break;
                                }
                            }

                            for (unsigned m = 0; m<strlen(binary); m++){/*go through binary string*/

                                if (binary[m]=='0'&& m==strlen(binary)-1){
                                    if (second[0]=='-'){
                                        printf("sub $t%d,$zero,%s\n",n,spot1);
                                    }
                                    else{
                                        printf("move $t%d,%s\n",n,spot1);
                                    }
                                }/*move registers for even numbers*/
                                
                                if (binary[m]=='0') continue; //skip OFF signals in binary

                                if (m==strlen(binary)-1){
                                    printf("add %s,%s,$%s\n",spot1,spot1,first);

                                    if (second[0]=='-'){
                                        t_used[n] = true;
                                        printf("sub $t%d,$zero,%s\n",n+2,spot1);
                                    }
                                    else{
                                        t_used[n] = true;
                                        t_used[n+1] = true;
                                        t_used[n+2] = true;
                                        printf("move $t%d,%s\n",n+2,spot1);
                                    }
                                    char y[8];
                                    sprintf(y, "%d",n+2);
                                    strcpy(first,"t");
                                    strcat(first,y);
                                    strcpy(second,"");
                                    break;
                                }/*simply add the 2^0 * n if ON*/

                                int samt = strlen(binary)-m-1; //shift amount

                                if (strcmp(spot1,"")==0){/*if no temp registers have been filled*/
                                    
                                    for (unsigned i = 0;i<10;i++){/*find unused temp register*/
                                        
                                        if (!t_used[i]){
                                            
                                            t_used[i] = true;

                                            printf("sll $t%d,$%s,%d\n",i,first,samt);
                                            
                                            for (int b = 0; b<10; b++){/*find new unused temp register*/
                                                
                                                if (!t_used[b]){
                                                    printf("move $t%d,$t%d\n",b,i);
                                                    t_used[i] = false; //set original temp register to unused
                                                    char temp[8];
                                                    sprintf(temp,"%d",b);
                                                    strcpy(spot1,"$t");
                                                    strcat(spot1,temp); /*copy current temp register to spot1*/
                                                    break;
                                                }
                                            }  
                                            break;
                                        }
                                    }
                                    
                                }

                                else{   /*1 temp register active*/
                    
                                    for (unsigned i = 0; i<10; i++){
                                        if (!t_used[i]){
                                            printf("sll $t%d,$%s,%d\n",i,first,samt);
                                            t_used[i] = true;
                                            for (unsigned y = 0; y<10;y++){
                                                if (!t_used[y]){
                                                    printf("add $t%d,$t%d,$t%d\n",y,y,i);
                                                    t_used[i] = false;
                                                    break;
                                                }
                                            }/*find available temp registers to add and shift*/
                                            break;
                                        }
                                    }/*add sums of powers of 2 into temp register*/
                                }
                            }

                            int v;
                            strcpy(second,"");
                            char temp[257];
                            for (unsigned i = 0; i<10;i++){
                                if (!t_used[i]){
                                    v = i-1;
                                    break;
                                }
                            }
                            sprintf(temp,"t%d",v);
                            strcpy(first,"");
                            strcat(first, temp);
                        }/*new value computed, second is now empty while first contains register*/
                    }
                    else{/*if no ints are involved*/
                        for (int i = 0; i < 10; i++){/*look for next available temp register*/

                            if (!t_used[i]){
                                
                                char y[257];
                                sprintf(y,"%d",i);
                                strcpy(destination,"");
                                strcat(destination,"t");
                                strcat(destination,y);
                                t_used[i] = true;
                                /*change int to string, and set destination to i th t register*/

                                if (strcmp(function,"mod")==0) printf("div $%s,$%s\nmfhi $%s\n",first,second,destination);
                                else printf("%s $%s,$%s\nmflo $%s\n",function,first,second,destination);
                                /*print out operation, destination, and registers / integers*/

                                strcpy(first,destination);
                                strcpy(destination,"");
                                strcpy(second,"");
                                /*reset second value and destination. Set previous destination to current first.*/

                                break;
                            }
                        }
                    }
                
                }
            }

            if (line[index]==' '){
                index++;
                continue;
            }/*skip spaces*/

            if (line[index]==';') break;
            //reached end of line
            
            if (isalpha(line[index])){/*if a variable is detected*/

                bool exist = false;
                
                for (unsigned int i = 0; i < 8; i++){

                    if (s[i].name == line[index]){/*find matching save register*/
                        exist = true;
                        char y[2];

                        if (strcmp(first,"")==0){
                            sprintf(y,"%d", i);
                            strcat(first,"s");
                            strcat(first,y);
                        }/*store in first if empty*/

                        else  {
                            sprintf(y,"%d", i);
                            strcat(second,"s");
                            strcat(second,y);
                        }/*else store in second*/

                        break;
                    }
                }

                if (!exist){/*if variable detected was never declared*/
                    char y[2];
                    for (unsigned i = 0; i<8; i++){
                        if (!s_used[i]&&s[i].name!=line[0]){/*does not remove assignment variable*/
                            s[i].name = line[index];
            
                            s_used[i] = true;
                            if (strcmp(first,"")==0){
                                sprintf(y,"%d", i);
                                strcat(first,"s");
                                strcat(first,y);
                            }/*store in first if empty*/

                            else  {
                                sprintf(y,"%d", i);
                                strcat(second,"s");
                                strcat(second,y);
                            }/*else store in second*/
                            break;
                        }
                    }
                }

                index++;
                continue;
                
            }

            else if (!isalpha(line[index]) && !isdigit(line[index])){/*if an operator is detected*/
                if (line[index] == '*') strcpy(function,"mult");
                else if (line[index] == '/') strcpy(function,"div"); 
                else if (line[index] == '%') strcpy(function,"mod");
                index++;
            }/*set function to mult or div*/

            else if(isdigit(line[index])||line[index]=='-') {/*if a digit is detected*/
                char y[3];

                while (true){

                    if (line[index-1]=='-'){
                        y[0] = '-';
                        y[1] = line[index];
                        y[2] = '\0';
                    }
                    else{
                        y[0] = line[index];
                        y[1] = '\0';
                    }/*accounts for negative numbers*/

                    if (!isdigit(line[index])) break;
                    if (strcmp(first,"")==0)strcat(first,y);
                    else strcat(second,y);
                    index++;
                    
                }/*capture entire int and set to first if empty and second if first is occupied*/
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                            /*Main starts here*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[]){

    FILE* file = fopen(argv[1],"r"); 
    //open file in read mode 

    if (argc<2){
        printf("invalid amount of arguments\n");
        return 1;
    }
    if (file == NULL){
        printf("Failed to open file.\n");
        return 1;
    }/*error checking*/

    char current_line[257];
    int id; //identifies the type of operation
    struct Variables s[8]; //saved mips variables ($t0,$t1,etc)
    bool s_used[8] = {false,false,false,false,false,false,false,false}; 
    //stores saved register availability

    bool t_used[10] = {false,false,false,false,false,false,false,false,false,false};
    /*same thing as s but for temporary variables*/

    int level = 0; //used to keep track of L0,L1,etc


    while (fgets(current_line,257,file) !=NULL){

        id = calc_type(current_line); //determine the operation type
        print_comment(current_line); //print c code line as comment

        execute(id,s,s_used,t_used,current_line,&level); //meat of the program
        
    }/*loop through each line in file*/

    fclose(file); //close file

    return 0;

}