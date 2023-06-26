#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define PROCESS_NUM 5
#define RESOURCE_NUM 5


void read_files(FILE * falloc,FILE * freq, FILE * fres, int alloc[][RESOURCE_NUM], int req[][RESOURCE_NUM], int res[]){
    int i=0;
    int j=0;
    for(i=0;i<PROCESS_NUM;i++){
        for(j=0;j<RESOURCE_NUM;j++){
            fscanf(falloc,"%d",&alloc[i][j]);
        }
    }
    
    for(i=0;i<PROCESS_NUM;i++){
        for(j=0;j<RESOURCE_NUM;j++){
            fscanf(freq,"%d",&req[i][j]);
        }
    }
    
    for(i=0;i<RESOURCE_NUM;i++){
        fscanf(fres,"%d",&res[i]);

    }
}



int main(){
    FILE * f_req=fopen("requests.txt","r");
    FILE * f_alloc=fopen("allocations.txt","r");
    FILE * f_res=fopen("resources.txt","r");
    char line[20];
    
    int req[PROCESS_NUM][RESOURCE_NUM]; //This is same with need
    int alloc[PROCESS_NUM][RESOURCE_NUM];
    int res[RESOURCE_NUM];
    //int available[RESOURCE_NUM]; // This will be res-alloc

    int work[RESOURCE_NUM];
    int finish[PROCESS_NUM]; // 0 means not finished, 1 means it is finished.

    int results[PROCESS_NUM+1];
    int deadlocks[PROCESS_NUM+1]; //array length is PROCESS_NUM+1, -1 means slots are not used
    int results_num=0;
    int deadlocks_num=0;
    
    int i;
    int j;

    for(i=0;i<PROCESS_NUM+1;i++){
        results[i]=-1;
        deadlocks[i]=-1;
        finish[i]=0;
    }

    read_files(f_alloc,f_req,f_res,alloc,req,res);

    for(i=0;i<PROCESS_NUM;i++){
        printf("Information for process: P%d",i+1);
        printf("\n");
        printf("Allocated resources: ");
        for(j=0;j<RESOURCE_NUM;j++){
            printf("  R%d:%d  ",j+1,alloc[i][j]);
        }
        printf("\n");
        printf("Resources request:   ");
        for(j=0;j<RESOURCE_NUM;j++){
            printf("  R%d:%d  ",j+1,req[i][j]);
        }
        printf("\n\n");
    }



    printf("\n\n");




    
    for(j=0;j<RESOURCE_NUM;j++){
        int alloc_j=0; // Total allocation of resource j
        for(i=0;i<PROCESS_NUM;i++){
            alloc_j+=alloc[i][j];
        }
        work[j]=res[j]-alloc_j; //work is equal to available.
    }
    
    int flag1=1;
    while(flag1){
        flag1=0;
        //After looping through all of processes,
        //if a process is finished, flag1 will be 1.
        //else it will be 0 and while loop will end.
        //After looping thorugh all processes, a process may not finish
        //because there may not be left any process to finish or...
        //there is a DEADLOCK.
        for(i=0;i<PROCESS_NUM;i++){
            if(finish[i]==0){
                int flag2=1;
                for(j=0;j<RESOURCE_NUM;j++){
                    if(req[i][j]>work[j]){
                        flag2=0; //Process is not finished, 
                        //do system enough resources to meet its requests?
                    }
                }
                if(flag2){
                    for(j=0;j<RESOURCE_NUM;j++){
                        work[j]+=alloc[i][j];
                    }
                    flag1=1;
                    finish[i]=1;
                    results[results_num]=i;
                    results_num++;
                }
            }
        }
    }

    //Eventually flag1 will be 0, let's try to find why it became 0.
    //If it became 0 because everything was finished, it is okay, but otherwise
    // unfinished tasks are deadlocked ones.
    flag1=1;
    for(i=0;i<PROCESS_NUM;i++){
        if(finish[i]==0){
            deadlocks[deadlocks_num]=i;
            deadlocks_num++;
            flag1=0;
        }
    }

    //After this stage if flag1==1 there is no deadlock, every process is finished.
    //If flag1==0, some of processes is not finished, there is deadlock.    


    printf("Running order for processes: ");
    i=0;
    while(results[i]!=-1){
        printf("P%d ",results[i]+1);
        i++;
    }
    printf("\n");

    //If there is deadlock, report it!
    if(flag1==0){
        printf("There is a deadlock. The causes of deadlock: ");
        i=0;
        while(deadlocks[i]!=-1){
            printf("P%d, ",deadlocks[i]+1);
            i++;
        }
    }
    printf("\n");




    return 0;
}








/*
for(i=0;i<PROCESS_NUM;i++){
            
            for(j=0;j<RESOURCE_NUM;j++){
                printf("%d ",alloc[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");
        for(i=0;i<PROCESS_NUM;i++){
            
            for(j=0;j<RESOURCE_NUM;j++){
                printf("%d ",req[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");
        for(i=0;i<RESOURCE_NUM;i++){
            printf("%d ",res[i]);
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