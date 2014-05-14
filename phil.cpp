#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int n; //количество философов
pthread_mutex_t forks[100];

typedef struct Phil {
    pthread_t thread;
    pthread_mutex_t *left, *right;
    int name, l, r;
} Ph;

void *routine( void* a ) {
    Ph *current = (Ph *) a;
    while ( pthread_mutex_trylock( current->left ) != 0 ) {
        sleep(rand() % 3 + 1);
        printf( "Phil %d tried to take his left fork\n", current->name );
    }
    while ( pthread_mutex_trylock( current->right ) != 0 ) {
        sleep(rand() % 3 + 1);
        printf( "Phil %d tried to take his right fork\n", current->name );
    }
    printf( "Phil %d is eating \n", current->name );
    sleep(rand() % 3 + 1);
    pthread_mutex_unlock( current->left );
    pthread_mutex_unlock( current->right );
    printf( "Phil %d dropped his forks\n", current->name );
    return 0;
}

void algorithm() {
    Ph philosophers[n];
    Ph *current;
    for (int  i = 0; i < n; i++ ) {
        current = &philosophers[i];
        current->name = i;
        if ( rand() % 2 ) {
            current->left = &forks[i];
            current->right = &forks[ (i + 1) % n ];
            current->l = i;
            current->r = (i+1) % n;
        }
        else {
            current->right = &forks[i];
            current->left = &forks[ (i + 1) % n ];
            current->r = i;
            current->l = (i+1) % n;
        }
        pthread_create( &current->thread, NULL, routine, current );
    }

    for (int i = 0; i < n; i++ ) {
        current = &philosophers[i];
        pthread_join( current->thread, NULL );
    }

}


int main() {
    n = 100;
    algorithm();

    return 0;
}
