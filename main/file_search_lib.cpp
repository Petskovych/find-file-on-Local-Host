#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include "file_search_lib.h"

#define MAX_THREADS 8
#define MAX_PATH_LEN 4096
#define MAX_FILENAME_LEN 256

typedef struct {
    char filename[MAX_FILENAME_LEN];
    char start_path[MAX_PATH_LEN];
} SearchParams;

typedef struct {
    pthread_t tid;
    int found;
} ThreadInfo;

void* search_file(void* args) {
    SearchParams* params = (SearchParams*)args;
    ThreadInfo* thread_info = (ThreadInfo*)malloc(sizeof(ThreadInfo));
    if (thread_info == NULL) {
        perror("malloc");
        pthread_exit(NULL);
    }
    thread_info->found = 0;

    DIR* dir = opendir(params->start_path);
    if (!dir) {
        perror("opendir");
        free(thread_info);
        pthread_exit(NULL);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH_LEN];
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", params->start_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            SearchParams* sub_params = (SearchParams*)malloc(sizeof(SearchParams));
            if (sub_params == NULL) {
                perror("malloc");
                free(thread_info);
                pthread_exit(NULL);
            }
            strcpy_s(sub_params->filename, params->filename);
            strcpy_s(sub_params->start_path, full_path);
            search_file(sub_params);
            free(sub_params);
        }
        else if (strcmp(entry->d_name, params->filename) == 0) {
            printf("Found file: %s\n", full_path); // Виведення шляху файлу
            thread_info->found = 1;
            closedir(dir);
            pthread_exit(thread_info); // Повертаємо thread_info як результат
            return NULL;
        }
    }

    closedir(dir);

    pthread_exit(thread_info); // Повертаємо thread_info, навіть якщо файл не знайдено
}

void start_search(const char* filename, const char* start_path) {
    printf("Searching for file: %s in directory: %s\n", filename, start_path);
    pthread_t threads[MAX_THREADS];
    ThreadInfo* thread_info[MAX_THREADS] = { 0 }; // Ініціалізуємо масив нульовими вказівниками
    int num_threads = 0;

    DIR* root_dir = opendir(start_path);
    if (!root_dir) {
        perror("opendir");
        return;
    }
    closedir(root_dir);

    struct dirent* entry;
    DIR* dir = opendir(start_path);
    if (!dir) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH_LEN];
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", start_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (num_threads >= MAX_THREADS)
                break;

            // Створення нового екземпляру SearchParams для кожного потоку
            SearchParams* params = (SearchParams*)malloc(sizeof(SearchParams));
            if (params == NULL) {
                perror("malloc");
                break;
            }
            strcpy_s(params->filename, filename);
            strcpy_s(params->start_path, full_path);

            // Передача відповідних параметрів кожному потоку
            thread_info[num_threads] = (ThreadInfo*)malloc(sizeof(ThreadInfo));
            if (thread_info[num_threads] == NULL) {
                perror("malloc");
                free(params);
                break;
            }
            pthread_create(&threads[num_threads], NULL, search_file, params);
            num_threads++;
        }
    }
    closedir(dir);

    for (int i = 0; i < num_threads; i++) {
        void* thread_result;
        pthread_join(threads[i], &thread_result);
        ThreadInfo* info = (ThreadInfo*)thread_result;
        if (info != NULL) {
            if (info->found) {
                printf("Search completed.\n");
                free(info);
                break;
            }
            else {
                printf("File not found: %s\n", filename);
            }
            free(info);
        }
    }
}
