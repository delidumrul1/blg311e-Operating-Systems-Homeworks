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

    
    
    int main_pid=getppid();
    
    for(;i<N;i++)
    {
        if(donut==0)
        {
            
            if(getppid()!=main_pid)
                printf("Hi I'm child PID: %d and my parent is PPID: %d\n", getpid(), getppid());
            donut=fork();
            

            
            if(donut>0){
                
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
            /*
            donut2=fork();
            if(donut2==0){
                printf("Hiii I'm a child PID: %d, my father is PPID: %d\n", getpid(), getppid());
            }
            */
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