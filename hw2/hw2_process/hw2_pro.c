#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include<time.h>
#define MAX_PRODUCT_NUM 5
#define MAX_CST_NUM 3

//Most of the code here is same with thread version.
//Thus most of the comments is there, comments here is special to process part.




double double_rand ( double low, double high )
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

int int_rand ( int low, int high )
{
    return rand()%(high-low+1)+low;
}




typedef struct __product {
    int id;
    double price;
    int quantity;
} product;

typedef struct __ordered_item{
    int id;
    int quantity;
} ordered_item;

typedef struct __purchased_item {
    int id;
    int quantity;
} purchased_item;

typedef struct __customer {
    int id;
    double balance;
    pid_t pid;
    ordered_item ordered_items[MAX_PRODUCT_NUM];
    purchased_item purchased_items[MAX_PRODUCT_NUM];
} customer;


//Here are the data structures that our program depends on.
//globals struct contains the main players.
typedef struct __globals{
product products[MAX_PRODUCT_NUM];
customer customers[MAX_CST_NUM];
sem_t m_lock;
} globals;




void initialize(void * arg1){
    globals * g = (globals *) arg1;
    int i;
    sem_init(&g->m_lock, 1, 1);
    for(i=0;i<MAX_PRODUCT_NUM;i++){
        g->products[i].id=i;
        g->products[i].price=double_rand(1,200);
        g->products[i].quantity=int_rand(1,10);
    }

    for(i=0;i<MAX_CST_NUM;i++){
        g->customers[i].id=i;
        g->customers[i].pid=0;
        g->customers[i].balance=double_rand(100,1000);
        int j;
        for(j=0;j<MAX_PRODUCT_NUM;j++){
            //-1 means that this slot is not used.
            g->customers[i].ordered_items[j].id=-1;
            g->customers[i].ordered_items[j].quantity=-1;
            g->customers[i].purchased_items[j].id=-1;
            g->customers[i].purchased_items[j].quantity=-1;
        }
    }

    //ordered_items is generated random too.
    for(i=0;i<MAX_CST_NUM;i++){
        int ordered_num=int_rand(0,5);
        //printf("ordered_num=%d,  %d,  %d\n",ordered_num,int_rand(0,5),int_rand(0,5));
        int j;
        for(j=0;j<ordered_num;j++){
            int iid=int_rand(0,4);
            int quant=int_rand(1,5);
            int k;
            int exist_before=0;
            for(k=0;k<j;k++){
                if(g->customers[i].ordered_items[k].id==iid){
                    g->customers[i].ordered_items[k].quantity+=quant;
                    if(g->customers[i].ordered_items[k].quantity>5)
                        g->customers[i].ordered_items[k].quantity=5;
                    exist_before=1;
                    break;
                }
            }
            if(exist_before==0){
                g->customers[i].ordered_items[j].id=iid;
                g->customers[i].ordered_items[j].quantity=quant;
            }
        }
    }



}


void * order(void * arg1,void * arg2){
    customer * cst= (customer *) arg1;
    globals * g = (globals * ) arg2;
    int i;
    double initial_balance=cst->balance;
    for(i=0;i<MAX_PRODUCT_NUM;i++){
        if(cst->ordered_items[i].id!=-1){
            
            sem_wait(&g->m_lock);

            //printing first half
            printf("Customer_%d:\n",cst->id);
            printf("Initial products:\n");
            printf("Products ID    Quantity       Price\n");
            int t;
            for(t=0;t<MAX_PRODUCT_NUM;t++){
                if(g->products[t].id!=-1){
                printf("%-15d%-15d%-15.2lf\n",g->products[t].id,
                g->products[t].quantity,g->products[t].price);
                }
            }


            double cost=cst->ordered_items[i].quantity*g->products[cst->ordered_items[i].id].price;
            double remaining_balance=cst->balance-cost;
            double remaining_products=g->products[cst->ordered_items[i].id].quantity
            -cst->ordered_items[i].quantity;
            
            
            if(remaining_balance<0){
                printf("Customer%d(%d,%d) fail! You are too poor! Ha ha:D\n "
                ,cst->id,cst->ordered_items[i].id,cst->ordered_items[i].quantity);
            }
            
            else if(remaining_products<0){
                printf("Customer%d(%d,%d) fail! Only %d left in stock.\n "
                ,cst->id,cst->ordered_items[i].id,cst->ordered_items[i].quantity,
                g->products[cst->ordered_items[i].id].quantity);
            }

            else{
                
                printf("Customer%d(%d,%d) success! Paid %.2lf dollars for each.\n"
                ,cst->id,cst->ordered_items[i].id,cst->ordered_items[i].quantity,
                g->products[cst->ordered_items[i].id].price);
                
                
                
                cst->balance=cst->balance-cost;

                g->products[cst->ordered_items[i].id].quantity=g->products[cst->ordered_items[i].id].quantity
                - cst->ordered_items[i].quantity;

                int k;
                for(k=0;k<MAX_PRODUCT_NUM;k++){
                    if(cst->purchased_items[k].id==-1){
                        cst->purchased_items[k].id=cst->ordered_items[i].id;
                        cst->purchased_items[k].quantity=cst->ordered_items[i].quantity;
                        break;
                    }
                }
                printf("Bought %d unit of products_%d for %.2lf dollars.\n"
                ,cst->purchased_items[k].quantity,
                cst->purchased_items[k].id,
                cost);
                

            }

                printf("\nUpdated products:\n");
                printf("Products ID    Quantity       Price\n");
                int n;
                for(n=0;n<MAX_PRODUCT_NUM;n++){
                    if(g->products[n].id!=-1){
                    printf("%-15d%-15d%-15.2lf\n",g->products[n].id,
                    g->products[n].quantity,g->products[n].price);
                    }
                }


            sem_post(&g->m_lock);

            

        }
    }
    
    sem_wait(&g->m_lock);
    printf("------------------------------\n");
    printf("Customer%d\n",cst->id);
    printf("initial balance: %lf\n",initial_balance);
    printf("updated balance: %lf\n",cst->balance);
    printf("Ordered products:\n");
    printf("id        quantity\n");
    for(i=0;i<MAX_PRODUCT_NUM;i++){
        if(cst->ordered_items[i].id!=-1)
        printf("%-10d%-10d\n",cst->ordered_items[i].id,cst->ordered_items[i].quantity);
        
    }
    printf("Purchased products:\n");
    printf("id        quantity\n");
    for(i=0;i<MAX_PRODUCT_NUM;i++){
        if(cst->purchased_items[i].id!=-1)
        printf("%-10d%-10d\n",cst->purchased_items[i].id,cst->purchased_items[i].quantity);
    }
    sem_post(&g->m_lock);

    
    return 0;
}


int main() {


    clock_t start, end;
    double execution_time;
    start = clock();



    unsigned int the_seed;
    printf("Enter an unsigned int for seed: ");
	scanf("%u", &the_seed);
	printf("This value you have just entered will determine the fate.\n");
    srand(the_seed);
    globals gl; // We are allocating the memory which we want to share
                //across processes. This variable contains the data structures
                //that our processes will use.
    void* shmem = mmap(NULL, sizeof(gl), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //Here we have created the memory to share across multiple processes.


    memcpy(shmem, &gl, sizeof(gl));
    //We are copying the data structures we have created before to shared
    //memory.
    
    initialize(shmem);
    //We are preparing our data structures in shared memory, for use. 


    int cst0,cst1,cst2;
    
    //Our shared memory pointers shmem is void* we should do type casting.
    globals * gg=(globals *)shmem;
    int i;
    for(i=0;i<MAX_CST_NUM;i++){
        printf("Customer_%d have %.2lf dollars.\n"
        ,gg->customers[i].id,gg->customers[i].balance);
        printf("Customer_%d wants to buy:\n",gg->customers[i].id);
        int j;
        for(j=0;j<MAX_PRODUCT_NUM;j++){
            if(gg->customers[i].ordered_items[j].id!=-1){
            printf("%d unit of product_%d\n",gg->customers[i].ordered_items[j].quantity
            ,gg->customers[i].ordered_items[j].id);
        }

        }
        printf("------------------------------\n\n");
    }


    cst0=fork();

    
    if(cst0==0){
        //Here we are in our first process. 

        gg->customers[0].pid=getpid();
        //Essentialy our customers are process. So here we assign them their id's.
        order(gg->customers+0,shmem);
        //And our process(customer) order its shopping list.
        //Note, order function uses shared memory to reach data structures
        //for shopping.
        return 0;
    }
    
    else{

        cst1=fork();
        
        if(cst1==0){
            gg->customers[1].pid=getpid();
            order(gg->customers+1,shmem);
            return 0;
        }
        
        else{
            
            cst2=fork();
            
            if(cst2==0){
                gg->customers[2].pid=getpid();
                order(gg->customers+2,shmem);
                return 0;
            }
            
            else{
                
            }
            
        }
        
    }
waitpid(cst0, NULL, 0);
waitpid(cst1, NULL, 0);
waitpid(cst2, NULL, 0);
//The parent process waiting for its children.
end = clock();

execution_time = ((double)(end - start))/CLOCKS_PER_SEC;

printf("\n\nExecution time of this program: %.9lf seconds\n",execution_time);
return 0;
}

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