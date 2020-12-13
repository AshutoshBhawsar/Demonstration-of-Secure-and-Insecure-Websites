/*
  Name: PacketFilter.cpp
  Copyright: Jesús Oliva 
  Author: Jesús Oliva (jeoliva@telefonica.net)
  Date: 25/09/03 20:19
*/

#include "stdafx.h"
#include "PacketFilter.h"


#include <stdlib.h>
#include <stdio.h>



/***********************************************
Class CPacketFilter.
***********************************************/
CPacketFilter::CPacketFilter()
{
	defaultAction = PF_ACTION_FORWARD;
}

CPacketFilter::~CPacketFilter()
{
	RemoveAll();
}


void CPacketFilter::RemoveAll()
{
	POSITION pos = interfacesTable.GetStartPosition();

	CPacketFilterInterface pckInt;
	unsigned long ip;

	while( pos != NULL )
	{

		interfacesTable.GetNextAssoc(pos, ip, pckInt);

		PfDeleteInterface(pckInt.hInterface);   

	}

	interfacesTable.RemoveAll();
}


int CPacketFilter::AddFilter(char	*localInterfaceIp, 
							 int	direction, 
							 char	*srcIp, 
							 char	*srcMask, 
							 char	*dstIp, 
							 char	*dstMask, 
							 int	srcPort, 
							 int	dstPort, 
							 int	protocol)
{
	CPacketFilterInterface interfaceHandle;

	// If the interface isn't created, then I create it.
	if(!interfacesTable.Lookup(CharToIp(localInterfaceIp), interfaceHandle))
	{

		interfaceHandle.Create(CharToIp(localInterfaceIp), defaultAction);

		interfacesTable[CharToIp(localInterfaceIp)] = interfaceHandle;
	}


	// Fill the real filter struct
	PF_FILTER_DESCRIPTOR ipFlt;
	ipFlt.dwFilterFlags     = FD_FLAGS_NOSYN;
	ipFlt.dwRule            = 0;
	ipFlt.pfatType          = PF_IPV4;
	ipFlt.dwProtocol        = protocol;
	ipFlt.fLateBound        = 0;
	ipFlt.wSrcPort          = srcPort;
	ipFlt.wSrcPortHighRange = srcPort;
	ipFlt.wDstPort          = dstPort;
	ipFlt.wDstPortHighRange = dstPort;

	unsigned long lIpSrc    = CharToIp(srcIp);
	unsigned long lIpDst	= CharToIp(dstIp);
	unsigned long lMaskSrc	= CharToIp(srcMask);
	unsigned long lMaskDst	= CharToIp(dstMask);
				
	ipFlt.SrcAddr = (PBYTE) &lIpSrc;
	ipFlt.SrcMask = (PBYTE) &lMaskSrc;
	ipFlt.DstAddr = (PBYTE) &lIpDst;
	ipFlt.DstMask = (PBYTE) &lMaskDst;

	DWORD errorCode;

	// I add the filter
	if(direction == IN_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfAddFiltersToInterface(interfaceHandle.hInterface,
											1,
											&ipFlt,
											0,
											NULL,
											NULL);
	
	if(direction == OUT_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfAddFiltersToInterface(interfaceHandle.hInterface,
											0,
											NULL, 
											1,
											&ipFlt,
											NULL);

	else
		return -2;

	if(errorCode != NO_ERROR)
		return -1;

	return 0;
}



int CPacketFilter::AddFilter(unsigned long  localInterfaceIp, 
							 int			direction, 
							 unsigned long	srcIp, 
							 unsigned long	srcMask, 
							 unsigned long 	dstIp, 
							 unsigned long 	dstMask, 
							 int	srcPort, 
							 int	dstPort, 
							 int	protocol)
{
	CPacketFilterInterface interfaceHandle;

	// If the interface isn't created, then I create it.
	if(!interfacesTable.Lookup(localInterfaceIp, interfaceHandle))
	{

		interfaceHandle.Create(localInterfaceIp, defaultAction);

		interfacesTable[localInterfaceIp] = interfaceHandle;
	}


	// Fill the real filter struct
	PF_FILTER_DESCRIPTOR ipFlt;
	ipFlt.dwFilterFlags     = 0;
	ipFlt.dwRule            = 0;
	ipFlt.pfatType          = PF_IPV4;
	ipFlt.dwProtocol        = protocol;
	ipFlt.fLateBound        = 0;
	ipFlt.wSrcPort          = srcPort;
	ipFlt.wSrcPortHighRange = srcPort;
	ipFlt.wDstPort          = dstPort;
	ipFlt.wDstPortHighRange = dstPort;

				
	ipFlt.SrcAddr = (PBYTE) &srcIp;
	ipFlt.SrcMask = (PBYTE) &srcMask;
	ipFlt.DstAddr = (PBYTE) &dstIp;
	ipFlt.DstMask = (PBYTE) &dstMask;

	DWORD errorCode;

	// I add the filter
	if(direction == IN_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfAddFiltersToInterface(interfaceHandle.hInterface,
											1,
											&ipFlt,
											0,
											NULL,
											NULL);
	
	if(direction == OUT_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfAddFiltersToInterface(interfaceHandle.hInterface,
											0,
											NULL, 
											1,
											&ipFlt,
											NULL);

	else
		return -2;

	if(errorCode != NO_ERROR)
		return -1;

	return 0;
}

int CPacketFilter::RemoveFilter(char	*localInterfaceIp, 
								int		direction, 
								char	*srcIp, 			
								char	*srcMask, 
								char	*dstIp, 
								char	*dstMask, 
								int		srcPort, 
								int		dstPort, 
								int		protocol)
{
	CPacketFilterInterface interfaceHandle;

	// If the interface isn't created, then I create it.
	if(!interfacesTable.Lookup(CharToIp(localInterfaceIp), interfaceHandle))
	{
		return -1;
	}


	// Fill the real filter struct
	PF_FILTER_DESCRIPTOR ipFlt;
	ipFlt.dwFilterFlags     = FD_FLAGS_NOSYN;
	ipFlt.dwRule            = 0;
	ipFlt.pfatType          = PF_IPV4;
	ipFlt.dwProtocol        = protocol;
	ipFlt.fLateBound        = 0;
	ipFlt.wSrcPort          = srcPort;
	ipFlt.wSrcPortHighRange = srcPort;
	ipFlt.wDstPort          = dstPort;
	ipFlt.wDstPortHighRange = dstPort;

	unsigned long lIpSrc    = CharToIp(srcIp);
	unsigned long lIpDst	= CharToIp(dstIp);
	unsigned long lMaskSrc	= CharToIp(srcMask);
	unsigned long lMaskDst	= CharToIp(dstMask);
				
	ipFlt.SrcAddr = (PBYTE) &lIpSrc;
	ipFlt.SrcMask = (PBYTE) &lMaskSrc;
	ipFlt.DstAddr = (PBYTE) &lIpDst;
	ipFlt.DstMask = (PBYTE) &lMaskDst;

	DWORD errorCode;

	// I add the filter
	if(direction == IN_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfRemoveFiltersFromInterface(interfaceHandle.hInterface,
												 1,
												 &ipFlt,
												 0,
												 NULL);
	
	if(direction == OUT_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfRemoveFiltersFromInterface(interfaceHandle.hInterface,
												 0,
												 NULL, 
												 1,
												 &ipFlt);

	else
		return -2;

	if(errorCode != NO_ERROR)
		return -1;

	return 0;
}



int CPacketFilter::RemoveFilter(unsigned long	localInterfaceIp, 
								int				direction, 
								unsigned long	srcIp, 			
								unsigned long	srcMask, 
								unsigned long	dstIp, 
								unsigned long	dstMask, 
								int				srcPort, 
								int				dstPort, 
								int				protocol)
{
	CPacketFilterInterface interfaceHandle;

	// If the interface isn't created, then I create it.
	if(!interfacesTable.Lookup(localInterfaceIp, interfaceHandle))
	{
		return -1;
	}


	// Fill the real filter struct
	PF_FILTER_DESCRIPTOR ipFlt;
	ipFlt.dwFilterFlags     = FD_FLAGS_NOSYN;
	ipFlt.dwRule            = 0;
	ipFlt.pfatType          = PF_IPV4;
	ipFlt.dwProtocol        = protocol;
	ipFlt.fLateBound        = 0;
	ipFlt.wSrcPort          = srcPort;
	ipFlt.wSrcPortHighRange = srcPort;
	ipFlt.wDstPort          = dstPort;
	ipFlt.wDstPortHighRange = dstPort;

	ipFlt.SrcAddr = (PBYTE) &srcIp;
	ipFlt.SrcMask = (PBYTE) &srcMask;
	ipFlt.DstAddr = (PBYTE) &dstIp;
	ipFlt.DstMask = (PBYTE) &dstMask;

	DWORD errorCode;

	// I add the filter
	if(direction == IN_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfRemoveFiltersFromInterface(interfaceHandle.hInterface,
												 1,
												 &ipFlt,
												 0,
												 NULL);
	
	if(direction == OUT_DIRECTION || direction == ANY_DIRECTION)
		errorCode = PfRemoveFiltersFromInterface(interfaceHandle.hInterface,
												 0,
												 NULL, 
												 1,
												 &ipFlt);

	else
		return -2;

	if(errorCode != NO_ERROR)
		return -1;

	return 0;
}



int CPacketFilter::AddGlobalFilter(char	*localInterfaceIp, 
								   int globalFilter)
							 
{
	CPacketFilterInterface interfaceHandle;

	// If the interface isn't created, then I create it.
	if(!interfacesTable.Lookup(CharToIp(localInterfaceIp), interfaceHandle))
	{

		interfaceHandle.Create(CharToIp(localInterfaceIp), defaultAction);

		interfacesTable[CharToIp(localInterfaceIp)] = interfaceHandle;
	}


	DWORD errorCode;

	// Add the global filter
	errorCode = PfAddGlobalFilterToInterface(interfaceHandle.hInterface, (GLOBAL_FILTER)globalFilter);
			
	if(errorCode != NO_ERROR)
		return -1;

	return 0;
}



int CPacketFilter::RemoveGlobalFilter(char	*localInterfaceIp, 
								      int   globalFilter)
							 
{
	CPacketFilterInterface interfaceHandle;

	// If the interface isn't created, then I create it.
	if(!interfacesTable.Lookup(CharToIp(localInterfaceIp), interfaceHandle))
	{
		return -1;
	}


	DWORD errorCode;

	// Add the global filter
	errorCode = PfRemoveGlobalFilterFromInterface(interfaceHandle.hInterface, (GLOBAL_FILTER)globalFilter);
			
	if(errorCode != NO_ERROR)
		return -1;

	return 0;
}


void CPacketFilter::SetDefaultAction(PFFORWARD_ACTION action)
{
	defaultAction = action;
}


PFFORWARD_ACTION  CPacketFilter::GetDefaultAction()
{
	return defaultAction;
}

/***********************************************
Class CPacketFilterInterface. 
***********************************************/

CPacketFilterInterface::CPacketFilterInterface()
{

}

CPacketFilterInterface::~CPacketFilterInterface()
{
//	PfDeleteInterface(hInterface);
}

// Create an Interface and associate it to a Ip Address
int CPacketFilterInterface::Create(unsigned long ip, PFFORWARD_ACTION defaultAction)
{

	// I create the interface. Predefined acctions, forward all.
	DWORD errorCode = PfCreateInterface(0, 
										defaultAction, 
										defaultAction, 
										FALSE, 
										TRUE, 
										&hInterface);

	if(errorCode != NO_ERROR)
	{
		return -1;
	}


	// Bind the Ip Address with the interface
	PBYTE lIp = (PBYTE)&ip;
	errorCode = PfBindInterfaceToIPAddress(hInterface, PF_IPV4, lIp);
			
	if(errorCode != NO_ERROR)
	{
		PfDeleteInterface(hInterface);

		hInterface = NULL;

		return -2;
	}

	return 0;
}




unsigned long CharToIp(const char *sIp)
{
	int octets[4];
	int i;
	const char * auxCad = sIp;
	unsigned long lIp = 0;
	
	//we extract each octet of the ip address
	//atoi will get characters until it found a non numeric character(in our case '.')
	for(i = 0; i < 4; i++)
	{
		octets[i] = atoi(auxCad);

		if(octets[i] < 0 || octets[i] > 255)
			return 0;

		lIp |= (octets[i] << (i*8));

		//update auxCad to point to the next octet
		auxCad = strchr(auxCad, '.');

		if(auxCad == NULL && i!=3)
			return -1;

		auxCad++;
	}


	return lIp;
}



