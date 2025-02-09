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
void processMovieFile(char* filePath, struct movie **head, struct movie **tail){
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
    free(currLine);
    fclose(movieFile);
}


int is_valid_file(char *filename){
    size_t len = strlen(filename);
    return strcmp(filename + len - 4, ".csv") == 0 && strcmp(filename + 7, "movies_");
}

char *make_dir_name(){
    char *new_dir_name = malloc(40);
    strcpy(new_dir_name, "mcleamic.movies.");
    int dir_number = rand() % 99999;
    snprintf(new_dir_name, 50, "mcleamic.movies.%d", dir_number);
    return new_dir_name;
}

void create_files(struct movie **head, char *dir_name){
    struct movie* node = *head;
    //Open directory and create files
    char dir_start[45] = "./";
    sprintf(dir_start + strlen(dir_start), "%s", dir_name);
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
    char* dir_name;
    int new_dir;
    int isin = 0;
    struct movie *temp;

    srand(time(NULL));

    do{
    printf(
            "1. Select file to process\n"
            "2. Exit the program\n\n"
            "Enter a choice 1 or 2: ");
    scanf("%d", &choice1);
    printf("\n");
    
    if(choice1 == 1){
        while(choice1){
            printf("Which file do you want to process?\n"
                "Enter 1 to pick the largest file\n"
                "Enter 2 to pick the smallest file\n"
                "Enter 3 to specify the name of a file\n\n"
                "Enter a choice from 1 to 3: ");
            scanf("%d", &choice2);

        
            if(choice2 == 1){
                
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
                
                printf("Now processing the chosen file named %s\n", parsefile);
                //Make directory name
                dir_name = make_dir_name();
                
                //Make directory
                new_dir = mkdir(dir_name, 0750);
                printf("Created directory with name %s\n", dir_name);
                closedir(currDir);

                //Parse file into movie structure
                processMovieFile(parsefile, &head, &tail);

                //create files in directory
                create_files(&head, dir_name);

                while (head != NULL) {
                    temp = head;
                    head = head->next;
                    free(temp->title);
                    free(temp);
                    }
                head = NULL;
                tail = NULL;
                free(dir_name);
                dir_name = NULL;
                if (parsefile){
                    free(parsefile);
                    parsefile = NULL;
                }
                size = 0;
                printf("\n\n");
                break;
            } else if(choice2 == 2){
                //Open current directory
                currDir = opendir(".");

                //Find smallest file in the directory
                while((entry = readdir(currDir)) != NULL){
                    stat(entry->d_name, &dirStat);
                    if (is_valid_file(entry->d_name) && size == 0){
                        free(parsefile);
                        parsefile = strdup(entry->d_name);
                        size = dirStat.st_size;
                    }
                    else if (is_valid_file(entry->d_name) && dirStat.st_size < size){
                        free(parsefile);
                        parsefile = strdup(entry->d_name);
                        size = dirStat.st_size;
                    }
                }

                printf("Now processing the chosen file named %s\n", parsefile);
                //Make directory name
                dir_name = make_dir_name();
                
                //Make directory
                new_dir = mkdir(dir_name, 0750);
                printf("Created directory with name %s\n", dir_name);
                closedir(currDir);
                //Parse file into movie structure
                processMovieFile(parsefile, &head, &tail);

                //create files in directory
                create_files(&head, dir_name);
                while (head != NULL) {
                    temp = head;
                    head = head->next;
                    free(temp->title);
                    free(temp);
                    }
                head = NULL;
                tail = NULL;
                free(dir_name);
                dir_name = NULL;
                if (parsefile){
                    free(parsefile);
                    parsefile = NULL;
                }
                size = 0;
                printf("\n\n");
                break;
            }else if(choice2 == 3){
                //Allocate memory to parsefile and ask user for file
                parsefile = malloc(256);
                printf("Enter the complete file name: ");
                scanf("%s", parsefile);

                currDir = opendir(".");

                while((entry = readdir(currDir)) != NULL){
                    stat(entry->d_name, &dirStat);
                    if (strcmp(entry->d_name, parsefile) == 0){
                        printf("Now processing the chosen file named %s\n", parsefile);

                        //get dir_name
                        dir_name = make_dir_name();
                
                        //Make directory
                        new_dir = mkdir(dir_name, 0750);
                        printf("Created directory with name %s\n", dir_name);
                        processMovieFile(parsefile, &head, &tail);

                        //create files in directory
                        create_files(&head, dir_name);

                        isin = 1;
                        break;
                    }
                }
                closedir(currDir);
                while (head != NULL) {
                    temp = head;
                    head = head->next;
                    free(temp->title);
                    free(temp);
                    }
                head = NULL;
                tail = NULL;
                if(dir_name){
                    free(dir_name);
                    dir_name = NULL;
                }
                if (parsefile){
                    free(parsefile);
                    parsefile = NULL;
                }
                size = 0;
                printf("\n\n");
                
                if(isin == 1){
                    isin = 0;
                    break;
                }else{
                    printf("You entered an incorrect choice. Try again.\n\n");    
                    
                }
                
            }
   
    }
    }else if (choice1 != 2){
        
        printf("You entered an incorrect choice. Try again.\n\n");
    
    }

    }while(choice1 != 2);
    return 0;
}