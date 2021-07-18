#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int cust_id=1; //global variable to keep track of total number of customers who visited the restaurant
int tablecount=0; //keeps track of the number of tables occupied at any given instant
int nooftables=3; //total number of tables in the restaurant

//structure of the order of food items for each table
struct order
{
    int itemno;
    int qty;
    struct order *next;
};

typedef struct order *orderp;
//end of food order struct

//structure of waiting list queue and tables linked list
struct dinein 
{
    int cid;
    char cname[100];
    int tableno;
    int bill;
    orderp torder; 
    struct dinein * next; 
      
}*dinefront=NULL,*llhead = NULL,*dinerear=NULL;

//end of struct dine in

char items[7][30] = {
    "Dosa",
    "Idli",
    "Chapathi",
    "Veg Biriyani (Pulav)",
    "Fried Rice",
    "Onion Cheese Maggi",
    "Spl Milk Bun"
};

int price[7] = {40, 30, 35, 50, 60, 35, 15};

//function to create a new node of type dinein

struct dinein * create(){
    struct dinein *d;
    d=(struct dinein*)malloc(sizeof(struct dinein));
    d->next=NULL;
    return d;
}

//end of create()

void menu()
{   
    printf("\n\t\tMENU\n\t\t****\n");
    printf("ID\tItems\t\t\tPrice\n");
    printf("**\t*****\t\t\t*****\n");
    
    printf("%d\t%s\t\t\tRs %d \n", 1, items[0], price[0]);
    printf("%d\t%s\t\t\tRs %d \n", 2, items[1], price[1]);
    printf("%d\t%s\t\tRs %d \n", 3, items[2], price[2]);
    printf("%d\t%s\tRs %d \n", 4, items[3], price[3]);
    printf("%d\t%s\t\tRs %d \n", 5, items[4], price[4]);
    printf("%d\t%s\tRs %d \n", 6, items[5], price[5]);
    printf("%d\t%s\t\tRs %d \n", 7, items[6], price[6]);
}

void acceptorder()
{
    int tno;
    printf("\nAccept order\n************\nEnter table number: ");
    scanf("%d", &tno);
    menu();
    char choice;
    do{
        orderp newitem = (orderp)malloc(sizeof(struct order));
        newitem->next = NULL;

        int id, q;
        printf("\nEnter id of the item you want to order: ");
        scanf("%d", &id);

        printf("Enter quantity: ");
        scanf("%d", &q);

        newitem->itemno = id-1;
        newitem->qty = q;

        struct dinein *ptr=llhead->next;
        while(ptr->tableno != tno)
        {
            ptr = ptr->next;
        }
        if(ptr->torder == NULL){
            ptr->torder = newitem;
        }

        else  
        {
        orderp temp = ptr->torder;
        while(temp->next != NULL)
        {
            temp=temp->next;
        }
        temp->next = newitem;
        }

        printf("\nDo you want to order another item(y/n): ");
        scanf(" %c", &choice);

    }while(choice == 'y');
}



//enqueue and dequeue for waiting list
void enqueue(struct dinein * dinein_wait){
        if(dinefront==NULL && dinerear==NULL)
        {
            dinefront=dinerear=dinein_wait;
        }
        else
        {
           dinerear->next=dinein_wait;
           dinerear=dinein_wait; 
        }
        if(tablecount >= nooftables)
            printf("\nAdded to waiting list\n");
}

void dequeue()
{
    struct dinein *f = dinefront;
    if(dinefront==NULL)
    {
        printf("\nNo customer in waiting list\n");
    }
    else if(dinefront==dinerear)
    {
        dinefront=dinerear=NULL;
    }
    else{
        dinefront=f->next;
        printf("\n\n");
    }
}
//end of enqueue() and dequeue()

void bill(struct dinein *cust)
{
    printf("\n\n############## RARA'S KITCHEN - FOOD BILL ################ \n");
    printf("Customer ID: %d \nCustomer Name: %s\n", cust->cid, cust->cname);

    printf("\n");
    orderp ptr1 = cust->torder;
    cust->bill=0;
        while(ptr1 != NULL)
        {
            printf("%d x %s = %d \n",ptr1->qty, items[ptr1->itemno], ptr1->qty * price[ptr1->itemno]);
            cust->bill += ptr1->qty * price[ptr1->itemno];
            ptr1 = ptr1->next;
        }
    printf("\nYour bill amount: Rs. %d", cust->bill);
    printf("\n########################################################## \n");
}

//display waiting queue
void display()
{
    struct dinein * f=dinefront;
    if(f==NULL){
        printf("No one is in the waiting list ! :/\n");
    }
    else{
        printf("The wait list is\n");
        while(f!=NULL){
        puts(f->cname);
        f=f->next;
        }
    }
    getchar();
    getchar();
    printf("\n");
}
//end of display()

//display tables list
void displaytable()
{
    struct dinein *head = llhead->next;
    while(head!=NULL){
        printf("(%s) %d :\n", head->cname, head->tableno);
        orderp ptr1 = head->torder;
        while(ptr1 != NULL)
        {
            printf("%d x %s\n", ptr1->qty, items[ptr1->itemno]);
            ptr1 = ptr1->next;
        }
        printf("\n");
        head = head->next;
    }
    printf("\n");
}
//end of displaytable()

//allocates a table to a customer
void addanewtable(int tno, struct dinein *dinein_wait)
{
    if(llhead==NULL){       
        llhead = create();  //dummy node - pointing to the first node
    }
    
    struct dinein *temp = llhead;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next = dinein_wait;
    dinein_wait->next = NULL;
    dinein_wait->tableno = tno;
    tablecount++;
    printf("\n%s can now occupy table %d\n", dinein_wait->cname, dinein_wait->tableno);
    acceptorder();
}

//removes customer details after he/she checks out
void deletetable()
{
    int tno;
    printf("\nEnter the table number: ");
    scanf("%d",&tno);
    struct dinein *temp=llhead;
    while(temp->next!=NULL && (temp->next)->tableno!=tno)
    {
        temp=temp->next;
    }
    
    if(temp->next == NULL){
        printf("Invalid table number\n");
    }

    else {
        struct dinein *reqtable = temp->next;
        temp->next = reqtable->next;
        bill(reqtable);
        
        free(reqtable);
        --tablecount;
        if(dinefront != NULL)
        {
            struct dinein *new = dinefront;
            dequeue();
            addanewtable(tno, new); 
        }
        getchar();
        getchar();
    }
}

void acceptcust(struct dinein *dinein_wait)
{ 
    char name[100];
    printf("\nEnter your name: ");
    scanf(" %s", name);
    strcpy(dinein_wait->cname,name);
    dinein_wait->tableno=-1;
    dinein_wait->cid=cust_id;
    cust_id++;
    dinein_wait->torder=NULL;
    enqueue(dinein_wait);
    if(tablecount < nooftables){
        addanewtable(tablecount + 1, dinein_wait);
        dequeue();
    }
}
//end of acceptcust()


void dineinfunc()
{
    int choice;
    do{
    system("clear");
    printf("\n\n\t\t\t\t\tRARA's KITCHEN\n\t\t\t\t\t**************\n\n");
    printf("\nDine In ! \n\n1. Accept new customer\n2. Accept order\n3. Checkout/Print Bill\n4. Display customer details\n5. Back\nEnter a choice: ");
    scanf("%d",&choice);
    struct dinein* dinein_wait;

    switch(choice){
        case 1: dinein_wait = create();
                acceptcust(dinein_wait);
                break;
        case 2: acceptorder();
                break;
        case 3: deletetable();
                break; 
        case 4: displaytable();
                display();
                break;
        case 5: break;
    }
    }while(choice!=5);
}

//---------------------------------------------------//

void main()
{
    dineinfunc();
}
