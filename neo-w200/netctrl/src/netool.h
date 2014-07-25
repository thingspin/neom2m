/**
 * @file    netool.h
 * @date   	2013-04-24
 * @author 	lonycell@gmail.com
 * @brief  	Classes that help with network access, beyond the normal MDS.net.* APIs.
 */

#ifndef __LINUX_NETOOL_H
#define __LINUX_NETOOL_H

#define CMD_PPP_UP     "pppd /dev/ttyUSB4"
#define CMD_PPP_DOWN   "kill -9 `cat /var/run/ppp0.pid`"
#define NET_INTERFACE  "ppp0"
#define LISTEN_PORT		8888

int MNet_pppOpen(void);
int MNet_pppClose(void);
int MNet_getState(void);
int MNet_getIP(char* netif, char* str);
int MNet_getDomainIP(char* domain, char* ip);
int get_ip_from_udhcp_config(char* path,char* buf);

#endif
