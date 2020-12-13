/*
  Name: SockUtil.h
  Copyright: Jes�s Oliva 
  Author: Jes�s Oliva (jeoliva@telefonica.net)
  Date: 25/09/03 20:19
*/

#if !defined(SOCKUTIL_H)
#define SOCKUTIL_H

int inet_addr(const char *sIp, unsigned long *lIp);

unsigned short htons(unsigned short port);

char *IpToString(char *ip, unsigned long lIp);

#endif
