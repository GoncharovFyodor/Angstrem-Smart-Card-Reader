#include "stdafx.h"
#ifndef SMARTCARDLIB_H
#define SMARTCARDLIB_H

#define SMART_ERR_INT	-1	//���������� ������, ���������� �� ������ ����� ���� �������� �������� smartInternalState
#define SMART_SUCCESS	0	//������ ���
#define SMART_ERR_PARAM 1	//������ � ��������� (����� ������� � ������)
#define SMART_ERR_CARD	2	//��� �����
#define SMART_ERR_CINV	3	//�������� �����
#define SMART_ERR_AUTH	4	//����� �� ���� ������������
#define SMART_ERR_SIZE	5	//������� ������ �������� size ��� ������ ������ ��� ������
#define SMART_ERR_ADDR	6	//������� ������ �������� addr ��� ������ ������ ��� ������

typedef unsigned char* SMART_READER_HANDLE;
typedef unsigned int SMART_CARD_HANDLE;


//������������� �����������. � ������� ������ ���������� SMART_SUCCESS, ����� SMART_ERR_INT
int smartReaderInit(SMART_READER_HANDLE *hd, unsigned int *err);

//���������� ��� ������(��� � ����� /usr/include/PCSC/pcsclite.h)
void smartInternalState(unsigned int err);

//�������� ������� �����. � ������ ������ ���������� SMART_SUCCESS, ����� SMART_ERR_CARD
int smartCardPresent(SMART_READER_HANDLE readerHD, unsigned int *err);

//�������� �����. � ������ ������ ��������� SMART_SUCCESS. ���� ����� ��� - �� SMART_ERR_CARD. ���� ����� ������� - �� SMART_ERR_CINV
int smartCardOpen(SMART_READER_HANDLE readerHD, SMART_CARD_HANDLE *cardHD, unsigned int *err);

//����������� �����. � ������ ������ ��������� SMART_SUCCESS. ���� ����������� �� ������ - SMART_ERR_AUTH. � ������ ������� � remain ������������ ���������� ���������� �������
int smartCardAuth(SMART_CARD_HANDLE cardHD, const char *pass, int *remain, unsigned int *err); ///

//����� ������ �� �����. � ������ ������ ��������� SMART_SUCCESS. ���� ����������� �� ���� �������� - SMART_ERR_AUTH.
int smartCardPasswd(SMART_CARD_HANDLE cardHD, const char *pass, unsigned int *err); ///

//������ ���������� �� �����. � ������ ������ ���������� SMART_SUCCESS. 
//���� ����������� �� ���� �������� - SMART_ERR_AUTH. ���� addr ��� size �� ����� - �� SMART_ERR_ADDR ��� SMART_ERR_SIZE ��������������
int smartCardRead(SMART_CARD_HANDLE cardHD, unsigned int addr, unsigned int size, unsigned char *data, unsigned int *err);

//������ ���������� � �����. � ������ ������ ��������� SMART_SUCCESS. 
//���� ����������� �� ���� �������� - SMART_ERR_AUTH. ���� addr ��� size �� ����� - �� SMART_ERR_ADDR ��� SMART_ERR_SIZE ��������������
int smartCardWrite(SMART_CARD_HANDLE cardHD, unsigned int addr, unsigned int size, unsigned char *data, unsigned int *err);

//���������� �������� �����
int getSerialNum(SMART_CARD_HANDLE cardHD, unsigned char *data, unsigned int *err);

#endif // SMARTCARDLIB_H


