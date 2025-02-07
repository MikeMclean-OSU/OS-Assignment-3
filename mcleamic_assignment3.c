#include <stdio.h>


int main(){

    int choice1;
    int choice2;

    do{
    printf("Please select one of the following options\n\n"
            "1. Select file to process\n"
            "2. Exit the program\n\n"
            "Enter a choice 1 or 2: ");
    scanf("%d", &choice1);

    if(choice1 == 1){

        printf("Which file do you want to process?\n"
            "Enter 1 to pick the largest file\n"
            "Enter 2 to pick the smallest file\n"
            "Enter 3 to specify the name of a file\n"
            "Enter a choice from 1 to 3: ");
        scanf("%d", &choice2);

        switch(choice2){
            case 1:
                
                
                
                break;
            case 2:
                
                
                
                break;
            case 3:
                
                
                
                break;
        }
    
    }else if (choice1 != 2){
        
        printf("Invlaid selection, please try again\n\n");
    
    }

    }while(choice1 != 2);

    return 0;
}