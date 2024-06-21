#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

bool isValidInt(const char *str){
    for (unsigned int i = 0; i < strlen(str); i++) if (!isdigit(*(str+i))) return false;
    return true;
}

bool checkmate(int x, int y, int m, int n, char ** board){
    
    for (int i = 0; i < m; i++){
        if (i == x) continue;
        if (*(*(board+i)+y)!=0) return true;
    }/*check entire row*/

    for (int i = 0; i < n; i++){
        if (i == y) continue;
        if (*(*(board+x)+i)!=0) return true;
    }/*check entire column*/

    int i = x;
    int j = y;

    while (i > 0 && j > 0){
        i--;
        j--;
        if (*(*(board+i)+j) != 0) return true;
    }/*diagonal to top left*/

    i = x;
    j = y;

    while (i < m-1 && j < n-1){
        i++;
        j++;
        if (*(*(board+i)+j) != 0) return true;
    }/*diagonal to bottom right*/

    i = x;
    j = y;
   
    while (i < m-1 && j > 0){
        i++;
        j--;
        if (*(*(board+i)+j) != 0) return true;
    }/*diagonal to bottom left*/

    i = x;
    j = y;

    while (i > 0 && j < n-1){
        i--;
        j++;
        if (*(*(board+i)+j) != 0) return true;
    }/*diagonal to top right*/

    return false;
}

void place_queen(int x, int y, int m, int n, char ** board, int* pipefd){
    
    *(*(board+x)+y) = 'q';
    x++;
    if (x==m){
        //solution reached if you get to this point
        #ifndef QUIET
            printf("P%d: found a solution; notifying top-level parent\n",getpid());
        #endif
        
        int temp = 0;
        if ((temp = write(*(pipefd+1),&x,sizeof(int)))<0){
                fprintf(stderr,"Something went wrong with the write. Value read: %d\n", temp );
                perror("");
            }
        return;
    }

    bool possible = false;
    int counter = 0;
    pid_t child;

    for (int b = 0; b < n; b++) if (*(*(board+x)+b) == 0 && !checkmate(x,b,m,n,board)) counter++; //count the amount  of possible moves

    #ifndef QUIET
    char* move = "move";
    char* process = "process";
    if (counter>1){
        process = "processes";
        move = "moves";
    }/*grammar*/
    if (counter != 0) printf("P%d: %d possible %s at row #%d; creating %d child %s...\n",getpid(),counter,move,x,counter,process);
    #endif

    for (int b = 0; b < n; b++){

        if (*(*(board+x)+b) == 0 && !checkmate(x,b,m,n,board)){

            possible = true;

            child = fork();

            #ifdef NO_PARALLEL
                int status;
                while ((waitpid(child,&status,0))>0);
            #endif

            if (child == -1) abort(); /*child fails to fork*/

            //child process
            if (child == 0) place_queen(x,b,m,n,board, pipefd);

            //parent process
            else{
                int status;
                while ((waitpid(-1,&status,0))>0){
                    if (WIFSIGNALED(status)) abort(); 
                }
            }
        }
    }

    //dead end reached if you get to this point
        #ifndef QUIET
            if (!possible) printf("P%d: dead end at row #%d; notifying top-level parent\n", getpid(),x);
        #endif
        int temp = 0;
        if (!possible){
            
            if ((temp = write(*(pipefd+1),&x,sizeof(int)))<0){
                fprintf(stderr,"Something went wrong with the write. Value read: %d\n", temp );
                perror("");
            }
        } 
}

int main(int argc, char** argv){

    //required line for the assignment
    setvbuf( stdout, NULL, _IONBF, 0 );

    if (argc!=3){
        fprintf(stderr, "Incorrect amount of arguments.\n");
        return EXIT_FAILURE;
    }

    if (!isValidInt(*(argv+1))||!isValidInt(*(argv+2))){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n>\n");
        return EXIT_FAILURE;
    }/*check if inputs are ints*/

    int m = atoi(*(argv+1));
    int n = atoi(*(argv+2));

    if (m<1 || n <1){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n>\n");
        return EXIT_FAILURE;
    }/*check if numbers are greater than 0*/

    /*if n < m, swap the values*/
    if (n<m){
        int temp;
        temp = m;
        m = n;
        n = temp;
    }

    if (m>256||n>256){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n>\n");
        return EXIT_FAILURE;
    }/*check if any value is greater than 256*/

    int* pipefd = calloc(2,sizeof(int));
    int rc = pipe(pipefd);
    int* answer = calloc(m+1,sizeof(int));

    if (rc == -1){
        perror("Pipe failed\n");
        return EXIT_FAILURE;
    }

    /*initialize the board*/
    char** board = calloc(m,sizeof(char*));
    for (int i = 0; i < m; i++) *(board+i) = calloc(n,sizeof(char));
    
    bool header = true;
    int counter = 0;
    pid_t child;
    int status = 0;

    for (int i = 0; i < n; i++) if (*(*(board)+i) == 0 && !checkmate(0,i,m,n,board)) counter++; /*count possible moves*/

    if (header){
        char* process = "process";
        char* move = "move";
        if (counter>1){
            process = "processes";
            move = "moves";
        }/*grammar*/
        printf("P%d: solving the Abridged (m,n)-Queens problem for %dx%d board\n",getpid(),m,n);
        printf("P%d: %d possible %s at row #%d; creating %d child %s...\n",getpid(),counter,move,0,counter,process);
    } 

    header = false; //assures header will only print once

    for (int i = 0; i < n; i++){
            
        if (*(*(board)+i) == 0 && !checkmate(0,i,m,n,board)){

            child = fork();

            #ifdef NO_PARALLEL
                int status;
                waitpid(child,&status,0);
            #endif

            if (child == -1) abort();
            
            //child
            if (child == 0){
                close(*(pipefd));
                place_queen(0,i,m,n,board,pipefd);
                close(*(pipefd+1));
            }
        }
    }    

    //child
    if (child == 0) close(*(pipefd+1));
    //parent
    else{
        while ((waitpid(-1,&status,0))>0){
            if (WIFSIGNALED(status)){
                fprintf(stderr,"The program ran into an error.\n");
                return EXIT_FAILURE;
            }
        }

        close(*(pipefd+1));
        printf("P%d: search complete\n", getpid());

        int buffer = 0;
        while (read(*(pipefd),&buffer,sizeof(int))>0) *(answer+buffer) = *(answer+buffer)+1;
        for (int i = 1; i<=m; i++) printf("P%d: number of %d-Queen end-states: %d\n",getpid(),i,*(answer+i));
        
    }

    //free memory
    for (int i = 0; i < m; i++) free(*(board+i));
    free(board);
    free(pipefd);
    free(answer);

    return EXIT_SUCCESS;
}