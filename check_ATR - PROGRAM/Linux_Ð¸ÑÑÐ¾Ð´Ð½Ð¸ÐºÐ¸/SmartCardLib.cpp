#include <stdio.h>
#include <winscard.h>
#include <reader.h>
#include <stdlib.h>
#include "SmartCardLib.h"

using namespace std;

int smartReaderInit(SMART_READER_HANDLE *hd, unsigned int *err)
{
	*err = 0;
	//функция инициализации считывателя
	//на вход получает указатель на переменную типа SMART_READER_HANDLE, в которую запишется имя ридера, подключенного к системе
	//на выходе возвращает SMART_SUCCESS если функция выполнена успешно и SMART_ERR_INT если функция выполнена с ошибкой

	SCARDCONTEXT	context; //переменная для хранения контекста системы
	LONG 			ret; //переменная для хранения возвращаемого значения функций библиотеки winscard
	LPTSTR			listReaders; //переменная для хранения имери ридера, подключенного к системе
	DWORD			cch = SCARD_AUTOALLOCATE; //автоматически определять длину имени ридера

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context); //собираем свелдения о системе

	ret = SCardListReaders(context, NULL, (LPTSTR)hd, &cch); //выясняем, есть ли подключенные к системе ридеры

	if (ret == SCARD_S_SUCCESS) //если команда на проверку подключенных к системе ридеров выполнена успешно
	{
		printf("Device: %s\n", *hd); //выводит имя подключенного к системе ридера на экран
		return SMART_SUCCESS; //возвращает "успех"
	}
	else //если выполнена с ошибкой
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT;  //возвращает "внутренняя ошибка"
	}

}

void smartInternalState(unsigned int err)
{
	if (err == 0x109)
		printf("Клиент попытался выполнить операцию смарт-карты в удаленном сеансе.\n");
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
					printf("9001:\tОшибка чтения заголовка текущего файла.\n");
					break;
				case(0x02):
					printf("9002:\tОшибка чтения файла текущих команд.\n)");
					break;
				case(0x03):
					printf("9003:\tошибка восстанавливаемого файла.\n)");
					break;
				case(0x04):
					printf("9004:\tОшибка ФС.\n");
					break;
				case(0x05):
					printf("9005:\tФайл ATR не найден.\n");
					break;
				case(0x06):
					printf("9006:\tОшибка MF.\n");
					break;
				case(0x07):
					printf("9007:\tФайл ATR неисправен.\n");
					break;
				case(0x08):
					printf("9008:\tФайл ATR ошибка размера.\n");
					break;
				case(0x0d):
					printf("900D:\tОшибка текста.\n");
					break;
				case(0x0e):
					printf("900E:\tНедопустимый p3.\n");
					break;
				case(0x0f):
					printf("900F:\tОшибка чтения транспортного кода.\n");
					break;
				case(0x11):
					printf("9011:\tОшибка записи транспортного кода.\n");
					break;
				case(0x14):
					printf("9014:\tПопытка повторной блокировки.\n");
					break;
				case(0x15):
					printf("9015:\tНеудачная попытка блокировки.\n");
					break;
				case(0x16):
					printf("9016:\tКарта блокирована.\n");
					break;
				}
				break;

				/*case(0x61):
				printf("61%02x:\tЧисло доступных байт данных: %d.\n", w2, w2);
				break;*/

			case (0x62):
				//printf("62");
				switch (w2)
				{
				case(0x00):
					printf("6200:\tПредупреждение: состояние энергозависимой памяти без изменений. Информация не предоставлена.\n");
					break;
				case(0x01):
					printf("6201:\tПредупреждение (спецификация): ошибка памяти. Данные записаны верно. Карта 'мягко' блокирована.\n");
					break;
				case(0x02):
					printf("6202:\tПредупреждение (спецификация): файловая система повреждена.\n");
					break;
				case (0x81):
					printf("6281:\tПредупреждение: состояние энергонезависимой памяти без изменений. Часть выдаваемых данных может быть искажена.\n");
					break;
				case(0x82):
					printf("6282:\tПредупреждение (спецификация): чтение за границей файла.\n");
					break;
				case(0x83):
					printf("6283:\tПредупреждение: состояние энергонезависимой памяти без изменений. Выбранный файл недействителен.\n");
					break;
				case(0x84):
					printf("6284:\tПредупреждение: состояние энергонезависимой памяти без изменений. Контрольная информация файла не форматирована по 5.3.3.\n");
					break;
				case(0x85):
					printf("6285:\tПредупреждение: состояние энергонезависимой памяти без изменений. Выбранный файл в стадии завершения.\n");
					break;
				case(0x86):
					printf("6286:\tПредупреждение: состояние энергонезависимой памяти без изменений. Нет входных данных на карту, полученных от датчиков.\n");
					break;
				case(0x87):
					printf("6287:\tПредупреждение: состояние энергонезависимой памяти без изменений. Как минимум одна запись деактивирована.\n");
					break;
				default:
					if ((w2 >= 0x03) && (w2 <= 0x80))
						printf("62%02x:\tПредупреждение: состояние энергонезависимой памяти без изменений. Активация карты.\n", w2);
					break;
				}
				break;

			case(0x63):
				//printf("63");
				switch (w2)
				{
				case(0x00):
					printf("6300:\tПредупреждение: состояние энергозависимой памяти изменено. Информация не предоставлена.\n");
					break;
				case(0x81):
					printf("6381:\tПредупреждение: состояние энергозависимой памяти изменено. Файл заполнен при последней операции записи.\n");
					break;
				default:
					if ((w2 >> 4) == 0x0C)
						printf("63%02x:Предупреждение: ошибка аутентификации.\n", w2);
					break;
				}
				break;

			case(0x64):
				//printf("64");
				switch (w2)
				{
				case(0x00):
					printf("6400:\tОшибка спецификации: ошибка памяти. Данные недоступны.\n");
					break;
				case(0x01):
					printf("6401:\tОшибка стандарта: состояние энергонезависимой памяти без изменений. Непосредственный ответ, требуемый картой.\n");
					break;
				default:
					if ((w2 >= 0x02) && (w2 <= 0x80))
						printf("64%02x:\tОшибка стандарта: состояние энергонезависимой памяти без изменений. Активация карты.\n", w2);
					break;
				}
				break;

			case (0x65):
				//printf("65");
				switch (w2)
				{
				case(0x00):
					printf("6500:\tОшибка стандарта: состояние энергонезависимой памяти изменено. Ошибка выполнения.\n");
					break;
				case(0x01):
					printf("6501:\tОшибка спецификации: ошибка памяти. Данные не записаны или записаны с ошибкой. Карта 'мягко' блокирована.\n");
					break;
				case(0x81):
					printf("6581:\tОшибка стандарта: состояние энергонезависимой памяти изменено. Отказ памяти.\n");
					break;
				}
				break;

			case(0x66):
				printf("66%02x:\tОшибка стандарта: для сообщений, связанных с безопасностью.\n", w2);
				break;

			case(0x67):
				switch (w2)
				{
				case(0x00):
					printf("6700:\tОшибка спецификации: неверная длина данных.\n");
					break;
				}
				break;

			case(0x68):
				//printf("68");
				switch (w2)
				{
				case(0x00):
					printf("6800:\tОшибка стандарта: функции, указанные в байте CLA, не поддерживаются. Информация не предоставлена.\n");
					break;
				case(0x81):
					printf("6881:\tОшибка стандарта: функции, указанные в байте CLA, не поддерживаются. Логический канал не поддерживается.\n");
					break;
				case(0x82):
					printf("6882:\tОшибка стандарта: функции, указанные в байте CLA, не поддерживаются. Безопасный обмен сообщениями не поддерживается.\n");
					break;
				case(0x83):
					printf("6883:\tОшибка стандарта: функции, указанные в байте CLA, не поддерживаются. Ожидается последняя команда в цепочке.\n");
					break;
				case(0x84):
					printf("6884:\tОшибка стандарта: функции, указанные в байте CLA, не поддерживаются. Сцепление команд не поддерживается.\n");
					break;
				}
				break;

			case(0x69):
				//printf("69");
				switch (w2)
				{
				case(0x00):
					printf("6900:\tОшибка стандарта: команда не разрешена. Информация не предоставлена.\n");
					break;
				case(0x81):
					printf("6981:\tОшибка спецификации: команда не применима в данном контексте (не применима к данному типу файла).\n");
					break;
				case(0x82):
					printf("6982:\tОшибка предъвления транспортного кода.\n");
					break;
				case(0x83):
					printf("6983:\tКончились попытки предъявления ТК.\n");
					break;
				case(0x84):
					printf("6984:\tОшибка спецификации: ключ блокирован.\n");
					break;
				case(0x85):
					printf("6985:\tОшибка спецификации: не выполнены условия использования команды (не выполнились команды ASK_RANDOM или SEND_RANDOM, ...).\n");
					break;
				case(0x86):
					printf("6986:\tНесовместимая инструкция.\n");
					break;
				case(0x87):
					printf("6987:\tОшибка стандарта: команда не разрешена. Пропажа ожидаемых информационных объектов, связанных с безопасным обменом сообщениями.\n");
					break;
				case(0x88):
					printf("6988:\tОшибка спецификации: неверный сертификат сообщения.\n");
					break;
				case(0x80):
					printf("6980:\tОшибка стандарта: команда не разрешена. Некорректные параметры в поле данных команды.\n");
					break;
				case(0x89):
					printf("6989:\tОшибка стандарта: команда не разрешена. Файл уже существует.\n");
					break;
				case(0x8A):
					printf("698A:\tОшибка стандарта: команда не разрешена. Имя DF уже существует.\n");
					break;
				}
				break;

			case(0x6A):
				//printf("6A");
				switch (w2)
				{
				case(0x80):
					printf("6A80:\tОшибка спецификации: неприемлимое значение поля данных (файл с указанным идентификатором уже присутствует).\n");
					break;
				case(0x81):
					printf("6A81:\tОшибка спецификации: функция не реализована.\n");
					break;
				case(0x82):
					printf("6A82:\tОшибка спецификации: файл не найден.\n");
					break;
				case(0x83):
					printf("6A83:\tОшибка спецификации: запись не найдена.\n");
					break;
				case(0x84):
					printf("6A84:\tОшибка спецификации: недостаточно места для записи.\n");
					break;
				case(0x85):
					printf("6A85:\tОшибка спецификации: значение длины Lc не соответствует структуре TLV.\n");
					break;
				case(0x86):
					printf("6A86:\tОшибка спецификации: неверные параметры P1-P2.\n");
					break;
				case(0x87):
					printf("6A87:\tОшибка спецификации: значение длины Lc не соответствует P1-P2.\n");
					break;
				case(0x88):
					printf("6A88:\tОшибка спецификации: требуемые структуры данных (файл ключей) отсутствуют.\n");
					break;
				default:
					printf("6A%02x:\tОшибка стандарта: неверно указанный(е) параметр(ы) P1-P2", w2);
					break;
				}
				break;

			case(0x6B):
				switch (w2)
				{
				case(0x00):
					printf("6B00:.\tОшибка спецификации: неверные параметры Р1-Р2 - смещение за границей файла.\n");
					break;
				}
				break;

			case(0x6C):
				printf("6C%02x:\tОшибка спецификации: неверный размер запрашиваемых в ответе данных.\n", w2);
				break;

			case(0x6D):
				switch (w2)
				{
				case(0x00):
					printf("6D00:\tНедопустимая инструкция.\n");
					break;
				}
				break;

			case(0x6E):
				switch (w2)
				{
				case(0x00):
					printf("6E00:\tНедопустимый класс.\n");
					break;
				}
				break;

			case(0x6F):
				switch (w2)
				{
				case(0x00):
					printf("6F00:\tОшибка спецификации: неизвестная ошибка.\n");
					break;
				case(0x82):
					printf("6F82:\tОшибка BUP файла.\n");
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
				printf("Произошла ошибка при установке указателя на объект файла смарт-карты.\n");
				break;
			case(0x02):
				printf("Действие было отменено запросом SCardCancel.\n");
				break;
			case(0x0E):
				printf("Система не распоряжается носителями запрошенным способом.\n)");
				break;
			case(0x1C):
				printf("Смарт-карта не отвечает минимальным требованиям.\n)");
				break;
			case(0x2D):
				printf("Запрашиваемый сертификат не может быть получен.\n");
				break;
			case(0x2F):
				printf("Ошибка связи со смарт-картой.\n");
				break;
			case(0x23):
				printf("На смарт-карте отсутствует запрашиваемый каталог.\n");
				break;
			case(0x1B):
				printf("Драйвер считывателя не вернул имя счивателя.\n");
				break;
			case(0x24):
				printf("На смарт-карте отсутствует запрашиваемый файл.\n");
				break;
			case(0x21):
				printf("Запрашиваемый порядок создания объектов не поддерживается.\n");
				break;
			case(0x20):
				printf("Невозможно найти первичный провайдер для смарт-карты.\n");
				break;
			case(0x08):
				printf("Буфер для возвращаемых данных слишком мал.\n");
				break;
			case(0x15):
				printf("Полученный ATR не является допустимым.\n");
				break;
			case(0x2A):
				printf("Указанный PIN-код неверен.\n");
				break;
			case(0x03):
				printf("Указанный handle неверен.\n");
				break;
			case(0x04):
				printf("Невозможно правильно интерпретировать один или несколько входных параметров.\n");
				break;
			case(0x05):
				printf("Информация о запуске реестра отсутствует либо недействительна.\n");
				break;
			case(0x11):
				printf("Невозможно правильно интерпретировать одно или несколько заданных значений параметров.\n");
				break;
			case(0x27):
				printf("Доступ к файлу запрещен.\n)");
				break;
			case(0x25):
				printf("Конечный адрес указанного пути не является каталогом.\n)");
				break;
			case(0x26):
				printf("Конечный адрес указанного пути не является файлом.\n");
				break;
			case(0x30):
				printf("На карте отсутствует запрашиваемый контейнер ключа.\n");
				break;
			case(0x06):
				printf("Недостаточно памяти для выполнения команды.\n");
				break;
			case(0x33):
				printf("Невозможно кэшировать PIN-код карты.\n");
				break;
			case(0x2E):
				printf("Отсутствует устройство чтения смарт-карт.\n");
				break;
			case(0x1D):
				printf("Не запущен диспетчер ресурсов смарт-карты.\n");
				break;
			case(0x0C):
				printf("Для запрашиваемой операции требуется смарт-карта, которая в настоящее время отсутствует.\n");
				break;
			case(0x2C):
				printf("Запрашиваемого сертификата не существует.\n");
				break;
			case(0x10):
				printf("Считыватель (или смарт-карта) не готов принимать команды.\n");
				break;
			case(0x16):
				printf("Была предпринята попытка завершения несуществующей транзакции.\n");
				break;
			case(0x19):
				printf("Буфер приема PCI слишком мал.\n");
				break;
			case(0x32):
				printf("Срок действия PIN-кода карты истек.\n");
				break;
			case(0x0F):
				printf("Запрашиваемые протоколы несовместимы с протоколом, используемым картой.\n");
				break;
			case(0x34):
				printf("Смарт-карта доступна только для чтения.\n");
				break;
			case(0x17):
				printf("В настоящее время указанный считыватель недоступен.\n");
				break;
			case(0x1A):
				printf("Драйвер считываетля не отвечает минимальным требованиям.\n");
				break;
			case(0x31):
				printf("Диспетчер ресурсов смарт-карт занят и не может завершить эту операцию.\n");
				break;
			case(0x1E):
				printf("Диспетчер ресурсов смарт-карты отключен.\n");
				break;
			case(0x0B):
				printf("Невозможно получить доступ к смарт-карте.\n");
				break;
			case(0x12):
				printf("Система отменила действие (предположительно, чтобы выйти или выключиться).\n)");
				break;
			case(0x0A):
				printf("Указанное пользователем время (тайм-аут) истекло.\n)");
				break;
			case(0x1F):
				printf("Произошла непредвиденная ошибка карты.\n");
				break;
			case(0x0D):
				printf("Не распознается указанное имя смарт-карты.\n");
				break;
			case(0x09):
				printf("Не распознается указанное имя считывателя.\n");
				break;
			case(0x2B):
				printf("Был возвращен неизвестный код ошибки.\n");
				break;
			case(0x22):
				printf("Смарт-карта не поддерживает запрашиваемую команду.\n");
				break;
			case(0x28):
				printf("Была предпринята попытка записать больше данных, чем в целевом объекте.\n");
				break;
			case(0x13):
				printf("Обнаружена ошибка внутренней связи.\n");
				break;
			case(0x01):
				printf("Обнаружена ошибка внутренней согласованности.\n");
				break;
			case(0x14):
				printf("Обнаружена внутренняя ошибка. Источник ошибки неизвестен.\n");
				break;
			case(0x07):
				printf("Истек таймер внутренней согласованности.\n");
				break;
			case(0x18):
				printf("Операция была прервана.\n");
				break;
			case(0x6E):
				printf("Действие отмененно пользователем.\n");
				break;
			case(0x70):
				printf("Запрашиваемый элемент в кэше не обнаружен.\n");
				break;
			case(0x71):
				printf("Запрашиваемый элемент кэша слишком стар и был удален.\n");
				break;
			case(0x72):
				printf("Новый элемент кэша превышает максимально допустимый размер.\n");
				break;
			case(0x6F):
				printf("Карте не предъявлен PIN-код.\n");
				break;
			case(0x6C):
				printf("Доступ к карте невозможен. Кончились попытки ввода PIN-кода.\n");
				break;
			case(0x6D):
				printf("Достигнут конец файла смарт-карты.\n");
				break;
			case(0x69):
				printf("Связь с картой невозможна. Карта была удалена из считывателя.\n");
				break;
			case(0x68):
				printf("Смарт-карта сброшена (reset).\n)");
				break;
			case(0x6A):
				printf("Прекрашен доступ к карте по причине нарушения безопасности.\n)");
				break;
			case(0x67):
				printf("Питание карты было прекращено. Дальнейшая связь с смарт-картой невозможна.\n");
				break;
			case(0x66):
				printf("Смарт-карта не отвечает на сигнал сброса (reset).\n");
				break;
			case(0x65):
				printf("Считыватель не может связаться с смарт-картой из-за конфликтов конфигурации ATR.\n");
				break;
			case(0x6B):
				printf("Невозможно получить смарт-карту. Предъясвлен неверный PIN-код.\n");
				break;
			}
		}
}

int smartCardPresent(SMART_READER_HANDLE readerHD, unsigned int *err)
{
	*err = 0;

	//функция проверки присутствия карты в считывателе
	//на вход получает переменную типа SMART_READER_HANDLE, в которую хранится имя ридера, подключенного к системе
	//на выходе возвращает SMART_SUCCESS если функция выполнена успешно, SMART_ERR_CARD если считыватель без карты и SMART_ERR_INT если прочая ошибка

	SCARDCONTEXT	context; //переменная для хранения контекста системы
	LONG			ret; //переменная для хранения возвращаемого значения функций библиотеки winscard
	DWORD			dwActiveProtocol; //переменная для хранения протокола, по которому дейтсвует карта
	SCARDHANDLE		cardHandle; //переменная для хранения идентификатора карты, вставленной в ридер

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context); //собираем свелдения о системе
        ret = SCardConnect(context, (LPTSTR)readerHD, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &cardHandle, &dwActiveProtocol); //подключаемся к карте, вставленной в ридер

	if (ret != SCARD_S_SUCCESS) //если подключение к карте было с ошибкой
								//if (ret == SCARD_W_REMOVED_CARD) //если карты нет в ридере
								//	return SMART_ERR_CARD; //ввозвращает "ошибка карты"
								//else
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //если карта есть в ридере, но другая ошибка, значит возвращает "внутренняя ошибка"
	}

	return SMART_SUCCESS; //возвращает "успех"
}

int smartCardOpen(SMART_READER_HANDLE readerHD, SMART_CARD_HANDLE *cardHD, unsigned int *err)
{
	*err = 0;

	//Функция открытия карты.
	//На вход получает переменную типа SMART_READER_HANDLE, в которую хранится имя ридера, подключенного к системе и указатель на переменную SMART_CARD_HANDLE, в которую запишется идентификатор карты
	//В случае успеха возвращае SMART_SUCCESS. Если карты нет - то SMART_ERR_CARD, если карта неверна - то SMART_ERR_CINV, а если выполнена с ошибкой, то возвращает SMART_ERR_INT

	SCARDCONTEXT	context; //переменная для хранения контекста системы
	LONG			ret; //переменная для хранения возвращаемого значения функций библиотеки winscard

	BYTE			cardATR[] = { 0x3b, 0x69, 0x00, 0x00, 0x8f, 0x8c, 0x8e, 0x8f, 0x8c, 0x80, 0x01, 0x23, 0x00 }; //ожидаемый ATR с карты
	BYTE			*atr; //указатель на массив для получения реального ATR карты
	DWORD			atrByte = SCARD_AUTOALLOCATE; //размер массива для хранения реального ATR карты определять автоматически
	DWORD			dwActiveProtocol = SCARD_AUTOALLOCATE; //протокол работы карты определять автоматически

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context); //собираем свелдения о системе

	ret = SCardConnect(context, (LPTSTR)readerHD, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, (LPSCARDHANDLE)cardHD, &dwActiveProtocol); //подключаемся к карте, вставленной в ридер

	if (ret == SCARD_W_REMOVED_CARD) //если считыватель без карты
		return SMART_ERR_CARD; //возвращаем "ошибка карты"

	if (ret == SCARD_S_SUCCESS) //если подключение к рате выполнена успешно
	{
                ret = SCardGetAttrib(*cardHD, SCARD_ATTR_ATR_STRING, (LPBYTE)&atr, &atrByte); //получаем ATR

		if (atrByte != 13)					//если длина полученного ATR не совпадает с ожидаемой
			return SMART_ERR_CINV; //возвращаем "неверная карта"

		for (int i = 0; i < atrByte; i++) //проверяем на совпадение ATR реального и ожидаемого
			if (atr[i] != cardATR[i])		//если хотя бы одно значение из считанного ATR не совпадает с ожидаемым
				return SMART_ERR_CINV; //возвращаем "неверная карта"

		return SMART_SUCCESS; //если высе выполнено успешно, возвращаем "успех"
	}
	else
	{
		*err = (unsigned int)ret;
		SMART_ERR_INT; //возвращаем "внутренняя ошибка"
	}
}

int smartCardAuth(SMART_CARD_HANDLE cardHD, const char *pass, int *remain, unsigned int *err) //что записывать в remain в случае успеха? Если же карта заблокирована?
{
	*err = 0;

	//Авторизация карты.
	//На вход получает переменную типа SMART_CARD_HANDLE, в которой хранится идентификатор карты, указатель на переменную типа char, в которой хранится пароль, и переменная типа int в которую
	//в случаее если пароль не верен, возвращает количество оставшихся попыток ввода пароля до блокировки карты.
	//В случае успеха возвращае SMART_SUCCESS. Если авторизация не прошла - SMART_ERR_AUTH. В случае неудачи в remain записывается количество остовшихся попыток

	BYTE		caseKeyFile[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x00, 0x09 }; //команда для выбора файла ключей
	BYTE		verifyPassWd[13] = { 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //массив для команды подачи ключа (5 байт командного слова и 8 байт пароля)

	BYTE		recvBuff[2]; //массив для хранения ответа карты (для хранения SW1 и SW2)
	LONG		ret; //переменная для хранения возвращаемого ответа
	unsigned long 	cRecv = 2; //количесвто байт возвращаемого ответа

	ret = SCardTransmit((SCARDHANDLE)cardHD, SCARD_PCI_T0, caseKeyFile, 7, NULL, recvBuff, &cRecv); //посылаем запрос на выбор ключевого файла

	if (ret == SCARD_S_SUCCESS) //проверяем что команда отправки сообщения карте выполнена успешно
	{
		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //если команда выбора клбчевого файла выполнена успешно
		{
			for (int i = 0; i < 8; i++) //формируем команду дла аутентификации
				verifyPassWd[5 + i] = (BYTE)pass[i];

			ret = SCardTransmit(cardHD, SCARD_PCI_T0, verifyPassWd, 13, NULL, recvBuff, &cRecv); //посылаем запрос подтверждение пароля

			if (ret == SCARD_S_SUCCESS) //проверяем что команда отправки сообщения карте выполнена успешно
			{
				if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //если команда успешно выполнена и пароль принят
					return SMART_SUCCESS; //возвращаем "успех"
				else ///если прочие ошибки? //если же команда выполнена с ошибкой
					if (recvBuff[0] == 0x63) //если пароль не верен
					{
						*remain = int(recvBuff[1] & 0x0f); //записываем читсло оставшихся попыток в remain
						return SMART_ERR_AUTH; //возвращаем "ошибка аутентификации"
					}
					else
					{
						*err = (recvBuff[0] << 8) + recvBuff[1];
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}
			}
			else
			{
				*err = (unsigned int)ret;
				return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
			}
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}
	}
	else
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
	}
}

int smartCardPasswd(SMART_CARD_HANDLE cardHD, const char *pass, unsigned int *err) ///если авторизация не была пройдена?
{
	*err = 0;

	//Смена пароля на карте.
	//На вход получает переменную типа SMART_CARD_HANDLE, в которой хранится идентификатор карты, указатель на переменную типа char, в которой хранится новый пароль
	//В случае успеха возвращае SMART_SUCCESS. Если авторизация не прошла - SMART_ERR_AUTH

	BYTE		caseKeyFile[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x00, 0x09 }; //команда для выбора файла ключей
	BYTE		newPassWd[13] = { 0x00, 0xDC, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //массив для команды подачи нового ключа (5 байт командного слова и 8 байт пароля)

	unsigned long 	cRecv = 2;
	BYTE		recvBuff[2]; //массив для хранения ответа карты (для хранения SW1 и SW2)
	LONG		ret; //переменная для хранения возвращаемого ответа

	ret = SCardTransmit((SCARDHANDLE)cardHD, SCARD_PCI_T0, caseKeyFile, 7, NULL, recvBuff, &cRecv); //посылаем запрос на выбор ключевого файла

	if (ret == SCARD_S_SUCCESS) //проверяем что команда отправки сообщения карте выполнена успешно
	{
		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00))
		{
			for (int i = 0; i < 8; i++)
				newPassWd[5 + i] = (BYTE)pass[i];


			ret = SCardTransmit(cardHD, SCARD_PCI_T0, newPassWd, 13, NULL, recvBuff, &cRecv); //посылаем запрос на изменение пароля

			if (ret == SCARD_S_SUCCESS) //проверяем что команда отправки сообщения карте выполнена успешно
			{
				if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //если команда картой принята успешно
					return SMART_SUCCESS; //возвращаем "успех"
				else ///если не пройдена авторизация? если прочие ошибки?
				{
					*err = (recvBuff[0] << 8) + recvBuff[1];
					return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
				}
			}
			else
			{
				*err = (unsigned int)ret;
				return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
			}
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}
	}
	else
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
	}
}

int smartCardRead(SMART_CARD_HANDLE cardHD, unsigned int addr, unsigned int size, unsigned char *data, unsigned int *err)
{
	*err = 0;

	BYTE		selectFile[] = { 0x00, 0xa4, 0x00, 0x00, 0x02, 0x2f, 0x02 }; //команда выбора самого первого файла карты
	BYTE		readCard[] = { 0x00, 0xb0, 0x00, 0x00, 0x00 }; //шаблон команды чтения
															   //data = (char*)malloc(size); //массив для возвращаемого ответа длины ответа 

	if ((addr >= 0) && (addr <= 1535)) //если число байт для чтения равно нулю
	{
		if (((addr + size) > 1536) || (size == 0))
			return SMART_ERR_SIZE;
	}
	else
		return SMART_ERR_ADDR;

	if (addr > 1023)  //в каком файле еначинается чтение и интерпретация адреса в пределах этого файла
		selectFile[6] = 0x04; //чтение в третьем файле
	else
		if (addr > 511)
			selectFile[6] = 0x03;

	addr = addr % 512; //интерпретируем адрес в пределах одного файла

	unsigned short countFile = (int)((addr + size - 1) / 512 + 1); //вычисление числа файлов, в которых проводится чтение
	unsigned short countBytes = size; //сколько байт будет читаться
	unsigned short writeB = 0; //количество уже считанных байт
		
	for (int i = 0; i < countFile; i++)
	{
		LONG			ret;
		unsigned long	cRecv; //переменная для количества байт возвращаемого ответа
		BYTE			*recvBuff; //массив для хранения возвращаемого ответа

		cRecv = 2;
		recvBuff = (BYTE*)malloc(cRecv);

		ret = SCardTransmit(cardHD, SCARD_PCI_T0, selectFile, 7, NULL, recvBuff, &cRecv); //выбираем первый файл для чтения

		if (ret != SCARD_S_SUCCESS) ///если выбор не удался, что делать?
		{
			*err = (unsigned int)ret;
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}


		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //если же выбор файла прошел успешно
		{

			free(recvBuff); //освобождаем буфер ответа

			if (countFile == 3) //определяем число байт для чтения в пределах одного файла
			{ //если три файла
				if (i == 0)
					countBytes = 512 - addr; //количество байт из первого файла
				if (i == 1)
					countBytes = 512; //количество байт из второго файла
				if (i == 2) //количество байт из третьего файла
					countBytes = size - writeB;
			}
			else
				if (countFile == 2)
				{//если читается два файла
					if (i == 0)
						countBytes = 512 - addr; //количество байт из первого файла
					if (i == 1)
						countBytes = size - writeB; //количество байт из второго файла
				}

			int countCom = (countBytes - 1) / 64 + 1; //количество команд чтения в пределах одного файла

			for (int j = 0; j < countCom; j++)
			{
				BYTE	readCard[] = { 0x00, 0xb0, 0x00, 0x00, 0x00 };

				if (j == countCom - 1) //если данная команда на чтение последняя в пределах текущего файла
				{

					readCard[2] = (BYTE)(addr >> 8); //записываем адрес, с которого начинается чтение
					readCard[3] = (BYTE)addr;
					readCard[4] = (BYTE)(countBytes - j * 64); //записываем сколько байт будет считываться

					cRecv = 2 + readCard[4]; //формируем массив для хранения ответа карты
					recvBuff = (BYTE*)malloc(cRecv);

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, readCard, 5, NULL, recvBuff, &cRecv); //отправляем команду чтения на карту

					if (ret != SCARD_S_SUCCESS) ///если команда не выполнена, что делать?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}

					if ((recvBuff[cRecv - 2] == 0x90) && (recvBuff[cRecv - 1] == 0x00)) //если команда выполнена успешно
					{
						for (int p = 0; p < cRecv - 2; p++)
							data[writeB + j * 64 + p] = (unsigned char)recvBuff[p]; //записываем считанные данные в итоговый массив


					}
					else ///если команда чтения выполнена с ошибкой, что делать?
					{
						*err = (recvBuff[cRecv - 2] << 8) + recvBuff[cRecv - 1];
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}


					addr = 0; //чтение следующего файла начнется с нулевого адреса
					writeB += countBytes; //увеличиваем число уже считанных файлов

				}
				else
				{
					cRecv = 2 + 64; //формируем массив для хранение ответа с карты
					recvBuff = (BYTE*)malloc(cRecv);

					readCard[2] = (BYTE)(addr >> 8);//записываем адрес, с которого начинается чтение
					readCard[3] = (BYTE)addr;
					readCard[4] = 0x40; //записываем сколько байт будет считываться (64 байта)

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, readCard, 5, NULL, recvBuff, &cRecv); //отправляем команду чтения на карту

					if (ret != SCARD_S_SUCCESS) ///если команда не выполнена, что делать?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}

					if ((recvBuff[cRecv - 2] == 0x90) && (recvBuff[cRecv - 1] == 0x00)) //если команда выполнена успешно
					{
						for (int p = 0; p < cRecv - 2; p++) //записываем считанные данные в итоговый массив
							data[writeB + j * 64 + p] = (unsigned char)recvBuff[p];
					}
					else ///если команда чтения выполнена с ошибкой, что делать?
					{
						*err = (recvBuff[cRecv - 2] << 8) + recvBuff[cRecv - 1];
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}

					free(recvBuff); //освобождаем массив ответа карты
					addr = addr + 64; //сдвигаем каретку считывания (адрес) на 64 байта
				}
			}
			selectFile[6]++; //выбираем следующий файл
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}
	}

	return SMART_SUCCESS; //возвращаем массив
}

int smartCardWrite(SMART_CARD_HANDLE cardHD, unsigned int addr, unsigned int size, unsigned char *data, unsigned int *err)
{
	*err = 0;

	//Запись данных на карту.
	//На вход получает переменную типа SMART_CARD_HANDLE, в которой хранится идентификатор карты. Начальный адрес (addr) в памяти карты, с которого будет производиться запись данных. Количество
	//байт (size), подаваемых на карту для записи. А также указатель на массив данных (data), в котором хранится информация для ее сохранения на карте
	//Функция возвращает: если авторизация не была пройдена - SMART_ERR_AUTH. Если addr или size не верны - то SMART_ERR_ADDR или SMART_ERR_SIZE соответственно. если выполнена успешно, то вернет 
	//SMART_SUCCESS

	BYTE		selectFile[] = { 0x00, 0xa4, 0x00, 0x00, 0x02, 0x2f, 0x02 }; //команда выбора первого файла для записи

	if ((addr >= 0) && (addr <= 1535)) //если адрес начала чтения в допустимых границах
	{
		if (((addr + size) > 1536) || (size == 0)) //если читается до байта за пределами памяти и число считываемых байта равно 0
			return SMART_ERR_SIZE; //возвращаем ошибку размера считываемых байт
	}
	else //в противном случае (если адрес за пределами памяти)
		return SMART_ERR_ADDR; //возвращаем ошибку адреса

	if (addr > 1023)  //в каком файле еначинается запись и интерпретация адреса в пределах этого файла
		selectFile[6] = 0x04; //запись в третьем файле
	else
		if (addr > 511)
			selectFile[6] = 0x03; //запись во втором файле

	addr = addr % 512; //интерпретируем адрес в пределах одного файла

	unsigned short countFile = (int)((addr + size - 1) / 512 + 1); //количество файлов, в которые производится запись

	unsigned short countBytes = size; //число байт для чтения
	unsigned short writeB = 0; //число записанных байт

	for (int i = 0; i < countFile; i++)
	{
		LONG			ret;
		unsigned long	cRecv = 2; //количество байт ответа
		BYTE			recvBuff[2]; //буфер для хранения ответа

		ret = SCardTransmit(cardHD, SCARD_PCI_T0, selectFile, 7, NULL, recvBuff, &cRecv); //отправляем команду на карту

		if (ret != SCARD_S_SUCCESS) ///если выбор файла не удался, что делать?
		{
			*err = (unsigned int)ret;
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}


		if ((recvBuff[0] == 0x90) && (recvBuff[1] == 0x00)) //если карта приняла команду
		{
			if (countFile == 3) //если количество файлов для записи равно трем
			{
				if (i == 0)
					countBytes = 512 - addr; //количество байт для записи из первого файла
				if (i == 1)
					countBytes = 512; //количество байт для за из второго файла
				if (i == 2)
					countBytes = size - writeB; //количесвто байт для чтения из третьего файла
			}
			else
				if (countFile == 2) //если количество файлов для записи равно двум
				{
					if (i == 0)
						countBytes = 512 - addr; //количество байт для записи в первый файл
					if (i == 1)
						countBytes = size - writeB; //количество байт для записи во второй файл
				}

			int countCom = (countBytes - 1) / 64 + 1; //количество команд, посываемое карте в пределах одного файла

			for (int j = 0; j < countCom; j++)
			{
				BYTE		*writeCard; //указатель на массив команды
				long		cWrite; //количество баййт посылаемой команды

				if (j == countCom - 1) //если команда для записи последняя в пределах этого файла
				{
					cWrite = 5 + countBytes - j * 64; //количество байт итоговой команды
					writeCard = (BYTE*)malloc(cWrite); //выделяем память под команду

					writeCard[0] = 0x00; //заполнение поля CLA
					writeCard[1] = 0xd6; //заполнение поля INS
					writeCard[2] = (BYTE)(addr >> 8); //заполнение адреса
					writeCard[3] = (BYTE)addr;
					writeCard[4] = (BYTE)(countBytes - j * 64); //заполнение поля "количесвто передаваемых байт"

					for (int k = j * 64; k < countBytes; k++)
						writeCard[5 + (k % 64)] = (BYTE)data[writeB + k]; //запись передаваемых байт в массив

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, writeCard, cWrite, NULL, recvBuff, &cRecv); //полсылаем команду на карту

					if (ret != SCARD_S_SUCCESS) ///что делать, если команда не принята не отправлена на карту?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}

					if ((recvBuff[0] != 0x90) && (recvBuff[1] != 0x00)) ///что делать, если команда не принята?
					{
						*err = (recvBuff[0] << 8) + recvBuff[1];
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}


					addr = 0; //запись в следующем файле начинается с нулевого адреса
					writeB = writeB + countBytes; //увеличиваем количество уже записанных на карту байт
					free(writeCard); //освобождаем массив, выделенный под команду записи на карту
				}
				else //если команда не последняя в пределах текущего файла
				{
					cWrite = 69; //длина команды (5 байт командного слова и 64 байта данных)
					writeCard = (BYTE*)malloc(cWrite); //выделяем память под команду

					writeCard[0] = 0x00; //CLA
					writeCard[1] = 0xd6; //INS
					writeCard[2] = (BYTE)(addr >> 8); //адрес
					writeCard[3] = (BYTE)addr;
					writeCard[4] = 0x40; //количество записываемых байт 64

					for (int k = j * 64; k < (j + 1) * 64; k++)
						writeCard[5 + (k % 64)] = (BYTE)data[writeB + k]; //записываем данные в команду

					ret = SCardTransmit(cardHD, SCARD_PCI_T0, writeCard, cWrite, NULL, recvBuff, &cRecv); //отправляем команду на карту

					if (ret != SCARD_S_SUCCESS) ///что делать, если команда не отправлена на карту?
					{
						*err = (unsigned int)ret;
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}

					if ((recvBuff[0] != 0x90) && (recvBuff[1] != 0x00)) ///что делать, если команда не принята?
					{
						*err = (recvBuff[0] << 8) + recvBuff[1];
						return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
					}

					free(writeCard); //освобождаем массив команды
					addr = addr + 64; //следующий адрес, с которого будет производиться запись находится на расстоянии 64 позиций от предыдущего
				}
			}
			selectFile[6]++; //когда выполнится последняя команда, необходимо выбрать следующий файл в карте
		}
		else
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}
	}

	return SMART_SUCCESS; //в случае успешного выполнения возвращает успех
}

int getSerialNum(SMART_CARD_HANDLE cardHD, unsigned char *data, unsigned int *err)
{
	*err = 0;
	//функция получает на вход переменную, хранящую в себе идентификатор подключенной карты и

	//возвращает массив из 6 байтов (в случае успешного выполнения всей функции, в котором первые два байта - префикс 9000, а последующие - серийный номер)
	//в случае, если будет встречена ошибка вернется только 2 байта (префикс), содержищие в себе код ошибки

	LONG 	ret; //для хранения возвращаемого значения
	BYTE	sendBuff[] = { 0x00, 0xCA, 0x01, 0x00, 0x04 }; //команда для получения серийного номера
	DWORD	cRecv = 6; //размер ожидаемого ответа (2 байта для SW1 и SW2 и 4 байта на серийный номер)
	BYTE	*recvBuff = recvBuff = (BYTE*)malloc(cRecv); //выделяем память под массив результата

	ret = SCardTransmit(cardHD, SCARD_PCI_T0, sendBuff, 5, NULL, recvBuff, &cRecv); //подаем команду

	if (ret != SCARD_S_SUCCESS) ///что возвращать если команда не передана на карту
	{
		*err = (unsigned int)ret;
		return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
	}
	else //если ответ получен
	{
		if ((recvBuff[cRecv - 2] == 0x90) && (recvBuff[cRecv - 1] == 0x00)) //проверяем по коду состояние карты и если команда принята
		{
			for (int i = 0; i < cRecv - 2; i++) //затем записываем данные в выходной массив
				data[i] = (unsigned char)recvBuff[i];
		}
		else ///если карта вернула ошибку, что возвращать?
		{
			*err = (recvBuff[0] << 8) + recvBuff[1];
			return SMART_ERR_INT; //возвращаем "внутренняя ошибка"
		}
	}

	return SMART_SUCCESS;
}
