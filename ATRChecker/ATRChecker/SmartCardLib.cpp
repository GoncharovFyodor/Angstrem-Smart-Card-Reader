#include "stdafx.h"
#include "SmartCardLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <winscard.h>
#pragma comment(lib, "Winscard.lib")
using namespace std;

int smartReaderInit(SMART_READER_HANDLE *hd, unsigned int *err)
{
	*err = 0;
	//������� ������������� �����������
	//�� ���� �������� ��������� �� ���������� ���� SMART_READER_HANDLE, � ������� ��������� ��� ������, ������������� � �������
	//�� ������ ���������� SMART_SUCCESS ���� ������� ��������� ������� � SMART_ERR_INT ���� ������� ��������� � �������

	SCARDCONTEXT	context; //���������� ��� �������� ��������� �������
	LONG 			ret; //���������� ��� �������� ������������� �������� ������� ���������� winscard
	LPTSTR			listReaders; //���������� ��� �������� ����� ������, ������������� � �������
	DWORD			cch = SCARD_AUTOALLOCATE; //������������� ���������� ����� ����� ������

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context); //�������� ��������� � �������

	ret = SCardListReaders(context, NULL, (LPTSTR)hd, &cch); //��������, ���� �� ������������ � ������� ������

	if (ret == SCARD_S_SUCCESS) //���� ������� �� �������� ������������ � ������� ������� ��������� �������
	{
		printf("Device: %s\n", *hd); //������� ��� ������������� � ������� ������ �� �����
		return SMART_SUCCESS; //���������� "�����"
	}
	else //���� ��������� � �������
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT;  //���������� "���������� ������"
	}

}

void smartInternalState(unsigned int err)
{
	if (err == 0x109)
		printf("������ ��������� ��������� �������� �����-����� � ��������� ������.\n");
	else
		if ((err >> 16) == 0)
		{
			unsigned char w1, w2;

			w1 = err >> 8;
			w2 = err && 0x00ff;

			switch (w1)
			{
			case (0x90):
				//printf("90");
				switch (w2)
				{
				case(0x01):
					printf("9001:\t������ ������ ��������� �������� �����.\n");
					break;
				case(0x02):
					printf("9002:\t������ ������ ����� ������� ������.\n)");
					break;
				case(0x03):
					printf("9003:\t������ ������������������ �����.\n)");
					break;
				case(0x04):
					printf("9004:\t������ ��.\n");
					break;
				case(0x05):
					printf("9005:\t���� ATR �� ������.\n");
					break;
				case(0x06):
					printf("9006:\t������ MF.\n");
					break;
				case(0x07):
					printf("9007:\t���� ATR ����������.\n");
					break;
				case(0x08):
					printf("9008:\t���� ATR ������ �������.\n");
					break;
				case(0x0d):
					printf("900D:\t������ ������.\n");
					break;
				case(0x0e):
					printf("900E:\t������������ p3.\n");
					break;
				case(0x0f):
					printf("900F:\t������ ������ ������������� ����.\n");
					break;
				case(0x11):
					printf("9011:\t������ ������ ������������� ����.\n");
					break;
				case(0x14):
					printf("9014:\t������� ��������� ����������.\n");
					break;
				case(0x15):
					printf("9015:\t��������� ������� ����������.\n");
					break;
				case(0x16):
					printf("9016:\t����� �����������.\n");
					break;
				}
				break;

				/*case(0x61):
				printf("61%02x:\t����� ��������� ���� ������: %d.\n", w2, w2);
				break;*/

			case (0x62):
				//printf("62");
				switch (w2)
				{
				case(0x00):
					printf("6200:\t��������������: ��������� ��������������� ������ ��� ���������. ���������� �� �������������.\n");
					break;
				case(0x01):
					printf("6201:\t�������������� (������������): ������ ������. ������ �������� �����. ����� '�����' �����������.\n");
					break;
				case(0x02):
					printf("6202:\t�������������� (������������): �������� ������� ����������.\n");
					break;
				case (0x81):
					printf("6281:\t��������������: ��������� ����������������� ������ ��� ���������. ����� ���������� ������ ����� ���� ��������.\n");
					break;
				case(0x82):
					printf("6282:\t�������������� (������������): ������ �� �������� �����.\n");
					break;
				case(0x83):
					printf("6283:\t��������������: ��������� ����������������� ������ ��� ���������. ��������� ���� ��������������.\n");
					break;
				case(0x84):
					printf("6284:\t��������������: ��������� ����������������� ������ ��� ���������. ����������� ���������� ����� �� ������������� �� 5.3.3.\n");
					break;
				case(0x85):
					printf("6285:\t��������������: ��������� ����������������� ������ ��� ���������. ��������� ���� � ������ ����������.\n");
					break;
				case(0x86):
					printf("6286:\t��������������: ��������� ����������������� ������ ��� ���������. ��� ������� ������ �� �����, ���������� �� ��������.\n");
					break;
				case(0x87):
					printf("6287:\t��������������: ��������� ����������������� ������ ��� ���������. ��� ������� ���� ������ ��������������.\n");
					break;
				default:
					if ((w2 >= 0x03) && (w2 <= 0x80))
						printf("62%02x:\t��������������: ��������� ����������������� ������ ��� ���������. ��������� �����.\n", w2);
					break;
				}
				break;

			case(0x63):
				//printf("63");
				switch (w2)
				{
				case(0x00):
					printf("6300:\t��������������: ��������� ��������������� ������ ��������. ���������� �� �������������.\n");
					break;
				case(0x81):
					printf("6381:\t��������������: ��������� ��������������� ������ ��������. ���� �������� ��� ��������� �������� ������.\n");
					break;
				default:
					if ((w2 >> 4) == 0x0C)
						printf("63%02x:��������������: ������ ��������������.\n", w2);
					break;
				}
				break;

			case(0x64):
				//printf("64");
				switch (w2)
				{
				case(0x00):
					printf("6400:\t������ ������������: ������ ������. ������ ����������.\n");
					break;
				case(0x01):
					printf("6401:\t������ ���������: ��������� ����������������� ������ ��� ���������. ���������������� �����, ��������� ������.\n");
					break;
				default:
					if ((w2 >= 0x02) && (w2 <= 0x80))
						printf("64%02x:\t������ ���������: ��������� ����������������� ������ ��� ���������. ��������� �����.\n", w2);
					break;
				}
				break;

			case (0x65):
				//printf("65");
				switch (w2)
				{
				case(0x00):
					printf("6500:\t������ ���������: ��������� ����������������� ������ ��������. ������ ����������.\n");
					break;
				case(0x01):
					printf("6501:\t������ ������������: ������ ������. ������ �� �������� ��� �������� � �������. ����� '�����' �����������.\n");
					break;
				case(0x81):
					printf("6581:\t������ ���������: ��������� ����������������� ������ ��������. ����� ������.\n");
					break;
				}
				break;

			case(0x66):
				printf("66%02x:\t������ ���������: ��� ���������, ��������� � �������������.\n", w2);
				break;

			case(0x67):
				switch (w2)
				{
				case(0x00):
					printf("6700:\t������ ������������: �������� ����� ������.\n");
					break;
				}
				break;

			case(0x68):
				//printf("68");
				switch (w2)
				{
				case(0x00):
					printf("6800:\t������ ���������: �������, ��������� � ����� CLA, �� ��������������. ���������� �� �������������.\n");
					break;
				case(0x81):
					printf("6881:\t������ ���������: �������, ��������� � ����� CLA, �� ��������������. ���������� ����� �� ��������������.\n");
					break;
				case(0x82):
					printf("6882:\t������ ���������: �������, ��������� � ����� CLA, �� ��������������. ���������� ����� ����������� �� ��������������.\n");
					break;
				case(0x83):
					printf("6883:\t������ ���������: �������, ��������� � ����� CLA, �� ��������������. ��������� ��������� ������� � �������.\n");
					break;
				case(0x84):
					printf("6884:\t������ ���������: �������, ��������� � ����� CLA, �� ��������������. ��������� ������ �� ��������������.\n");
					break;
				}
				break;

			case(0x69):
				//printf("69");
				switch (w2)
				{
				case(0x00):
					printf("6900:\t������ ���������: ������� �� ���������. ���������� �� �������������.\n");
					break;
				case(0x81):
					printf("6981:\t������ ������������: ������� �� ��������� � ������ ��������� (�� ��������� � ������� ���� �����).\n");
					break;
				case(0x82):
					printf("6982:\t������ ����������� ������������� ����.\n");
					break;
				case(0x83):
					printf("6983:\t��������� ������� ������������ ��.\n");
					break;
				case(0x84):
					printf("6984:\t������ ������������: ���� ����������.\n");
					break;
				case(0x85):
					printf("6985:\t������ ������������: �� ��������� ������� ������������� ������� (�� ����������� ������� ASK_RANDOM ��� SEND_RANDOM, ...).\n");
					break;
				case(0x86):
					printf("6986:\t������������� ����������.\n");
					break;
				case(0x87):
					printf("6987:\t������ ���������: ������� �� ���������. ������� ��������� �������������� ��������, ��������� � ���������� ������� �����������.\n");
					break;
				case(0x88):
					printf("6988:\t������ ������������: �������� ���������� ���������.\n");
					break;
				case(0x80):
					printf("6980:\t������ ���������: ������� �� ���������. ������������ ��������� � ���� ������ �������.\n");
					break;
				case(0x89):
					printf("6989:\t������ ���������: ������� �� ���������. ���� ��� ����������.\n");
					break;
				case(0x8A):
					printf("698A:\t������ ���������: ������� �� ���������. ��� DF ��� ����������.\n");
					break;
				}
				break;

			case(0x6A):
				//printf("6A");
				switch (w2)
				{
				case(0x80):
					printf("6A80:\t������ ������������: ������������ �������� ���� ������ (���� � ��������� ��������������� ��� ������������).\n");
					break;
				case(0x81):
					printf("6A81:\t������ ������������: ������� �� �����������.\n");
					break;
				case(0x82):
					printf("6A82:\t������ ������������: ���� �� ������.\n");
					break;
				case(0x83):
					printf("6A83:\t������ ������������: ������ �� �������.\n");
					break;
				case(0x84):
					printf("6A84:\t������ ������������: ������������ ����� ��� ������.\n");
					break;
				case(0x85):
					printf("6A85:\t������ ������������: �������� ����� Lc �� ������������� ��������� TLV.\n");
					break;
				case(0x86):
					printf("6A86:\t������ ������������: �������� ��������� P1-P2.\n");
					break;
				case(0x87):
					printf("6A87:\t������ ������������: �������� ����� Lc �� ������������� P1-P2.\n");
					break;
				case(0x88):
					printf("6A88:\t������ ������������: ��������� ��������� ������ (���� ������) �����������.\n");
					break;
				default:
					printf("6A%02x:\t������ ���������: ������� ���������(�) ��������(�) P1-P2", w2);
					break;
				}
				break;

			case(0x6B):
				switch (w2)
				{
				case(0x00):
					printf("6B00:.\t������ ������������: �������� ��������� �1-�2 - �������� �� �������� �����.\n");
					break;
				}
				break;

			case(0x6C):
				printf("6C%02x:\t������ ������������: �������� ������ ������������� � ������ ������.\n", w2);
				break;

			case(0x6D):
				switch (w2)
				{
				case(0x00):
					printf("6D00:\t������������ ����������.\n");
					break;
				}
				break;

			case(0x6E):
				switch (w2)
				{
				case(0x00):
					printf("6E00:\t������������ �����.\n");
					break;
				}
				break;

			case(0x6F):
				switch (w2)
				{
				case(0x00):
					printf("6F00:\t������ ������������: ����������� ������.\n");
					break;
				case(0x82):
					printf("6F82:\t������ BUP �����.\n");
					break;
				}
				break;

			}
		}
		else
		{
			unsigned char w1 = err;

			switch (w1)
			{
			case(0x29):
				printf("��������� ������ ��� ��������� ��������� �� ������ ����� �����-�����.\n");
				break;
			case(0x02):
				printf("�������� ���� �������� �������� SCardCancel.\n");
				break;
			case(0x0E):
				printf("������� �� ������������� ���������� ����������� ��������.\n)");
				break;
			case(0x1C):
				printf("�����-����� �� �������� ����������� �����������.\n)");
				break;
			case(0x2D):
				printf("������������� ���������� �� ����� ���� �������.\n");
				break;
			case(0x2F):
				printf("������ ����� �� �����-������.\n");
				break;
			case(0x23):
				printf("�� �����-����� ����������� ������������� �������.\n");
				break;
			case(0x1B):
				printf("������� ����������� �� ������ ��� ���������.\n");
				break;
			case(0x24):
				printf("�� �����-����� ����������� ������������� ����.\n");
				break;
			case(0x21):
				printf("������������� ������� �������� �������� �� ��������������.\n");
				break;
			case(0x20):
				printf("���������� ����� ��������� ��������� ��� �����-�����.\n");
				break;
			case(0x08):
				printf("����� ��� ������������ ������ ������� ���.\n");
				break;
			case(0x15):
				printf("���������� ATR �� �������� ����������.\n");
				break;
			case(0x2A):
				printf("��������� PIN-��� �������.\n");
				break;
			case(0x03):
				printf("��������� handle �������.\n");
				break;
			case(0x04):
				printf("���������� ��������� ���������������� ���� ��� ��������� ������� ����������.\n");
				break;
			case(0x05):
				printf("���������� � ������� ������� ����������� ���� ���������������.\n");
				break;
			case(0x11):
				printf("���������� ��������� ���������������� ���� ��� ��������� �������� �������� ����������.\n");
				break;
			case(0x27):
				printf("������ � ����� ��������.\n)");
				break;
			case(0x25):
				printf("�������� ����� ���������� ���� �� �������� ���������.\n)");
				break;
			case(0x26):
				printf("�������� ����� ���������� ���� �� �������� ������.\n");
				break;
			case(0x30):
				printf("�� ����� ����������� ������������� ��������� �����.\n");
				break;
			case(0x06):
				printf("������������ ������ ��� ���������� �������.\n");
				break;
			case(0x33):
				printf("���������� ���������� PIN-��� �����.\n");
				break;
			case(0x2E):
				printf("����������� ���������� ������ �����-����.\n");
				break;
			case(0x1D):
				printf("�� ������� ��������� �������� �����-�����.\n");
				break;
			case(0x0C):
				printf("��� ������������� �������� ��������� �����-�����, ������� � ��������� ����� �����������.\n");
				break;
			case(0x2C):
				printf("�������������� ����������� �� ����������.\n");
				break;
			case(0x10):
				printf("����������� (��� �����-�����) �� ����� ��������� �������.\n");
				break;
			case(0x16):
				printf("���� ����������� ������� ���������� �������������� ����������.\n");
				break;
			case(0x19):
				printf("����� ������ PCI ������� ���.\n");
				break;
			case(0x32):
				printf("���� �������� PIN-���� ����� �����.\n");
				break;
			case(0x0F):
				printf("������������� ��������� ������������ � ����������, ������������ ������.\n");
				break;
			case(0x34):
				printf("�����-����� �������� ������ ��� ������.\n");
				break;
			case(0x17):
				printf("� ��������� ����� ��������� ����������� ����������.\n");
				break;
			case(0x1A):
				printf("������� ����������� �� �������� ����������� �����������.\n");
				break;
			case(0x31):
				printf("��������� �������� �����-���� ����� � �� ����� ��������� ��� ��������.\n");
				break;
			case(0x1E):
				printf("��������� �������� �����-����� ��������.\n");
				break;
			case(0x0B):
				printf("���������� �������� ������ � �����-�����.\n");
				break;
			case(0x12):
				printf("������� �������� �������� (����������������, ����� ����� ��� �����������).\n)");
				break;
			case(0x0A):
				printf("��������� ������������� ����� (����-���) �������.\n)");
				break;
			case(0x1F):
				printf("��������� �������������� ������ �����.\n");
				break;
			case(0x0D):
				printf("�� ������������ ��������� ��� �����-�����.\n");
				break;
			case(0x09):
				printf("�� ������������ ��������� ��� �����������.\n");
				break;
			case(0x2B):
				printf("��� ��������� ����������� ��� ������.\n");
				break;
			case(0x22):
				printf("�����-����� �� ������������ ������������� �������.\n");
				break;
			case(0x28):
				printf("���� ����������� ������� �������� ������ ������, ��� � ������� �������.\n");
				break;
			case(0x13):
				printf("���������� ������ ���������� �����.\n");
				break;
			case(0x01):
				printf("���������� ������ ���������� ���������������.\n");
				break;
			case(0x14):
				printf("���������� ���������� ������. �������� ������ ����������.\n");
				break;
			case(0x07):
				printf("����� ������ ���������� ���������������.\n");
				break;
			case(0x18):
				printf("�������� ���� ��������.\n");
				break;
			case(0x6E):
				printf("�������� ��������� �������������.\n");
				break;
			case(0x70):
				printf("������������� ������� � ���� �� ���������.\n");
				break;
			case(0x71):
				printf("������������� ������� ���� ������� ���� � ��� ������.\n");
				break;
			case(0x72):
				printf("����� ������� ���� ��������� ����������� ���������� ������.\n");
				break;
			case(0x6F):
				printf("����� �� ���������� PIN-���.\n");
				break;
			case(0x6C):
				printf("������ � ����� ����������. ��������� ������� ����� PIN-����.\n");
				break;
			case(0x6D):
				printf("��������� ����� ����� �����-�����.\n");
				break;
			case(0x69):
				printf("����� � ������ ����������. ����� ���� ������� �� �����������.\n");
				break;
			case(0x68):
				printf("�����-����� �������� (reset).\n)");
				break;
			case(0x6A):
				printf("��������� ������ � ����� �� ������� ��������� ������������.\n)");
				break;
			case(0x67):
				printf("������� ����� ���� ����������. ���������� ����� � �����-������ ����������.\n");
				break;
			case(0x66):
				printf("�����-����� �� �������� �� ������ ������ (reset).\n");
				break;
			case(0x65):
				printf("����������� �� ����� ��������� � �����-������ ��-�� ���������� ������������ ATR.\n");
				break;
			case(0x6B):
				printf("���������� �������� �����-�����. ����������� �������� PIN-���.\n");
				break;
			}
		}
}

int smartCardPresent(SMART_READER_HANDLE readerHD, unsigned int *err)
{
	*err = 0;

	//������� �������� ����������� ����� � �����������
	//�� ���� �������� ���������� ���� SMART_READER_HANDLE, � ������� �������� ��� ������, ������������� � �������
	//�� ������ ���������� SMART_SUCCESS ���� ������� ��������� �������, SMART_ERR_CARD ���� ����������� ��� ����� � SMART_ERR_INT ���� ������ ������

	SCARDCONTEXT	context; //���������� ��� �������� ��������� �������
	LONG			ret; //���������� ��� �������� ������������� �������� ������� ���������� winscard
	DWORD			dwActiveProtocol; //���������� ��� �������� ���������, �� �������� ��������� �����
	SCARDHANDLE		cardHandle; //���������� ��� �������� �������������� �����, ����������� � �����

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context); //�������� ��������� � �������
	ret = SCardConnect(context, (LPTSTR)readerHD, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &cardHandle, &dwActiveProtocol); //������������ � �����, ����������� � �����

	if (ret != SCARD_S_SUCCESS) //���� ����������� � ����� ���� � �������
								//if (ret == SCARD_W_REMOVED_CARD) //���� ����� ��� � ������
								//	return SMART_ERR_CARD; //����������� "������ �����"
								//else
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //���� ����� ���� � ������, �� ������ ������, ������ ���������� "���������� ������"
	}

	return SMART_SUCCESS; //���������� "�����"
}

int smartCardOpen(SMART_READER_HANDLE readerHD, SMART_CARD_HANDLE *cardHD, unsigned int *err)
{
	*err = 0;

	//������� �������� �����.
	//�� ���� �������� ���������� ���� SMART_READER_HANDLE, � ������� �������� ��� ������, ������������� � ������� � ��������� �� ���������� SMART_CARD_HANDLE, � ������� ��������� ������������� �����
	//� ������ ������ ��������� SMART_SUCCESS. ���� ����� ��� - �� SMART_ERR_CARD, ���� ����� ������� - �� SMART_ERR_CINV, � ���� ��������� � �������, �� ���������� SMART_ERR_INT

	SCARDCONTEXT	context; //���������� ��� �������� ��������� �������
	LONG			ret; //���������� ��� �������� ������������� �������� ������� ���������� winscard

	BYTE			cardATR[] = { 0x3b, 0x69, 0x00, 0x00, 0x8f, 0x8c, 0x8e, 0x8f, 0x8c, 0x80, 0x01, 0x23, 0x00 }; //��������� ATR � �����
	BYTE			*atr; //��������� �� ������ ��� ��������� ��������� ATR �����
	DWORD			atrByte = SCARD_AUTOALLOCATE; //������ ������� ��� �������� ��������� ATR ����� ���������� �������������
	DWORD			dwActiveProtocol = SCARD_AUTOALLOCATE; //�������� ������ ����� ���������� �������������

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context); //�������� ��������� � �������

	ret = SCardConnect(context, (LPTSTR)readerHD, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, (LPSCARDHANDLE)cardHD, &dwActiveProtocol); //������������ � �����, ����������� � �����

	if (ret == SCARD_W_REMOVED_CARD) //���� ����������� ��� �����
		return SMART_ERR_CARD; //���������� "������ �����"

	if (ret == SCARD_S_SUCCESS) //���� ����������� � ���� ��������� �������
	{
		ret = SCardGetAttrib(*cardHD, SCARD_ATTR_ATR_STRING, (LPBYTE)&atr, &atrByte); //�������� ATR

		if (atrByte != 13)					//���� ����� ����������� ATR �� ��������� � ���������
			return SMART_ERR_CINV; //���������� "�������� �����"

		for (int i = 0; i < atrByte; i++) //��������� �� ���������� ATR ��������� � ����������
			if (atr[i] != cardATR[i])		//���� ���� �� ���� �������� �� ���������� ATR �� ��������� � ���������
				return SMART_ERR_CINV; //���������� "�������� �����"

		return SMART_SUCCESS; //���� ���� ��������� �������, ���������� "�����"
	}
	else
	{
		*err = (unsigned int)ret;
		SMART_ERR_INT; //���������� "���������� ������"
	}
}

int smartCardAuth(SMART_CARD_HANDLE cardHD, const char *pass, int *remain, unsigned int *err) //��� ���������� � remain � ������ ������? ���� �� ����� �������������?
{
	*err = 0;

	//����������� �����.
	//�� ���� �������� ���������� ���� SMART_CARD_HANDLE, � ������� �������� ������������� �����, ��������� �� ���������� ���� char, � ������� �������� ������, � ���������� ���� int � �������
	//� ������� ���� ������ �� �����, ���������� ���������� ���������� ������� ����� ������ �� ���������� �����.
	//� ������ ������ ��������� SMART_SUCCESS. ���� ����������� �� ������ - SMART_ERR_AUTH. � ������ ������� � remain ������������ ���������� ���������� �������

	BYTE		caseKeyFile[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x00, 0x09 }; //������� ��� ������ ����� ������
	BYTE		verifyPassWd[13] = { 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //������ ��� ������� ������ ����� (5 ���� ���������� ����� � 8 ���� ������)

	BYTE		recvBuff[2]; //������ ��� �������� ������ ����� (��� �������� SW1 � SW2)
	LONG		ret; //���������� ��� �������� ������������� ������
	unsigned long 	cRecv = 2; //���������� ���� ������������� ������

	ret = SCardTransmit((SCARDHANDLE)cardHD, SCARD_PCI_T0, caseKeyFile, 7, NULL, recvBuff, &cRecv); //�������� ������ �� ����� ��������� �����

	if (ret == SCARD_S_SUCCESS) //��������� ��� ������� �������� ��������� ����� ��������� �������
	{
		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //���� ������� ������ ��������� ����� ��������� �������
		{
			for (int i = 0; i < 8; i++) //��������� ������� ��� ��������������
				verifyPassWd[5 + i] = (BYTE)pass[i];

			ret = SCardTransmit(cardHD, SCARD_PCI_T0, verifyPassWd, 13, NULL, recvBuff, &cRecv); //�������� ������ ������������� ������

			if (ret == SCARD_S_SUCCESS) //��������� ��� ������� �������� ��������� ����� ��������� �������
			{
				if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //���� ������� ������� ��������� � ������ ������
					return SMART_SUCCESS; //���������� "�����"
				else ///���� ������ ������? //���� �� ������� ��������� � �������
					if (recvBuff[0] == 0x63) //���� ������ �� �����
					{
						*remain = int(recvBuff[1] & 0x0f); //���������� ������ ���������� ������� � remain
						return SMART_ERR_AUTH; //���������� "������ ��������������"
					}
					else
					{
						*err = (recvBuff[0] << 8) + recvBuff[1];
						return SMART_ERR_INT; //���������� "���������� ������"
					}
			}
			else
			{
				*err = (unsigned int)ret;
				return SMART_ERR_INT; //���������� "���������� ������"
			}
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //���������� "���������� ������"
		}
	}
	else
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //���������� "���������� ������"
	}
}

int smartCardPasswd(SMART_CARD_HANDLE cardHD, const char *pass, unsigned int *err) ///���� ����������� �� ���� ��������?
{
	*err = 0;

	//����� ������ �� �����.
	//�� ���� �������� ���������� ���� SMART_CARD_HANDLE, � ������� �������� ������������� �����, ��������� �� ���������� ���� char, � ������� �������� ����� ������
	//� ������ ������ ��������� SMART_SUCCESS. ���� ����������� �� ������ - SMART_ERR_AUTH

	BYTE		caseKeyFile[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x00, 0x09 }; //������� ��� ������ ����� ������
	BYTE		newPassWd[13] = { 0x00, 0xDC, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //������ ��� ������� ������ ������ ����� (5 ���� ���������� ����� � 8 ���� ������)

	unsigned long 	cRecv = 2;
	BYTE		recvBuff[2]; //������ ��� �������� ������ ����� (��� �������� SW1 � SW2)
	LONG		ret; //���������� ��� �������� ������������� ������

	ret = SCardTransmit((SCARDHANDLE)cardHD, SCARD_PCI_T0, caseKeyFile, 7, NULL, recvBuff, &cRecv); //�������� ������ �� ����� ��������� �����

	if (ret == SCARD_S_SUCCESS) //��������� ��� ������� �������� ��������� ����� ��������� �������
	{
		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00))
		{
			for (int i = 0; i < 8; i++)
				newPassWd[5 + i] = (BYTE)pass[i];


			ret = SCardTransmit(cardHD, SCARD_PCI_T0, newPassWd, 13, NULL, recvBuff, &cRecv); //�������� ������ �� ��������� ������

			if (ret == SCARD_S_SUCCESS) //��������� ��� ������� �������� ��������� ����� ��������� �������
			{
				if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //���� ������� ������ ������� �������
					return SMART_SUCCESS; //���������� "�����"
				else ///���� �� �������� �����������? ���� ������ ������?
				{
					*err = (recvBuff[0] << 8) + recvBuff[1];
					return SMART_ERR_INT; //���������� "���������� ������"
				}
			}
			else
			{
				*err = (unsigned int)ret;
				return SMART_ERR_INT; //���������� "���������� ������"
			}
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //���������� "���������� ������"
		}
	}
	else
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //���������� "���������� ������"
	}
}

int smartCardRead(SMART_CARD_HANDLE cardHD, unsigned int addr, unsigned int size, unsigned char *data, unsigned int *err)
{
	*err = 0;

	BYTE		selectFile[] = { 0x00, 0xa4, 0x00, 0x00, 0x02, 0x2f, 0x02 }; //������� ������ ������ ������� ����� �����
	BYTE		readCard[] = { 0x00, 0xb0, 0x00, 0x00, 0x00 }; //������ ������� ������
															   //data = (char*)malloc(size); //������ ��� ������������� ������ ����� ������ 

	if ((addr >= 0) && (addr <= 1535)) //���� ����� ���� ��� ������ ����� ����
	{
		if (((addr + size) > 1536) || (size == 0))
			return SMART_ERR_SIZE;
	}
	else
		return SMART_ERR_ADDR;

	if (addr > 1023)  //� ����� ����� ����������� ������ � ������������� ������ � �������� ����� �����
		selectFile[6] = 0x04; //������ � ������� �����
	else
		if (addr > 511)
			selectFile[6] = 0x03;

	addr = addr % 512; //�������������� ����� � �������� ������ �����

	unsigned short countFile = (int)((addr + size - 1) / 512 + 1); //���������� ����� ������, � ������� ���������� ������
	unsigned short countBytes = size; //������� ���� ����� ��������
	unsigned short writeB = 0; //���������� ��� ��������� ����

	for (int i = 0; i < countFile; i++)
	{
		LONG			ret;
		unsigned long	cRecv; //���������� ��� ���������� ���� ������������� ������
		BYTE			*recvBuff; //������ ��� �������� ������������� ������

		cRecv = 2;
		recvBuff = (BYTE*)malloc(cRecv);

		ret = SCardTransmit(cardHD, SCARD_PCI_T0, selectFile, 7, NULL, recvBuff, &cRecv); //�������� ������ ���� ��� ������

		if (ret != SCARD_S_SUCCESS) ///���� ����� �� ������, ��� ������?
		{
			*err = (unsigned int)ret;
			return SMART_ERR_INT; //���������� "���������� ������"
		}


		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //���� �� ����� ����� ������ �������
		{

			free(recvBuff); //����������� ����� ������

			if (countFile == 3) //���������� ����� ���� ��� ������ � �������� ������ �����
			{ //���� ��� �����
				if (i == 0)
					countBytes = 512 - addr; //���������� ���� �� ������� �����
				if (i == 1)
					countBytes = 512; //���������� ���� �� ������� �����
				if (i == 2) //���������� ���� �� �������� �����
					countBytes = size - writeB;
			}
			else
				if (countFile == 2)
				{//���� �������� ��� �����
					if (i == 0)
						countBytes = 512 - addr; //���������� ���� �� ������� �����
					if (i == 1)
						countBytes = size - writeB; //���������� ���� �� ������� �����
				}

			int countCom = (countBytes - 1) / 64 + 1; //���������� ������ ������ � �������� ������ �����

			for (int j = 0; j < countCom; j++)
			{
				BYTE	readCard[] = { 0x00, 0xb0, 0x00, 0x00, 0x00 };

				if (j == countCom - 1) //���� ������ ������� �� ������ ��������� � �������� �������� �����
				{

					readCard[2] = (BYTE)(addr >> 8); //���������� �����, � �������� ���������� ������
					readCard[3] = (BYTE)addr;
					readCard[4] = (BYTE)(countBytes - j * 64); //���������� ������� ���� ����� �����������

					cRecv = 2 + readCard[4]; //��������� ������ ��� �������� ������ �����
					recvBuff = (BYTE*)malloc(cRecv);

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, readCard, 5, NULL, recvBuff, &cRecv); //���������� ������� ������ �� �����

					if (ret != SCARD_S_SUCCESS) ///���� ������� �� ���������, ��� ������?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //���������� "���������� ������"
					}

					if ((recvBuff[cRecv - 2] == 0x90) && (recvBuff[cRecv - 1] == 0x00)) //���� ������� ��������� �������
					{
						for (int p = 0; p < cRecv - 2; p++)
							data[writeB + j * 64 + p] = (unsigned char)recvBuff[p]; //���������� ��������� ������ � �������� ������


					}
					else ///���� ������� ������ ��������� � �������, ��� ������?
					{
						*err = (recvBuff[cRecv - 2] << 8) + recvBuff[cRecv - 1];
						return SMART_ERR_INT; //���������� "���������� ������"
					}


					addr = 0; //������ ���������� ����� �������� � �������� ������
					writeB += countBytes; //����������� ����� ��� ��������� ������

				}
				else
				{
					cRecv = 2 + 64; //��������� ������ ��� �������� ������ � �����
					recvBuff = (BYTE*)malloc(cRecv);

					readCard[2] = (BYTE)(addr >> 8);//���������� �����, � �������� ���������� ������
					readCard[3] = (BYTE)addr;
					readCard[4] = 0x40; //���������� ������� ���� ����� ����������� (64 �����)

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, readCard, 5, NULL, recvBuff, &cRecv); //���������� ������� ������ �� �����

					if (ret != SCARD_S_SUCCESS) ///���� ������� �� ���������, ��� ������?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //���������� "���������� ������"
					}

					if ((recvBuff[cRecv - 2] == 0x90) && (recvBuff[cRecv - 1] == 0x00)) //���� ������� ��������� �������
					{
						for (int p = 0; p < cRecv - 2; p++) //���������� ��������� ������ � �������� ������
							data[writeB + j * 64 + p] = (unsigned char)recvBuff[p];
					}
					else ///���� ������� ������ ��������� � �������, ��� ������?
					{
						*err = (recvBuff[cRecv - 2] << 8) + recvBuff[cRecv - 1];
						return SMART_ERR_INT; //���������� "���������� ������"
					}

					free(recvBuff); //����������� ������ ������ �����
					addr = addr + 64; //�������� ������� ���������� (�����) �� 64 �����
				}
			}
			selectFile[6]++; //�������� ��������� ����
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //���������� "���������� ������"
		}
	}

	return SMART_SUCCESS; //���������� ������
}

int smartCardWrite(SMART_CARD_HANDLE cardHD, unsigned int addr, unsigned int size, unsigned char *data, unsigned int *err)
{
	*err = 0;

	//������ ������ �� �����.
	//�� ���� �������� ���������� ���� SMART_CARD_HANDLE, � ������� �������� ������������� �����. ��������� ����� (addr) � ������ �����, � �������� ����� ������������� ������ ������. ����������
	//���� (size), ���������� �� ����� ��� ������. � ����� ��������� �� ������ ������ (data), � ������� �������� ���������� ��� �� ���������� �� �����
	//������� ����������: ���� ����������� �� ���� �������� - SMART_ERR_AUTH. ���� addr ��� size �� ����� - �� SMART_ERR_ADDR ��� SMART_ERR_SIZE ��������������. ���� ��������� �������, �� ������ 
	//SMART_SUCCESS

	BYTE		selectFile[] = { 0x00, 0xa4, 0x00, 0x00, 0x02, 0x2f, 0x02 }; //������� ������ ������� ����� ��� ������

	if ((addr >= 0) && (addr <= 1535)) //���� ����� ������ ������ � ���������� ��������
	{
		if (((addr + size) > 1536) || (size == 0)) //���� �������� �� ����� �� ��������� ������ � ����� ����������� ����� ����� 0
			return SMART_ERR_SIZE; //���������� ������ ������� ����������� ����
	}
	else //� ��������� ������ (���� ����� �� ��������� ������)
		return SMART_ERR_ADDR; //���������� ������ ������

	if (addr > 1023)  //� ����� ����� ����������� ������ � ������������� ������ � �������� ����� �����
		selectFile[6] = 0x04; //������ � ������� �����
	else
		if (addr > 511)
			selectFile[6] = 0x03; //������ �� ������ �����

	addr = addr % 512; //�������������� ����� � �������� ������ �����

	unsigned short countFile = (int)((addr + size - 1) / 512 + 1); //���������� ������, � ������� ������������ ������

	unsigned short countBytes = size; //����� ���� ��� ������
	unsigned short writeB = 0; //����� ���������� ����

	for (int i = 0; i < countFile; i++)
	{
		LONG			ret;
		unsigned long	cRecv = 2; //���������� ���� ������
		BYTE			recvBuff[2]; //����� ��� �������� ������

		ret = SCardTransmit(cardHD, SCARD_PCI_T0, selectFile, 7, NULL, recvBuff, &cRecv); //���������� ������� �� �����

		if (ret != SCARD_S_SUCCESS) ///���� ����� ����� �� ������, ��� ������?
		{
			*err = (unsigned int)ret;
			return SMART_ERR_INT; //���������� "���������� ������"
		}


		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //���� ����� ������� �������
		{
			if (countFile == 3) //���� ���������� ������ ��� ������ ����� ����
			{
				if (i == 0)
					countBytes = 512 - addr; //���������� ���� ��� ������ �� ������� �����
				if (i == 1)
					countBytes = 512; //���������� ���� ��� �� �� ������� �����
				if (i == 2)
					countBytes = size - writeB; //���������� ���� ��� ������ �� �������� �����
			}
			else
				if (countFile == 2) //���� ���������� ������ ��� ������ ����� ����
				{
					if (i == 0)
						countBytes = 512 - addr; //���������� ���� ��� ������ � ������ ����
					if (i == 1)
						countBytes = size - writeB; //���������� ���� ��� ������ �� ������ ����
				}

			int countCom = (countBytes - 1) / 64 + 1; //���������� ������, ���������� ����� � �������� ������ �����

			for (int j = 0; j < countCom; j++)
			{
				BYTE		*writeCard; //��������� �� ������ �������
				long		cWrite; //���������� ����� ���������� �������

				if (j == countCom - 1) //���� ������� ��� ������ ��������� � �������� ����� �����
				{
					cWrite = 5 + countBytes - j * 64; //���������� ���� �������� �������
					writeCard = (BYTE*)malloc(cWrite); //�������� ������ ��� �������

					writeCard[0] = 0x00; //���������� ���� CLA
					writeCard[1] = 0xd6; //���������� ���� INS
					writeCard[2] = (BYTE)(addr >> 8); //���������� ������
					writeCard[3] = (BYTE)addr;
					writeCard[4] = (BYTE)(countBytes - j * 64); //���������� ���� "���������� ������������ ����"

					for (int k = j * 64; k < countBytes; k++)
						writeCard[5 + (k % 64)] = (BYTE)data[writeB + k]; //������ ������������ ���� � ������

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, writeCard, cWrite, NULL, recvBuff, &cRecv); //��������� ������� �� �����

					if (ret != SCARD_S_SUCCESS) ///��� ������, ���� ������� �� ������� �� ���������� �� �����?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //���������� "���������� ������"
					}

					if ((recvBuff[0] != 0x90) && (recvBuff[1] != 0x00)) ///��� ������, ���� ������� �� �������?
					{
						*err = (recvBuff[0] << 8) + recvBuff[1];
						return SMART_ERR_INT; //���������� "���������� ������"
					}


					addr = 0; //������ � ��������� ����� ���������� � �������� ������
					writeB = writeB + countBytes; //����������� ���������� ��� ���������� �� ����� ����
					free(writeCard); //����������� ������, ���������� ��� ������� ������ �� �����
				}
				else //���� ������� �� ��������� � �������� �������� �����
				{
					cWrite = 69; //����� ������� (5 ���� ���������� ����� � 64 ����� ������)
					writeCard = (BYTE*)malloc(cWrite); //�������� ������ ��� �������

					writeCard[0] = 0x00; //CLA
					writeCard[1] = 0xd6; //INS
					writeCard[2] = (BYTE)(addr >> 8); //�����
					writeCard[3] = (BYTE)addr;
					writeCard[4] = 0x40; //���������� ������������ ���� 64

					for (int k = j * 64; k < (j + 1) * 64; k++)
						writeCard[5 + (k % 64)] = (BYTE)data[writeB + k]; //���������� ������ � �������

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, writeCard, cWrite, NULL, recvBuff, &cRecv); //���������� ������� �� �����

					if (ret != SCARD_S_SUCCESS) ///��� ������, ���� ������� �� ���������� �� �����?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //���������� "���������� ������"
					}

					if ((recvBuff[0] != 0x90) && (recvBuff[1] != 0x00)) ///��� ������, ���� ������� �� �������?
					{
						*err = (recvBuff[0] << 8) + recvBuff[1];
						return SMART_ERR_INT; //���������� "���������� ������"
					}

					free(writeCard); //����������� ������ �������
					addr = addr + 64; //��������� �����, � �������� ����� ������������� ������ ��������� �� ���������� 64 ������� �� �����������
				}
			}
			selectFile[6]++; //����� ���������� ��������� �������, ���������� ������� ��������� ���� � �����
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //���������� "���������� ������"
		}
	}

	return SMART_SUCCESS; //� ������ ��������� ���������� ���������� �����
}

int getSerialNum(SMART_CARD_HANDLE cardHD, unsigned char *data, unsigned int *err)
{
	*err = 0;
	//������� �������� �� ���� ����������, �������� � ���� ������������� ������������ ����� �

	//���������� ������ �� 6 ������ (� ������ ��������� ���������� ���� �������, � ������� ������ ��� ����� - ������� 9000, � ����������� - �������� �����)
	//� ������, ���� ����� ��������� ������ �������� ������ 2 ����� (�������), ���������� � ���� ��� ������

	LONG 	ret; //��� �������� ������������� ��������
	BYTE	sendBuff[] = { 0x00, 0xCA, 0x01, 0x00, 0x04 }; //������� ��� ��������� ��������� ������
	DWORD	cRecv = 6; //������ ���������� ������ (2 ����� ��� SW1 � SW2 � 4 ����� �� �������� �����)
	BYTE	*recvBuff = recvBuff = (BYTE*)malloc(cRecv); //�������� ������ ��� ������ ����������

	ret = SCardTransmit(cardHD, SCARD_PCI_T0, sendBuff, 5, NULL, recvBuff, &cRecv); //������ �������

	if (ret != SCARD_S_SUCCESS) ///��� ���������� ���� ������� �� �������� �� �����
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //���������� "���������� ������"
	}
	else //���� ����� �������
	{
		if ((recvBuff[cRecv - 2] == 0x90) && (recvBuff[cRecv - 1] == 0x00)) //��������� �� ���� ��������� ����� � ���� ������� �������
		{
			for (int i = 0; i < cRecv - 2; i++) //����� ���������� ������ � �������� ������
				data[i] = (unsigned char)recvBuff[i];
		}
		else ///���� ����� ������� ������, ��� ����������?
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //���������� "���������� ������"
		}
	}

	return SMART_SUCCESS;
}