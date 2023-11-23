#include "my_ls.h"

options_t * get_options(char ** av)
{
    // malloc the return val
    options_t * ret = malloc(sizeof(options_t) + 1);

    // default bool false
    ret->a = false;
    ret->t = false;

    // -a -t -at -ta -....
    for (int i = 0, j; av[i]; i++)
    {
        if (av[i][0] == '-')
        {
            for (j = 1; av[i][j]; j++)
            {
                switch (av[i][j]) 
                {
                    case 'a':
                        ret->a = true;
                        break;
                    case 't':
                        ret->t = true;
                        break;
                    default:
                        printf("%s: invalid input!\ninvalid option: %c\n", av[i], av[i][j]);
                }
            }
        }
    }

    return ret;
}

// get dir
dirs_t * get_dir(int ac, char ** av, dirs_t * ret, char * last_dir)
{
    // check requested directory

    // dirs_t * ret = malloc(sizeof(dirs_t) + 1);

    strcpy(ret->dir_name, ".");

    // only gets the first dir it has given
    int index = ret->dir_i;
    while (av[index]) 
    {
        if (av[index][0] != '-' && av[index] != NULL)
        {
            strcpy(ret->dir_name, av[index]);
            break;
        }
        index++;
    }
    
    // strcpy(ret->dir_name, dir_name);

    // if there is more directories go for the loop
    if (index < ac-1 && av[index][0] != '-' && av[index] != NULL && strcmp(av[index], last_dir))
    {
        ret->loop = true;
        ret->num_of_loop++;
        // printf("index: %d, ac: %d\n", index, ac);
        // printf("more dir to check\n");
    }
    else ret->loop = false;

    ret->dir_i = index;

    // printf("is loop: %d\n", ret->loop);

    // printf("av 0: %s\n", av[0]);
    // printf("dir name: %s\n", ret->dir_name);

    return ret;
}

file_name_t * count_and_save_files(const char *dir_path)
{
    DIR *dir = opendir(dir_path);

    if (dir == NULL) {
        printf("my_ls: cannot access '%s': No such file or directory\n", dir_path);
        // perror("");
        exit(EXIT_FAILURE);
    }

    struct dirent* entity;
    entity = readdir(dir);

    // storing the names of the files 
    file_name_t *file_data = malloc(sizeof(file_name_t) * 300 + 1);
    int num_of_files = 0;
    file_name_t temp;
    int dir_num = 0;

    while (entity != NULL) 
    {   
        if ((entity->d_type == DT_REG || entity->d_type == DT_DIR)) // entity type = regular files! or directories
        { 
            strcpy(temp.name, entity->d_name);
        }

        if (entity->d_type == DT_DIR) dir_num++;

        entity = readdir(dir);

        file_data[num_of_files] = temp;
        num_of_files++;
    }

    file_data->num_of_files = num_of_files;
    file_data->num_of_dir = dir_num;

    // printf("file: %s\n", file_data->name);

    closedir(dir);

    return file_data;
}

file_name_t * sort_alphabetically(file_name_t * data) 
{
    char temp[100];
    int num_of_files = data->num_of_files;

    file_name_t * alpha_sorted= malloc(sizeof(file_name_t) * 100);

    char** arr = (char**)malloc(sizeof(char*) * num_of_files + 1);

    // malloc and copy the data into arr
    for (int i = 0; i < num_of_files; i++) 
    {
        arr[i] = (char*)malloc(sizeof(char) * 255 + 1);
        strcpy(arr[i], data[i].name);
    }

    // compare and sort
    int j = 0;
    while (j < num_of_files-1)
    {
        if (strcmp(arr[j], arr[j+1]) > 0) 
        {
            strcpy(temp, arr[j]);
            strcpy(arr[j], arr[j+1]);
            strcpy(arr[j+1], temp);
            j = 0;
        }
        else
            j++;
    }

    for (int i = 0; i < num_of_files; i++) 
    {
        strcpy(alpha_sorted[i].name, arr[i]);
        free(arr[i]);
    }
    free(arr);

    alpha_sorted->num_of_files = num_of_files;

    return alpha_sorted;
}

// lstat
file_name_t * get_time_data(file_name_t * data, char * dir_name)
{
    // get the file names
    file_name_t * time_data= malloc(sizeof(file_name_t) * 100);
    
    for (int i = 0; i < data->num_of_files; i++) 
    {
        strcpy(time_data[i].name, data[i].name);
    }

    // check for each one of the files time
    struct stat fileInfo;
    for (int i = 0; i < data->num_of_files; i++)
    {
        char *current_file = malloc(sizeof(char) * 100 + strlen(dir_name));
        strcpy(current_file, dir_name);


        strcat(current_file, "/");

        strcat(current_file, time_data[i].name);

        // printf("file name: %s\n\n", current_file);

        lstat(current_file, &fileInfo);
        
        // WHEN THE FILE DATA IS FROM A DIR; 
        // IT READS THE WRONG DATA
        // SEC AND N_SEC JUST REPEATS ITSELF MEANINGLESSLY

        // save it as long n_sec
        // You want to use st_mtim
        time_data[i].n_sec = (long)fileInfo.st_mtim.tv_nsec;
        time_data[i].sec = (long)fileInfo.st_mtim.tv_sec;

    }

    // for (int i = 0; i < data->num_of_files; i++)
    // {
    //     printf("%s\n\n", time_data[i].name);
    //     printf("%ld\n", time_data[i].sec);
    //     printf("%ld\n\n", time_data[i].n_sec);
    // }

    return time_data;
}

void swap(long* first, long* second)
{
    long temp = *first;
    *first = *second;
    *second = temp;
}

file_name_t * sort_by_time (file_name_t * data)
{
    int num_of_files = data->num_of_files;

    file_name_t * time_sorted= malloc(sizeof(file_name_t) * num_of_files + 1);

    // printf("%d\n", num_of_files);

    // save the values of data into a temp
    // compare the temp and save it to the time_sorted accordingly

    long arr[num_of_files+1];       // arr for sec
    long arr_n_sec[num_of_files+1]; // arr for n_sec

    for (int i = 0; i < num_of_files; i++)
    {
        arr[i] = data[i].sec;
        arr_n_sec[i] = data[i].n_sec;
        // printf("%ld", data[i].n_sec);
    }

    int i, j, max_ind;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < num_of_files - 1; i++) {
        // Find the max element in unsorted array
        max_ind = i;
        for (j = i + 1; j < num_of_files; j++)
            if (arr[j] > arr[max_ind])
                max_ind = j;
            else if (arr[j] == arr[max_ind] && (arr_n_sec[j] > arr_n_sec[max_ind]))
            {    
                max_ind = j;
                // printf("samemee\n");
            }

        swap(&arr[max_ind], &arr[i]);
        swap(&arr_n_sec[max_ind], &arr_n_sec[i]);
    }

    // check to see same file name wouldnt put twice

    // an array as num_of_files as zero
    // if i use on of the data.name i'll mark it with 1
    bool arr_name_check[num_of_files];
    for (int i = 0; i < num_of_files; i++)
        arr_name_check[i] = 0;

    i = 0;
    j = 0;
    while (i < num_of_files)
    {
        if ((arr[i] == data[j].sec) && (arr_n_sec[i] == data[j].n_sec) && (!arr_name_check[j]))
        {
            strcpy(time_sorted[i].name, data[j].name);
            time_sorted[i].sec = arr[i];
            time_sorted[i].n_sec = arr_n_sec[i];
            i++;
            arr_name_check[j] = 1;
            j = 0;
        }
        else j++;
    }

    // for (int i = 0; i < num_of_files; i++)
    // {
    //     printf("%s\n", time_sorted[i].name);
    //     printf("%ld\n", time_sorted[i].n_sec);
    //     printf("%ld\n", arr[i]);
    // }

    return time_sorted;
}

// write a funtion to get individual datas and print if asked
void print_data (char * dir_name, options_t * opts)
{
    // this will get the names and numbers of the files in to a file_name_t data
    file_name_t * file_data = count_and_save_files(dir_name);

    // alphabetically sorting and storing the list
    file_name_t * alpha_sorted  = sort_alphabetically(file_data);

    // if asked get time data
    file_name_t * time_data = get_time_data(file_data, dir_name);
    time_data->num_of_files = alpha_sorted->num_of_files;

    // sort by time
    file_name_t * time_sorted = sort_by_time(time_data);

    int num_of_files = file_data->num_of_files;

    for (int i = 0; i < num_of_files; i++)
    {
        // - -a -t -at 
        if (opts->a && opts->t)
            printf("%s\n", time_sorted[i].name);
        else if (opts->a && !opts->t)
            printf("%s\n", alpha_sorted[i].name);
        else if (opts->t && !opts->a && time_sorted[i].name[0] != '.')
            printf("%s\n", time_sorted[i].name);
        else if ((!opts->a && !opts->t) && alpha_sorted[i].name[0] != '.')
            printf("%s\n", alpha_sorted[i].name);
    }

    free(file_data);
    free(alpha_sorted);
    free(time_data);
    free(time_sorted);
}

char ** sort_dir(int ac, char ** dir_names)
{
    char temp[100];

    char** arr = (char**)malloc(sizeof(char*) * ac + 1);

    // strcpy(arr[0], dir_names[0]);

    // malloc and copy the data into arr
    for (int i = 0; i < ac; i++) 
    {
    arr[i] = (char*)malloc(sizeof(char) * 255 + 1);
    strcpy(arr[i], dir_names[i]);
    }

    // compare and sort
    int j = 1;
    while (j < ac-1)
    {
    if (strcmp(arr[j], arr[j+1]) > 0) 
    {
        strcpy(temp, arr[j]);
        strcpy(arr[j], arr[j+1]);
        strcpy(arr[j+1], temp);
        j = 1;
    }
    else
        j++;
    }

    return arr;
}

// check for the loop
void check_loop (int ac, char ** av, options_t * opts)
{
    // check requested directory
    dirs_t * ret = malloc(sizeof(dirs_t) + 1);
    ret->dir_i = 1;
    ret->num_of_loop = 0;

    char **  new_av = sort_dir(ac, av);

    // for (int i = 0; av[i]; i++)
    // {
    //     printf("before: %d, %s\n", i, av[i]);

    //     printf("after: %d, %s\n",i,  new_av[i]);
    // }

    dirs_t * dirs = get_dir(ac, av, ret, "");


    // if you need to loop thrugh the dirs, first recieve all dir names and sort. print one by one later
    // av[1] av[2] av[3] in alphabetical order
    // if av[!0] and av[i][0] != '-' and ac > 1 fix all the input

    if (dirs->loop && strcmp(new_av[dirs->dir_i], "./my_ls")) 
    {
        printf("%s:\n", new_av[dirs->dir_i]);
        
    }
     
    print_data(dirs->dir_name, opts);

    // after finishing displaying the first directory check if there is more directory to print
    while (dirs->loop)
    {
        dirs->dir_i++;
        dirs = get_dir(ac, new_av, dirs, new_av[dirs->dir_i-1]);
        printf("\n%s:\n", dirs->dir_name);
        print_data(dirs->dir_name, opts);
    }

    for (int i = 0 ; av[i]; i++) free(new_av[i]);
    free(new_av);
    free(dirs);
}

// ls dir1 dir2 uses 

int main(int ac, char **av)
{
    // check the input number
    if (ac < 1)
    {
        printf("invalid!");
        return -1;
    }
    
    // for (int i = 0 ; av[i]; i++)
    //     printf("%d, %s\n", i, av[i]);

    // check options
    options_t * opts = get_options(av);

    // check the loop, if there is no loop just print dir or go for the loop
    check_loop(ac, av, opts);
    
    free(opts); // Free the memory allocated for opts

    return 0;
}
