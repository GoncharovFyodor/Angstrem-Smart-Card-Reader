// ATRChecker.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "ATRChecker.h"
#include "cardcheck.h"
#include <winscard.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    CheckCard(HWND, UINT, WPARAM, LPARAM);

std::wstring string_to_wstring(const std::string& text) {
	return std::wstring(text.begin(), text.end());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
	
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ATRCHECKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ATRCHECKER));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ATRCHECKER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ATRCHECKER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////     Initialization   /////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	SCARDCONTEXT    hSC;
	LONG            lReturn;
	LPTSTR          pmszReaders = NULL;
	LPTSTR          pReader = NULL;
	DWORD           cch = SCARD_AUTOALLOCATE;
	SCARDHANDLE     hCardHandle = NULL;
	DWORD           dwAP = NULL;

	BYTE		pbSend_SerNum[] = { 0x00, 0xCA, 0x01, 0x00, 0x04 };
	DWORD		dwSend = 5;
	BYTE		*pbRecv_SerNum = new BYTE[6];
	DWORD		dwRecv_SerNum = 6;
	std::string newTitle="";
	LPBYTE   pbAttr = NULL;
	DWORD    cByte = SCARD_AUTOALLOCATE;
	DWORD    i;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case IDM_GUIDE:
				MessageBoxA(hWnd, 
					"Для проверки смарт-карты выполните следующие действия:\n 1. Вставьте карту\n 2. Нажмите в меню «Файл»–«Проверка смарт-карты»", 
					"Инструкция", MB_OK);
				break;
			case IDM_CHK:
				////////////////////////////////////////////////////////////////////////////////////
				/////////////////////     SCardEstablishContext   //////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				lReturn = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hSC);
				if (SCARD_SCOPE_USER != lReturn)
					MessageBoxA(hWnd, "", "ОШИБКА!!!", MB_OK);
					//DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, CheckCard);
				else
				{
					MessageBoxA(hWnd, "Служба запущена", "Информация", MB_OK);
					//DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, CheckCard);
				}
				////////////////////////////////////////////////////////////////////////////////////
				/////////////////////       SCardListReaders      //////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				lReturn = SCardListReaders(hSC, NULL, (LPTSTR)&pmszReaders, &cch);
				cardCheck(hWnd, lReturn, hSC, hCardHandle, pReader, pmszReaders, dwAP, pbAttr, cByte, newTitle);
				/*
				switch (lReturn)
				{
				case SCARD_E_NO_READERS_AVAILABLE:
					MessageBoxA(hWnd, "Ридер не подключен!", "Подключение ридера", MB_OK);
					//printf("SCardListReaders: Reader is not in groups!!!\n");
					break;

				case SCARD_S_SUCCESS:
					pReader = pmszReaders;
					MessageBoxA(hWnd, "Ридер подключен", "Подключение ридера", MB_OK);
					//printf("SCardListReaders: Reader %S\n", pReader);
				}
				
				////////////////////////////////////////////////////////////////////////////////////
				/////////////////////        SCardConnect         //////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////

				lReturn = SCardConnect(hSC, (LPTSTR)pReader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCardHandle, &dwAP); // NULL
				if (SCARD_S_SUCCESS != lReturn)
				{
					MessageBoxA(hWnd, "Карта не обнаружена!", "Ошибка", MB_OK);
					//printf("SCardConnect: Failed SCardConnect!!!\n");
				}
				switch (dwAP)
				{
				case SCARD_PROTOCOL_T0:
					MessageBoxA(hWnd, "Активен протокол T0", "Информация", MB_OK);
					//printf("SCardConnect: Active protocol T0\n");
					break;

				case SCARD_PROTOCOL_T1:
					MessageBoxA(hWnd, "Активен протокол T1", "Информация", MB_OK);
					//printf("SCardConnect: Active protocol T1\n");
					break;

				case SCARD_PROTOCOL_UNDEFINED:
				default:
					MessageBoxA(hWnd, "Неизвестный активный протокол!!!", "Информация", MB_OK);
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
				newTitle += "\nATR: ";
				if (SCARD_S_SUCCESS != lReturn)
				{
					newTitle+=("Value not supported!!!\n");
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
					for (i = 0; i < cByte; i++) {
						sprintf(st, "%02X ", (*(pbAttr + i)));
						if (i == 0) TS = st;
						if (i == 1) T0 = st;
						if (i == 2) T1 = st;
						if (i == 3) T2 = st;
						newTitle += st;
					}
					newTitle+=("\n");
					if (newTitle == "\nATR: 3B 69 00 00 8F 8C 8E 8F 8C 80 01 23 00 \n")
						ok = "Карта исправна";
					else if (newTitle == "\nATR: 3B 69 00 00 00 00 00 \n" || newTitle == "\nATR: 3B 69 00 00 00 00 01 \n")
						ok = "Чистая карта";
					else
						ok = "Карта не является исправной/Брак";
					/*
					//TS Analysis
					if (TS == "3B ")
						analysisTS = "Прямая полярность";
					if (TS == "3F ")
						analysisTS = "Инверсная полярность";

					//T0 Analysis
					if (T0[0] == '5') {
						analysisT0 = "TA1 передается, ";
						analysisT0 += T0[0];
						analysisT0 += " исторических байтов";
						enabledTA1 = true;
					}
					if (T0[0] == '6') {
						analysisT0 = "TB1 передается, ";
						analysisT0 += T0[0];
						analysisT0 += " исторических байтов";
						enabledTB1 = true;
					}
					if (T0[0] == '7') {
						analysisT0 = "TC1 передается, ";
						analysisT0 += T0[0];
						analysisT0 += " исторических байтов";
						enabledTC1 = true;
					}
					if (T0[0] == '8') {
						analysisT0 = "TD1 передается, ";
						analysisT0 += T0[0];
						analysisT0 += " исторических байтов";
						enabledTD1 = true;
					}

					//TA1 Analysis
					if (enabledTA1) {
						analysisTA1 = "Делимое - ";
						analysisTA1 += T1[0];
						analysisTA1 += ", делитель";
						analysisTA1 += T1[1];
					}

					//TB1 Analysis
					if (enabledTB1) {
						if (T1[0] == '0' && T1[0] == '0') {
							analysisTB1 = "VPP не подключен";
						}
						else {
							analysisTB1 = "Максимальный ток программирования - ";
							analysisTB1 += T1[0];
							analysisTB1 += ", напряжение программирования";
							analysisTB1 += T1[1];
						}
					}

					//TС1 Analysis
					if (enabledTC1) {
							analysisTC1 = "Guard time - ";
							analysisTC1 += strtol(""+T1[0]+T1[1],NULL,16);
					}

					//TD1 Analysis
					if (enabledTD1) {
						if (T1[0] == '5') {
							analysisTD1 = "TA2 передается, ";
							enabledTA2 = true;
						}
						if (T1[0] == '6') {
							analysisTD1 = "TB2 передается, ";
							enabledTB2 = true;
						}
						if (T1[0] == '7') {
							analysisTD1 = "TC2 передается, ";
							enabledTC2 = true;
						}
						if (T1[0] == '8') {
							analysisTD1 = "TD2 передается, ";
							enabledTD2 = true;
						}
						if (T1[1] == '0') {
							analysisTD1 += "Протокол Т0";
						}
						if (T1[1] == '1') {
							analysisTD1 += "Протокол Т1";
						}
					}
					//TA2 Analysis
					if (enabledTA2) {
						if (T2[0] == '0') {
							analysisTA2 = "Режим переговоров";
						}else{
							analysisTA2 = "Специфицированный режим";
						}
					}
					
					// Free the memory when done.
					// hContext was set earlier by SCardEstablishContext
					lReturn = SCardFreeMemory(hSC, pbAttr);
					char msg[100];
					std::string analysis = "";
					strcpy(msg, (newTitle+ok+analysis).c_str());
					MessageBoxA(hWnd, msg, "Результат проверки", MB_OK);
					
				}
				*/
				//////////////////////////////////
				//////////    END   //////////////
				//////////////////////////////////
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// Обработчик сообщений для окна "Проверка смарт-карты".
INT_PTR CALLBACK CheckCard(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


