#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#define MAX_PRODUCT_NUM 5
#define MAX_CST_NUM 3

double double_rand ( double low, double high )
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

int int_rand ( int low, int high )
{
    return rand()%(high-low+1)+low;
}



//Here we define what product is, what atrributes is contains.
typedef struct __product {
    int id;
    double price;
    int quantity;
} product;


//What does a costomer intent to buy?
typedef struct __ordered_item{
    int id;
    int quantity;
} ordered_item;


//This struct is for purchased_item, it tells out which and how many item
//did customer get at the end of the shopping.
typedef struct __purchased_item {
    int id;
    int quantity;
} purchased_item;


//Each thread is associated with a customer, so a customer should
//know the thread's id thus tid. Rest is like described in pdf.
typedef struct __customer {
    int id;
    double balance;
    pthread_t tid;
    ordered_item ordered_items[MAX_PRODUCT_NUM];
    purchased_item purchased_items[MAX_PRODUCT_NUM];
} customer;


//Here is our main players in the program.
//Products to buy, customers to spend their money.
product products[MAX_PRODUCT_NUM];
customer customers[MAX_CST_NUM];
pthread_mutex_t lock;




//With this function, we are initializing our data structures thus we can know
//what to expect in an untouch data structures so we can fill them.
void initialize(){
    int i;

    for(i=0;i<MAX_PRODUCT_NUM;i++){
        products[i].id=i;
        products[i].price=double_rand(1,200);
        products[i].quantity=int_rand(1,10);
    }

    for(i=0;i<MAX_CST_NUM;i++){
        customers[i].id=i;
        customers[i].tid=0;
        customers[i].balance=double_rand(100,1000);
        int j;
        for(j=0;j<MAX_PRODUCT_NUM;j++){
            //-1 means that this slot is not used.
            customers[i].ordered_items[j].id=-1;
            customers[i].ordered_items[j].quantity=-1;
            customers[i].purchased_items[j].id=-1;
            customers[i].purchased_items[j].quantity=-1;
        }
    }

    //ordered_items is generated random too.
    for(i=0;i<MAX_CST_NUM;i++){
        int ordered_num=int_rand(0,5);
        //printf("ordered_num=%d,  %d,  %d\n",ordered_num,int_rand(0,5),int_rand(0,5));
        int j;
        for(j=0;j<ordered_num;j++){
            int iid=int_rand(0,4);
            int quant=int_rand(1,3);
            int k;
            int exist_before=0;
            for(k=0;k<j;k++){
                if(customers[i].ordered_items[k].id==iid){
                    customers[i].ordered_items[k].quantity+=quant;
                    //According to pdf, we cannot buy more than 5 products.
                    if(customers[i].ordered_items[k].quantity>5)
                        customers[i].ordered_items[k].quantity=5;
                    exist_before=1;
                    break;
                }
            }
            if(exist_before==0){
                customers[i].ordered_items[j].id=iid;
                customers[i].ordered_items[j].quantity=quant;
            }
        }
    }



}



//We just need customer as an argument so we can easliy reach his/her
//ordered_items list.
void * order(void * arg){


    customer * cst= (customer *) arg;
    int i;
    double initial_balance=cst->balance;
    for(i=0;i<MAX_PRODUCT_NUM;i++){
        if(cst->ordered_items[i].id!=-1){
            //We are in critical section here, we should be careful.
            pthread_mutex_lock(&lock);

            //printing first half
            printf("Customer_%d:\n",cst->id);
            printf("Initial products:\n");
            printf("Products ID    Quantity       Price\n");
            int t;
            for(t=0;t<MAX_PRODUCT_NUM;t++){
                if(products[t].id!=-1){
                printf("%-15d%-15d%-15.2lf\n",products[t].id,
                products[t].quantity,products[t].price);
                }
            }

            //Transaction has not happened yet. We are thinking here what
            //would happen if we buy this things even it is impossible.
            double cost=cst->ordered_items[i].quantity*products[cst->ordered_items[i].id].price;
            double remaining_balance=cst->balance-cost;
            double remaining_products=products[cst->ordered_items[i].id].quantity
            -cst->ordered_items[i].quantity;
            
            //Well, if we have bought it, we would get a negative balance.
            //Impossible!
            if(remaining_balance<0){
                printf("Customer%d(%d,%d) fail! You are too poor! Ha ha:D\n "
                ,cst->id,cst->ordered_items[i].id,cst->ordered_items[i].quantity);
            }
            //There cannot be negative products remained!
            else if(remaining_products<0){
                printf("Customer%d(%d,%d) fail! Only %d left in stock.\n "
                ,cst->id,cst->ordered_items[i].id,cst->ordered_items[i].quantity,
                products[cst->ordered_items[i].id].quantity);
            }

            //Everything seems okay. We can finally buy, let's update
            //necessary data structures.
            else{
                
                printf("Customer%d(%d,%d) success! Paid %.2lf dollars for each.\n"
                ,cst->id,cst->ordered_items[i].id,cst->ordered_items[i].quantity,
                products[cst->ordered_items[i].id].price);
                
                
                //balance updated
                cst->balance=cst->balance-cost;
                //product number updated
                products[cst->ordered_items[i].id].quantity=products[cst->ordered_items[i].id].quantity
                - cst->ordered_items[i].quantity;

                //We are adding the successfull order to purchased_items
                //list.
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
                //What has remained after all the things I bought?
                printf("\nUpdated products:\n");
                printf("Products ID    Quantity       Price\n");
                int n;
                for(n=0;n<MAX_PRODUCT_NUM;n++){
                    if(products[n].id!=-1){
                    printf("%-15d%-15d%-15.2lf\n",products[n].id,
                    products[n].quantity,products[n].price);
                    }
                }


            pthread_mutex_unlock(&lock);
            //This lock was for each purchase.
            

        }
    }
    //This lock here is for end of the shopping process.
    //We are assessing here what things we could buy, and could not.
    pthread_mutex_lock(&lock);
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
    pthread_mutex_unlock(&lock);

    
    return 0;
}

int main(){
    
    clock_t start, end;
    double execution_time;
    start = clock();



    //We are determining the fate of our innocent customers here.
    unsigned int the_seed;
    printf("Enter an unsigned int for seed: ");
	scanf("%u", &the_seed);
	printf("This value you have just entered will determine the fate.\n");
    srand(the_seed);
    
    
    pthread_mutex_init(&lock, NULL); //Bunu sanırım hemen alt satirdaki initialize() fonksiyonuna dahil edebilirdim.
    initialize(); //Our customers and products are ready.
    
    //What does our customers intend to do?
    int i;
    for(i=0;i<MAX_CST_NUM;i++){
        printf("Customer_%d have %.2lf dollars.\n"
        ,customers[i].id,customers[i].balance);
        printf("Customer_%d wants to buy:\n",customers[i].id);
        int j;
        for(j=0;j<MAX_PRODUCT_NUM;j++){
            if(customers[i].ordered_items[j].id!=-1){
            printf("%d unit of product_%d\n",customers[i].ordered_items[j].quantity
            ,customers[i].ordered_items[j].id);
        }

        }
        printf("------------------------------\n\n");
    }

    pthread_create(&(customers[0].tid), NULL, &order, (customers+0));
    pthread_create(&(customers[1].tid), NULL, &order, (customers+1));
    pthread_create(&(customers[2].tid), NULL, &order, (customers+2));
    
    pthread_join(customers[0].tid, NULL);
    pthread_join(customers[1].tid, NULL);
    pthread_join(customers[2].tid, NULL);



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