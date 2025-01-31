#include "SmartCardLib.h"
#include <stdio.h>
//#include <stdlib.h>
//#include <locale.h>

using namespace std;

int main()
{
	//setlocale(0, "Russian");

	SMART_READER_HANDLE 	handle;
	SMART_CARD_HANDLE	cardHandle;

	unsigned char password[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
	unsigned char newPasWd[] = { 0x02, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
	unsigned char outData[12];
	unsigned char inData[524];
	unsigned char serNum[4];
	unsigned int  err;
	int remain;

	smartReaderInit(&handle, &err);

	smartCardPresent(handle, &err);
	printf("%d\n", err);
	smartCardOpen(handle, &cardHandle, &err);

	if (smartCardAuth(cardHandle, (const char*)&password, &remain, &err) == SMART_ERR_AUTH)
		printf("%d %02x\n", remain, err);
	else
		printf("%02X\n", err);

	//smartCardPasswd(cardHandle, (const char*)&password, &err);

	for (int i = 0; i < 8; i++)
		inData[i] = i+1;

	smartCardWrite(cardHandle, 0, 8, inData, &err);

	smartCardRead(cardHandle, 0, 8, outData, &err);

	printf("\n\nData from card:\n");
	for (int i = 0; i < 8; i++)
		printf("%02X ", outData[i]);
	printf("\n");

	//getSerialNum(cardHandle, serNum, &err);

	//for (int i = 0; i < 4; i++)
	//	printf("%02X ", serNum[i]);
	//printf("\n");

	return 0;
}