/*
  Name: rules.h
  Copyright: Jesús Oliva 
  Author: Jesús Oliva (jeoliva@telefonica.net)
  Date: 25/09/03 20:19
*/

#if !defined _RULES_H
#define _RULES_H

typedef struct _RuleInfo
{
	unsigned long sourceIp;
	unsigned long sourceMask;
	unsigned short sourcePort;

	unsigned long destinationIp;
	unsigned long destinationMask;
	unsigned short destinationPort;

	unsigned int protocol;

	int action;
}RuleInfo, *PRuleInfo;

#endif