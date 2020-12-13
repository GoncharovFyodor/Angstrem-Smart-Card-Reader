#include "stdafx.h"
#include "cardcheck.h"
void cardCheck(HWND hWnd, LONG lReturn, SCARDCONTEXT hSC, SCARDHANDLE hCardHandle, LPTSTR pReader, 
	LPTSTR pmszReaders, DWORD dwAP, LPBYTE pbAttr, DWORD cByte, std::string title){
	switch (lReturn)
	{
	case SCARD_E_NO_READERS_AVAILABLE:
		MessageBoxA(hWnd, "����� �� ���������!", "����������� ������", MB_OK);
		//printf("SCardListReaders: Reader is not in groups!!!\n");
		break;

	case SCARD_S_SUCCESS:
		pReader = pmszReaders;
		MessageBoxA(hWnd, "����� ���������", "����������� ������", MB_OK);
		//printf("SCardListReaders: Reader %S\n", pReader);
	}

	////////////////////////////////////////////////////////////////////////////////////
	/////////////////////        SCardConnect         //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	lReturn = SCardConnect(hSC, (LPTSTR)pReader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCardHandle, &dwAP); // NULL
	if (SCARD_S_SUCCESS != lReturn)
	{
		MessageBoxA(hWnd, "����� �� ����������!", "������", MB_OK);
		//printf("SCardConnect: Failed SCardConnect!!!\n");
	}
	switch (dwAP)
	{
	case SCARD_PROTOCOL_T0:
		MessageBoxA(hWnd, "������� �������� T0", "����������", MB_OK);
		//printf("SCardConnect: Active protocol T0\n");
		break;

	case SCARD_PROTOCOL_T1:
		MessageBoxA(hWnd, "������� �������� T1", "����������", MB_OK);
		//printf("SCardConnect: Active protocol T1\n");
		break;

	case SCARD_PROTOCOL_UNDEFINED:
	default:
		MessageBoxA(hWnd, "����������� �������� ��������!!!", "����������", MB_OK);
		//printf("SCardConnect: Active protocol unnegotiated or unknown!!!\n");
		break;
	}
	////////////////////////////////////////////////////////////////////////////////////
	/////////////////////      ATR: SCardGetAttrib    //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	pbAttr = NULL;
	cByte = SCARD_AUTOALLOCATE;

	lReturn = SCardGetAttrib(hCardHandle, SCARD_ATTR_ATR_STRING, (LPBYTE)&pbAttr, &cByte);
	//printf("\n");
	//printf("ATR: ");
	title += "\nATR: ";
	if (SCARD_S_SUCCESS != lReturn)
	{
		title += ("Value not supported!!!\n");
	}
	else
	{
		char* st = new char[100];
		std::string TS = "";
		std::string T0 = "";
		std::string T1 = "";
		std::string T2 = "";
		std::string TC1 = "";
		std::string ok = "";
		std::string analysisTS = "";
		std::string analysisT0 = "";
		std::string analysisTA1 = "";
		std::string analysisTB1 = "";
		std::string analysisTC1 = "";
		std::string analysisTD1 = "";
		std::string analysisTA2 = "";
		boolean enabledTA1 = false, enabledTB1 = false, enabledTC1 = false, enabledTD1 = false;
		boolean enabledTA2 = false, enabledTB2 = false, enabledTC2 = false, enabledTD2 = false;
		// Output the bytes.
		for (int i = 0; i < cByte; i++) {
			sprintf(st, "%02X ", (*(pbAttr + i)));
			if (i == 0) TS = st;
			if (i == 1) T0 = st;
			if (i == 2) T1 = st;
			if (i == 3) T2 = st;
			title += st;
		}
		title += ("\n");
		if (title == "\nATR: 3B 69 00 00 8F 8C 8E 8F 8C 80 01 23 00 \n")
			ok = "����� ��������";
		else if (title == "\nATR: 3B 69 00 00 00 00 00 \n" || title == "\nATR: 3B 69 00 00 00 00 01 \n")
			ok = "������ �����";
		else
			ok = "����� �� �������� ���������/����";
		/*
		//TS Analysis
		if (TS == "3B ")
			analysisTS = "������ ����������";
		if (TS == "3F ")
			analysisTS = "��������� ����������";

		//T0 Analysis
		if (T0[0] == '5') {
			analysisT0 = "TA1 ����������, ";
			analysisT0 += T0[0];
			analysisT0 += " ������������ ������";
			enabledTA1 = true;
		}
		if (T0[0] == '6') {
			analysisT0 = "TB1 ����������, ";
			analysisT0 += T0[0];
			analysisT0 += " ������������ ������";
			enabledTB1 = true;
		}
		if (T0[0] == '7') {
			analysisT0 = "TC1 ����������, ";
			analysisT0 += T0[0];
			analysisT0 += " ������������ ������";
			enabledTC1 = true;
		}
		if (T0[0] == '8') {
			analysisT0 = "TD1 ����������, ";
			analysisT0 += T0[0];
			analysisT0 += " ������������ ������";
			enabledTD1 = true;
		}

		//TA1 Analysis
		if (enabledTA1) {
			analysisTA1 = "������� - ";
			analysisTA1 += T1[0];
			analysisTA1 += ", ��������";
			analysisTA1 += T1[1];
		}

		//TB1 Analysis
		if (enabledTB1) {
			if (T1[0] == '0' && T1[0] == '0') {
				analysisTB1 = "VPP �� ���������";
			}
			else {
				analysisTB1 = "������������ ��� ���������������� - ";
				analysisTB1 += T1[0];
				analysisTB1 += ", ���������� ����������������";
				analysisTB1 += T1[1];
			}
		}

		//T�1 Analysis
		if (enabledTC1) {
				analysisTC1 = "Guard time - ";
				analysisTC1 += strtol(""+T1[0]+T1[1],NULL,16);
		}

		//TD1 Analysis
		if (enabledTD1) {
			if (T1[0] == '5') {
				analysisTD1 = "TA2 ����������, ";
				enabledTA2 = true;
			}
			if (T1[0] == '6') {
				analysisTD1 = "TB2 ����������, ";
				enabledTB2 = true;
			}
			if (T1[0] == '7') {
				analysisTD1 = "TC2 ����������, ";
				enabledTC2 = true;
			}
			if (T1[0] == '8') {
				analysisTD1 = "TD2 ����������, ";
				enabledTD2 = true;
			}
			if (T1[1] == '0') {
				analysisTD1 += "�������� �0";
			}
			if (T1[1] == '1') {
				analysisTD1 += "�������� �1";
			}
		}
		//TA2 Analysis
		if (enabledTA2) {
			if (T2[0] == '0') {
				analysisTA2 = "����� �����������";
			}else{
				analysisTA2 = "����������������� �����";
			}
		}*/
		// Free the memory when done.
					// hContext was set earlier by SCardEstablishContext
		lReturn = SCardFreeMemory(hSC, pbAttr);
		char msg[100];
		std::string analysis = "";
		strcpy(msg, (title + ok + analysis).c_str());
		MessageBoxA(hWnd, msg, "��������� ��������", MB_OK);
	}
}
