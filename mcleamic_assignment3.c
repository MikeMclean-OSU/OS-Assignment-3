#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int is_valid_file(char *filename){
    size_t len = strlen(filename);
    return strcmp(filename + len - 4, ".csv") == 0 && strcmp(filename + 7, "movies_");
}

char *make_dir_name(){
    char dir_name[25] = "mcleamic.movies.";
    int dir_number = rand() % 99999;
    char str_dir_number[6];
    sprintf(str_dir_number, "%d", dir_number);
    strcat(dir_name, str_dir_number);
    return dir_name;
}

int main(){

    int choice1;
    int choice2;
    int size = 0;
    DIR* currDir;
    struct dirent *entry;
    struct dirent *parsefile;
    struct stat dirStat;

    srand(time(NULL));

    do{
    printf("Please select one of the following options\n"
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
        printf("\n");

        switch(choice2){
            case 1:
                
                //Open current directory
                currDir = opendir(".");

                //Find biggest file in the directory
                while((entry = readdir(currDir)) != NULL){
                    stat(entry->d_name, &dirStat);
                    if (is_valid_file(entry->d_name) && dirStat.st_size > size){
                        parsefile = entry;
                        size = dirStat.st_size;
                    }
                }

                //Make directory name
                char *dir_name = make_dir_name();
                
                //Make directory
                int new_dir = mkdir(dir_name, 0750);
                //
                free(dir_name);
                closedir(currDir);
                size = 0;
                printf("\n\n");
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