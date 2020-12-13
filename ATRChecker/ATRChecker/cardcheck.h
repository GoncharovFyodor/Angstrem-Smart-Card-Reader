#pragma once
#include "stdafx.h"
#include <winscard.h>
#include <iostream>
#include <cstdio>
#include <cstring>
void cardCheck(HWND hWnd, LONG lReturn, SCARDCONTEXT hSC,
	SCARDHANDLE hCardHandle, LPTSTR pReader, LPTSTR pmszReaders, DWORD dwAP, LPBYTE pbAttr, DWORD cByte, std::string title);