/*===========================================================================================*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

#include "netool.h"

/*===========================================================================================*/
#define THREAD_CONNMAN	0
#define THREAD_RECEIVER	1
#define THREAD_SENDER	2
#define THREAD_SENDER_LAN 3
#define THREAD_COUNT	1


#define CONFIG_NET_MONITOR_INTERVAL_SEC 30
#define CONFIG_NET_MAX_FAIL_CNT			10
static void *thread_connman(void *args);


int get_server_ip = 0;

/*===========================================================================================*/
int main(int argc, char** argv)
{
	int ret = 0;
    
	//if(daemonize > 0) 
	{
		pid_t pid;

		if( (pid = fork()) < 0) {
			exit(0);
		} else if(pid != 0) {
			exit(0);
		}

		signal(SIGHUP, SIG_IGN);


		setsid();
	}
   
   printf("net ctrl start \r\n");	

	int status = 0;
	int i = 0;

	pthread_t tid[THREAD_COUNT] = {0, };
    
	void* threads[THREAD_COUNT] = {
		thread_connman, 
	};
    
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr,131072);

	for( i = 0; i < THREAD_COUNT; i++) {
		if (pthread_create(&tid[i], &attr, threads[i], NULL) < 0) {
			exit(0);
		}
	}
    
	for( i = 0; i < THREAD_COUNT; i++) {
		if (pthread_join(tid[i], (void**)&status) < 0) {
			exit(0);
		}
	}

	return ret;
}

/*===========================================================================================*/
void *thread_connman(void *args)
{
	
    int polls = CONFIG_NET_MONITOR_INTERVAL_SEC;
    int timeout = CONFIG_NET_MAX_FAIL_CNT;

    
//    static int get_server_ip = 0;
    
	while(timeout > 0)
	{
		if(MNet_pppOpen() > 0) {
			polls = CONFIG_NET_MONITOR_INTERVAL_SEC; // 30초 마다 네트워크 체크
            
        
		} else {
			polls = CONFIG_NET_MONITOR_INTERVAL_SEC;
            timeout --;
		}

		sleep(polls);
	}


	sleep(1);
    system("poweroff");

	return 0;
}
