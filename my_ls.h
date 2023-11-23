#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_options {
    bool a;
    bool t;
} options_t;

typedef struct s_dirs {
    char dir_name[255];
    bool loop;
    int dir_i;
    int num_of_loop;
} dirs_t;

typedef struct s_file_name {
    char name[255]; 
    long sec;
    long n_sec;
    int num_of_files;
    int num_of_dir;
} file_name_t;

file_name_t * sort_alphabetically(file_name_t * data);