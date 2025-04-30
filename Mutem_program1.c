//this program to understand various lock system
//pthrad_mutex_init()-to initialize the mutex
//pthread_mutex_lock(& mutex)-to acquire lock.
//pthread_mutex_unlock(&mutex)- to release lock.
//pthread_mutex_trylock(&mutex)-to try lock special way to lock if lock is available.
//pthread_mutex_attr_init()-
//pthread_mutex_attr_destroy()-
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
// function to print  value of shared variable  5 times and increment each time
// after completion resset it to default.
static int shared_variable=0;
static int shared_variable2=20;
//this function to illustrate pthread_mutex_lock and unlock function.
void *start1(void *arg)
{
    printf("starting  of thread.... \n");
    pthread_mutexattr_t atr;
    pthread_mutexattr_init(&atr);
    pthread_mutexattr_settype(&atr,PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_t mutex; // creating mutex pointer
    pthread_mutex_init(&mutex,&atr);
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);
    int temp=shared_variable;
    for(int i=0;i<5;i++)
    {
        printf(" %d ",shared_variable);
        shared_variable++;
    }
    printf("\n");
    shared_variable=temp;
   pthread_mutexattr_destroy(&atr);
    pthread_mutex_unlock(&mutex);
}
//this function to illustrate pthread_mutex_trylock.
// if there is use of two mutex in any order such that one aquired lock of 
// first mutex and other acquired lock of other mutex
//next both want to acquired the mutex which 
void *start2(void *arg)
{
    printf("starting  of thread.... \n");
    // setting attribute 
    pthread_mutexattr_t atr;
    pthread_mutexattr_init(&atr);
    pthread_mutexattr_settype(&atr,PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_t mutex1, mutex2; // creating mutex pointer
    pthread_mutex_init(&mutex1, &atr);
    pthread_mutex_init(&mutex2, &atr);
    if(shared_variable2==20)
    {
        int temp=shared_variable2;
        shared_variable2=25;
       if(pthread_mutex_trylock(&mutex1)==0)
       { 
       for(int i=0;i<5;i++)
        {
        printf(" %d ",temp);
        }
        printf("\n");
        }
       if(pthread_mutex_trylock(&mutex2)==0)
       {
        //printing again 
        printf("printing again \n");
        for(int i=0;i<5;i++)
        {
        printf(" %d ",temp);
        }
        printf("\n");
       }
       pthread_mutexattr_destroy(&atr);
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
    else if(shared_variable2==25)
    {
        int temp=shared_variable2;
        shared_variable2=20;
       if( pthread_mutex_trylock(&mutex2)==0)
       {
        for(int i=0;i<5;i++)
        {
        printf(" %d ",temp);
        }
        printf("\n");
        }
        if(pthread_mutex_trylock(&mutex1)==0)
        {
        //printing again 
        printf("printing again \n");
        for(int i=0;i<5;i++)
        {
        printf(" %d ",temp);
        }
        printf("\n");
        }
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
    }
}
//creating the main function
int main()
{
    pthread_t t1,t2;
    pthread_create(&t1,NULL,&start1,NULL);
    pthread_create(&t2,NULL,&start1,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    //trying trylock 
    pthread_t t3,t4;
    pthread_create(&t3,NULL,&start2,NULL);
    pthread_create(&t4,NULL,&start2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    return 0;
}
/* Attribute	            Description
PTHREAD_MUTEX_NORMAL	    Default type; deadlock if re-locked by same thread.
PTHREAD_MUTEX_RECURSIVE	    Allows same thread to lock multiple times without deadlock (must unlock the same number of times).
PTHREAD_MUTEX_ERRORCHECK	Returns error on double-locking by same thread instead of deadlocking.
PTHREAD_PROCESS_SHARED	    Allows the mutex to be shared between processes (not just threads).
PTHREAD_PROCESS_PRIVATE	    Default; mutex is usable only by threads in the same process.*/
