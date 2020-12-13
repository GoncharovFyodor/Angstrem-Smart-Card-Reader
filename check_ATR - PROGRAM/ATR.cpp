//#include "stdafx.h"
//#include <winscard.h>
#include <stdio.h>
#include <reader.h>
//#include <locale.h>
//#include <conio.h>
#include <iostream>
#include <winscard.h>
//#include "SmartCardLib.h"
//#include <libccidtwin>
//#include <fstream>
//#include <sstream>

using namespace std;

int main()
{
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////     Initialization   /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
	SCARDCONTEXT    hSC;
	LONG            lReturn;
	LPTSTR          pmszReaders = NULL;
	LPTSTR          pReader;
	DWORD           cch = SCARD_AUTOALLOCATE;
	SCARDHANDLE     hCardHandle;
	DWORD           dwAP;

	BYTE		pbSend_SerNum[] = {0x00, 0xCA, 0x01, 0x00, 0x04}; 
	DWORD		dwSend = 5; 
	BYTE		*pbRecv_SerNum = new BYTE[6]; 
	DWORD		dwRecv_SerNum = 6; 

////////////////////////////////////////////////////////////////////////////////////
/////////////////////     SCardEstablishContext   //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
	lReturn = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hSC);
	if(SCARD_SCOPE_USER != lReturn)
		printf("SCardEstablishContext: Error!!! \n");
	else
	{
		printf("SCardEstablishContext: SCARD_S_SUCCESS\n");
	}
////////////////////////////////////////////////////////////////////////////////////
/////////////////////       SCardListReaders      //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
	lReturn = SCardListReaders(hSC,NULL,(LPTSTR)&pmszReaders,&cch );
	switch( lReturn )
	{
		case SCARD_E_NO_READERS_AVAILABLE:
			printf("SCardListReaders: Reader is not in groups!!!\n");
			break;

		case SCARD_S_SUCCESS:
			pReader = pmszReaders;
			printf("SCardListReaders: Reader %S\n", pReader );
	}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////        SCardConnect         //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

	lReturn = SCardConnect( hSC, (LPTSTR)/*pmszReaders*/pReader,SCARD_SHARE_SHARED,SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,&hCardHandle,&dwAP ); // NULL
	if ( SCARD_S_SUCCESS != lReturn )
	{
		printf("SCardConnect: Failed SCardConnect!!!\n");
	}
	switch ( dwAP )
	{
		case SCARD_PROTOCOL_T0:
			printf("SCardConnect: Active protocol T0\n"); 
			break;

		case SCARD_PROTOCOL_T1:
			printf("SCardConnect: Active protocol T1\n"); 
			break;

		case SCARD_PROTOCOL_UNDEFINED:
		default:
			printf("SCardConnect: Active protocol unnegotiated or unknown!!!\n"); 
			break;
	}
////////////////////////////////////////////////////////////////////////////////////
/////////////////////      ATR: SCardGetAttrib    //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
	LPBYTE   pbAttr = NULL;
	DWORD    cByte = SCARD_AUTOALLOCATE;
	DWORD    i;

	lReturn = SCardGetAttrib(hCardHandle,SCARD_ATTR_ATR_STRING,(LPBYTE)&pbAttr,&cByte);
	printf("\n");
	printf("ATR: ");
	if ( SCARD_S_SUCCESS != lReturn )
	{
		printf("Value not supported!!!\n");
	}
	else
	{
		// Output the bytes.
		for (i = 0; i < cByte; i++)
			printf("%02X ", *(pbAttr+i));
		printf("\n");
		
		// Free the memory when done.
		// hContext was set earlier by SCardEstablishContext
		lReturn = SCardFreeMemory( hSC, pbAttr );
	}
//////////////////////////////////
//////////    END   //////////////
//////////////////////////////////

	
	getchar();
	getchar();
	return 0;
}