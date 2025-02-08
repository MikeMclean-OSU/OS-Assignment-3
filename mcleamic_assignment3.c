#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

struct movie{
    char* title;
    int year;
    struct movie *next; 
};

struct movie *createMovie(char* title, int year){
    
    struct movie *newMovie = malloc(sizeof(struct movie));
    newMovie->title = calloc(strlen(title) + 1, sizeof(char));
    strcpy(newMovie->title, title);
    newMovie->year = year;
    newMovie->next = NULL;

    return newMovie;
}

int processMovieFile(char* filePath, struct movie **head, struct movie **tail){
    char *currLine = NULL;
    size_t len = 0;
    char *title;
    char *yearString;
    int year;

    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    getline(&currLine, &len, movieFile);

    // Read the file line by line
    while(getline(&currLine, &len, movieFile) != -1)
    {
        title = strtok(currLine, ",");
        yearString = strtok(NULL, ",");
        year = atoi(yearString);
        
        struct movie *newMovie = createMovie(title, year);

        if(*head == NULL){
            *head = newMovie;
            *tail = newMovie;
        } else{
            (*tail)->next = newMovie;
            *tail = newMovie;
        }

    }
}


int is_valid_file(char *filename){
    size_t len = strlen(filename);
    return strcmp(filename + len - 4, ".csv") == 0 && strcmp(filename + 7, "movies_");
}

char *make_dir_name(){
    char *new_dir_name = malloc(30);
    strcpy(new_dir_name, "mcleamic.movies.");
    int dir_number = rand() % 99999;
    char str_dir_number[6];
    sprintf(str_dir_number, "%d", dir_number);
    strcat(new_dir_name, str_dir_number);
    return new_dir_name;
}

int main(){

    int choice1;
    int choice2;
    int size = 0;
    DIR* currDir;
    struct dirent *entry;
    char *parsefile = NULL;
    struct stat dirStat;
    struct movie* head = NULL;
    struct movie* tail = NULL;

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
                        free(parsefile);
                        parsefile = strdup(entry->d_name);
                        size = dirStat.st_size;
                    }
                }

                //Make directory name
                char *dir_name = make_dir_name();
                
                //Make directory
                int new_dir = mkdir(dir_name, 0750);
                
                closedir(currDir);
                //Parse file into movie structure
                processMovieFile(parsefile, &head, &tail);

                struct movie* node = head;

                //Open directory and create files
                char dir_start[45] = "./";
                sprintf(dir_start + strlen(dir_start), "%s", dir_name);
                currDir = opendir(dir_start);
                int file;
                char yearString[5];
                char path[100];

                while (node != NULL){
                    sprintf(yearString, "%d", node->year);
                    snprintf(path, sizeof(path), "%s/%s.txt", dir_start, yearString);
                    file = open(path, O_RDWR | O_CREAT | O_APPEND, 0640);
                    int write_bytes = write(file, node->title, strlen(node->title));
                    write_bytes = write(file, "\n", 1);
                    close(file);
                    node = node->next; 
                }
                closedir(currDir);
                //Reset variables
                head = NULL;
                tail = NULL;
                free(dir_name);
                size = 0;
                printf("\n\n");
                break;
            case 2:
                
                
                
                break;
            case 3:
                
                
                
                break;
        }
    
    }else if (choice1 != 2){
        
        printf("You entered an incorrect choice. Try again.\n\n");
    
    }

    }while(choice1 != 2);

    return 0;
}