#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>

int hash(char* temp, int cache_size){
    
    int sum = 0;
    int length = strlen(temp);
    
    for (int i = 0; i < length; i++){
        sum+=*(temp+i);
    }
    
    return sum % cache_size;
}

int main(int argc, char ** argv){

    ///////////////////////////INITIALIZATION////////////////////

    int cache_size = atoi(*(argv+1));
    char** cache;

    if (cache_size > 0) cache = (char **)calloc(cache_size, sizeof(char*));
    else {
        fprintf(stderr,"Cache size must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    if (argc < 3){
        fprintf(stderr, "Error: invalid number of arguments.\n");
        fprintf(stderr,"USAGE: a.out n <filename>\n");
        return EXIT_FAILURE;
    }
    ///////////////////////////////////////////////////////////////

for (int p = 1; p < argc-1; p++){
    
    int f_read = open(*(argv+1+p),O_RDONLY);

    if (f_read == -1){
        perror("open() failed\n");
        return EXIT_FAILURE;
    }

    int c;
    char * buffer = calloc(4,sizeof(char));
    char * temp = calloc(129,sizeof(char));
    int index = 0;
 
    while ((c = read(f_read, buffer, 3)) >= 3) {

        *(buffer+3) = '\0';

        for (int i = 0; i < 3; i++){
            
            *(temp+i+index) = *(buffer+i);

            if (!isalnum(*(buffer+i))){
        
                *(temp+i+index) = '\0';
                lseek(f_read,-1*(2-i),SEEK_CUR);
                index = -3;

                // only accept suitable temp words //
                if (strlen(temp)>=3){
                    int hash_num = hash(temp,cache_size);
                    
                    if (*(cache+hash_num) == NULL){
                        printf("Word \"%s\" ==> %d (calloc)\n",temp,hash_num);
                        *(cache+hash_num)= calloc((strlen(temp)+1),sizeof(char));
                    }

                    else if (strlen(*(cache+hash_num)) == strlen(temp)){
                        printf("Word \"%s\" ==> %d (nop)\n",temp,hash_num);
                    }

                    else {
                        printf("Word \"%s\" ==> %d (realloc)\n",temp,hash_num);
                        char * x = realloc(*(cache+hash_num),(strlen(temp)+1)*sizeof(char));
                        *(cache+hash_num) = x;   
                    }

                    strcpy(*(cache+hash_num),temp);

                }
                
                free(temp);
                temp = calloc(129,sizeof(char));
                break;
            } 
        }

        index += 3;

    }
    free(buffer);
    free(temp);
    close(f_read);
}

    printf("\nCache:\n");
    for (int i = 0; i <cache_size; i++){
        if (*(cache+i) == NULL) continue;
        printf("%c%d%c ==> \"%s\"\n",91,i,93,*(cache+i));
    }

//////////////////////////// FREE ///////////////////////////////

    for (int i = 0; i < cache_size; i++){

        if (*(cache+i)!=NULL) free(*(cache+i));

    }

    free(cache);

}