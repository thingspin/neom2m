/**
 * @file    netool.c
 * @date   	2013-04-24
 * @author 	lonycell@gmail.com
 * @brief  	Classes that help with network access, beyond the normal MDS.net.* APIs.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ether.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "netool.h"

/*===========================================================================================*/
int MNet_pppOpen(void)
{
	int counter = 3;
	int ret = 0;
	int check_ppp0 = 3;

	do {
		if(MNet_getState() > 0) {
			ret = 1;
			break;
		}

		ret = system("pppd unit 0 /dev/smd0");

		sleep(3);

	} while (counter-- > 0);

	
	// 여기 까지 왔으면 기존 루틴상의  네트워크 연결은 문제가없다고 판단된다.
	if (ret)
	{
		FILE *fp;
		int found = 0;

		char returnData[64];

		sleep(2);

		// 일단 ifconfig 명령어를 통해서 실제로 활성화된 네트워크 인터페이스를 찾는다.
		fp = popen("/sbin/ifconfig", "r");

		while (fgets(returnData, 64, fp) != NULL)
		{
			if ( strstr(returnData,"ppp0") ) // found PPP0 interface
				found = 1;
		}

		pclose(fp);

		// 인터페이스를 찾았는지 체크
		if (found)
		{
			ret = 1;
		}
		else
		{
			printf("<webdm> Network check routine  : Cannot Find PPP0 Network!!!!!!!!!! kill pppd process. \r\n");

			system(CMD_PPP_DOWN);
			sleep(1);
			ret = 0;
		}

	}


	return ret;
}

/*===========================================================================================*/
int MNet_pppClose(void)
{
	int ret = 0;

	ret = system("killall pppd");

	return ret;
}

/*===========================================================================================*/
int MNet_getState(void)
{
	int ret = 0;
	int	sock;
	struct ifreq ifr;

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock < 0) {
		return ret;
	}

	sprintf((char *)&ifr.ifr_name, "%s", "ppp0");

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
		ret = 0;
	} else {
		ret = 1;
	}

	close(sock);

	return ret;
}

int ip_check(char *ip)
{
    if(strcmp(ip, " ") == 0)
    {
        //printf("IP address NULL!\n");
        return 0;
    }

    int len = strlen(ip);

    if( len > 15 || len < 7 )
        return 0;

    int nNumCount = 0;
    int nDotCount = 0;
    int i = 0;

    for( i=0; i<len; i++)
    {
        if(ip[i] < '0' || ip[i] > '9')
        {
            if(ip[i] == '.')
            {
                ++nDotCount;
                nNumCount = 0;
            }
            else
                return 0;
        }
        else
        {
            if(++nNumCount > 3)
                return 0;
        }
    }

    if(nDotCount != 3)
        return 0;

    return 1;
}

int MNet_getDomainIP(char* domain, char* ip)
{
    struct hostent *host_entry;
    int             ndx;

    host_entry = gethostbyname(domain);

    if ( !host_entry)
    {
        //printf( "gethostbyname() fail/n");
        return 0;
    }
    
    //for ( ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++)
    sprintf( ip, "%s", inet_ntoa( *(struct in_addr*)host_entry->h_addr_list[0]));
    
    return ip_check(ip);

}

/*===========================================================================================*/
int MNet_getIP(char* netif, char* str)
{
	int fd;
	struct ifreq ifr;
	int res = 0;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;

	strncpy(ifr.ifr_name, netif, IFNAMSIZ-1);

	res = ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	sprintf(str, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	return res;
}

/*===========================================================================================*/
/*===========================================================================================*/
