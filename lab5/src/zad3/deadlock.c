#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *do_work_one();
void *do_work_two();

pthread_mutex_t first_mutex, second_mutex;

int main(){
    pthread_t tid_1, tid_2;
    pthread_attr_t attr_1, attr_2;

    pthread_mutex_init(&first_mutex, NULL);
    pthread_mutex_init(&second_mutex, NULL);

    pthread_create(&tid_1, NULL, do_work_one, NULL);

    pthread_create(&tid_2, NULL, do_work_two, NULL);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
}

void *do_work_one(){

    pthread_mutex_lock(&first_mutex);
    sleep(500);
    pthread_mutex_lock(&second_mutex);

    printf("Never got there\n");

    pthread_mutex_unlock(&first_mutex);
    pthread_mutex_unlock(&second_mutex);

    pthread_exit(0);
}

void *do_work_two(){

    pthread_mutex_lock(&second_mutex);

    pthread_mutex_lock(&first_mutex);

    printf("Never got there\n");

    pthread_mutex_unlock(&second_mutex);
    pthread_mutex_unlock(&first_mutex);

    pthread_exit(0);
}