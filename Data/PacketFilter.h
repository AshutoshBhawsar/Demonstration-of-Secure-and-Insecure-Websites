/*
  Name: PacketFilter.h
  Copyright: Jesús Oliva 
  Author: Jesús Oliva (jeoliva@telefonica.net)
  Date: 25/09/03 20:19
  Description: Type declarations.
*/

#if !defined(AFX_PACKETFILTER_H__B0DCFC2C_08A6_469A_B9B2_BC088FA72DBB__INCLUDED_)
#define AFX_PACKETFILTER_H__B0DCFC2C_08A6_469A_B9B2_BC088FA72DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>
#include <afxtempl.h>
#include <Iphlpapi.h>
#include <Fltdefs.h>

#pragma comment(lib, "Iphlpapi.lib")

// Values for filter rules
#define ANY_DIRECTION	 0x0
#define IN_DIRECTION	 0x1
#define OUT_DIRECTION	 0x2


#define ANY_ADDRESS		 "0.0.0.0"
#define ANY_TCPUDP_PORT  (WORD)0x0000
#define ANY_ICMP_TYPE	 (BYTE)0xff
#define ANY_ICMP_CODE	 (BYTE)0xff

// Protocol Values
#define PROTO(ProtoId)   MAKELONG(MAKEWORD((ProtoId),0x00),0x00000)
#define ANY_PROTOCOL	 PROTO(0x00)
#define TCP_PROTOCOL	 PROTO(0x06)
#define UDP_PROTOCOL	 PROTO(0x17)
#define ICMP_PROTOCOL	 PROTO(0x01)


// Global Filter Constants
#define GF_FRAGMENTS	 2
#define GF_STRONGHOST	 8
#define GF_FRAGCACHE	 9



// Pass an ip address in string format to unsigned long
unsigned long CharToIp(const char *sIp);


/***********************************************
Class CPacketFilterInterface. 

Class to create and bound interfaces with local ips.
***********************************************/

class CPacketFilterInterface  
{
public:
	CPacketFilterInterface();
	~CPacketFilterInterface();


public:
	int Create(unsigned long ip, PFFORWARD_ACTION defaultAction);
	INTERFACE_HANDLE hInterface;

};


// HashTable to relation ips with interfaces created. I only create the needed interfaces handles.
typedef CMap<unsigned long, unsigned long, CPacketFilterInterface, CPacketFilterInterface> MapIpToInterface;



/***********************************************
Class CPacketFilterInterface. 

Class for add filters to local interfaces.
***********************************************/

class CPacketFilter
{
public:
	CPacketFilter();
	~CPacketFilter();


public:

	void RemoveAll();
	int AddFilter(char	*localInterfaceIp, 
				  int	direction, 
				  char	*srcIp, 
				  char	*srcMask, 
				  char	*dstIp, 
				  char	*dstMask, 
				  int	srcPort, 
				  int	dstPort, 
				  int	protocol);

	int AddFilter(unsigned long localInterfaceIp, 
				  int			direction, 
				  unsigned long	srcIp, 
				  unsigned long	srcMask, 
				  unsigned long dstIp, 
				  unsigned long dstMask, 
				  int			srcPort, 
				  int			dstPort, 
				  int			protocol);

	int RemoveFilter(char	*localInterfaceIp, 
				     int	direction, 
					 char	*srcIp, 
					 char	*srcMask, 
					 char	*dstIp, 
					 char	*dstMask, 
					 int	srcPort, 
					 int	dstPort, 
					 int	protocol);

	int RemoveFilter(unsigned long	localInterfaceIp, 
			 		 int			direction, 
					 unsigned long	srcIp, 			
					 unsigned long	srcMask, 
					 unsigned long	dstIp, 
					 unsigned long	dstMask, 
					 int			srcPort, 
					 int			dstPort, 
					 int			protocol);

	int AddGlobalFilter(char *localInterfaceIp,
						int globalFilter);

	int RemoveGlobalFilter(char *localInterfaceIp, 
						   int globalFilter);


	void SetDefaultAction(PFFORWARD_ACTION action);

	PFFORWARD_ACTION GetDefaultAction();

	MapIpToInterface interfacesTable;

	

protected:
	PFFORWARD_ACTION defaultAction;

};



#endif // !defined(AFX_PACKETFILTER_H__B0DCFC2C_08A6_469A_B9B2_BC088FA72DBB__INCLUDED_)
