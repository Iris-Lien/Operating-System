#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#define NUM_THREADS 5
#define MAX 10711225

struct timeval tp_s, tp_e;
struct timezone tzp_s, tzp_e;     

/* Each thread will begin control in this function */
void *runner(void *param)
{
    int i=0,j=0;
    long x;
    pthread_t tid= pthread_self(); //return  ID from pthread_create create of thread
    struct timeval tp_s1, tp_e1;
    struct timezone tzp_s1, tzp_e1;
    printf(" %ld created\n",tid); //print ID
    gettimeofday(&tp_s1,&tzp_s1); //time record before calculate   
	
	for (i=0;i<1000;i++)
    {                     
		for (j=0;j<MAX;j++)    
			x=(i+1)*j;      
		//printf("%ld\n",x);        
	}
	
    gettimeofday(&tp_e1,&tzp_e1);//time record after calculate
    printf("Execution time %ld diff=%ld\n",tid,tp_e1.tv_sec-tp_s1.tv_sec);//time start from each thread create and end at execution
    printf("Total time for %ld thread diff=%ld\n",tid,tp_e1.tv_sec-tp_s.tv_sec);// time start from original and end at execution
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int i, policy, scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;

	pthread_attr_init(&attr);//get the default attributes 

	//getscope
    if (pthread_attr_getscope(&attr, &scope )!= 0)
        fprintf(stderr, "Unable to get scheduling scope\n");
    else
	{
		if (scope == PTHREAD_SCOPE_PROCESS)
            printf("PTHREAD_SCOPE_PROCESS\n");
		else
		{
			if (scope == PTHREAD_SCOPE_SYSTEM)
				printf("PTHREAD_SCOPE_SYSTEM\n");
			else
				fprintf(stderr, "Illegal scope value.\n");
		}
        
		//setscope
        if(pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM) != 0)//set the pthread scope - process, system
            fprintf(stderr, "Unable to set scope.\n");
        else
            printf("Successful to set scope: PTHREAD_SCOPE_SYSTEM\n");
		
		//getschedpolicy
		if (pthread_attr_getschedpolicy(&attr, &policy) != 0) //get the current scheduling policy 
			fprintf(stderr, "Unable to get policy.\n"); 
		else 
		{ 
			if (policy == SCHED_OTHER) 
				printf("SCHED OTHER\n"); 
			else if (policy == SCHED_RR) 
				printf("SCHED RR\n"); 
			else if (policy == SCHED_FIFO) 
				printf("SCHED FIFO\n");
        }
		
		//setschedpolicy
		if(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0)//set the scheduling policy - FIFO, RR, or OTHER 
            fprintf(stderr, "Unable to set policy.\n");
        else
            printf("Successful to set policy: SCHED_FIFO\n");
			
		//gettime
		gettimeofday(&tp_s,&tzp_s);//record time before get into 5 times thread
		for (i=0;i<NUM_THREADS;i++)
		{
			if (pthread_create(&tid[i],&attr,runner,NULL)!=0)
				fprintf(stderr, "Unable to create a thread\n");
		}
		for (i=0;i<NUM_THREADS;i++)
			pthread_join(tid[i],NULL);//wait thread complete
		gettimeofday(&tp_e,&tzp_e);//record time after get into 5 times thread
		printf("Total time for all=%ld\n",tp_e.tv_sec-tp_s.tv_sec);//Get Total time after calculate
	}
}