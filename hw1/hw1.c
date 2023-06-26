#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>



int main()
{

    int donut=0;
    int donut2=0;
    int i=0;
    int j=0;
    int N=0;
    int M=0;
    
    printf("Enter the number N: ");
    scanf("%d",&N);
    printf("Enter the number M: ");
    scanf("%d",&M);
    N++;
    
    
    donut=fork();
    //Thanks to these variables I can separate between child and parent 
    //processes.
    
    
    int main_pid=getppid();
    
    //This loop is for to "multiply" same chain of processes that I talked 
    // about below. Thanks to this loop there will be N+1 chaines.
    for(;i<N;i++)
    {
        if(donut==0)
        {   
            
            // I did not want to deal with main process, so I did this.
            if(getppid()!=main_pid)
                printf("Hi I'm child PID: %d and my parent is PPID: %d\n", getpid(), getppid());
            donut=fork();
            

            
            if(donut>0){
                //This first loop is for one long chain of processes. 
                //This chain I'm talking about, contains M+1 processes.
                for(;j<M;j++){
                
                if(donut2==0)
                    donut2=fork();
                if(donut2==0){
                    
                    printf("Heey hey people, I'm child PID: %d and my parent is PPID: %d\n", getpid(), getppid());

                }
                else
                {
                wait(NULL);
                }
                
                }
            
            
            }

            


        }
        else
        {   
           // We are waiting to child processes end. If we don't 
           //Children can be zombies and lost their parents. We don't want that.
            wait(NULL);
        }
        

    }   





}


/*


                if(donut2==0)
                    donut2=fork();
                if(donut2==0){
                    
                    printf("Hiii I'm a child PID: %d, my father is PPID: %d\n", getpid(), getppid());

                }
                else
                {
                wait(NULL);
                }
                
                if(donut2==0)
                    donut2=fork();
                if(donut2==0){
                    
                    printf("Hiii I'm a child PID: %d, my father is PPID: %d\n", getpid(), getppid());

                }
                else
                {
                wait(NULL);
                }
                
                if(donut2==0)
                    donut2=fork();
                if(donut2==0){
                    
                    printf("Hiii I'm a child PID: %d, my father is PPID: %d\n", getpid(), getppid());

                }
                else
                {
                wait(NULL);
                }
*/







/*
.: ⡴⠑⡄⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠸⡇⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ 
⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ 
⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ 
⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉
*/
