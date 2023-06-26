#pragma hdrstop
#include "Header.h"
#include <random>

class cassa
{
    HANDLE hUpdateEvt; //событие "данные обновились"
    HANDLE hStopEvt;   //событие "конец работы"

    HANDLE hMutex;  //защита данных

    HANDLE hThread; //дескриптор потока
    int timer;

    Clients* all_clients;

public:
    cassa(Clients& clients);
    ~cassa();

    bool isWorking();
    void startWork();
    void stopwork();
    int gettime();
    void changeTime();

private:
    static DWORD WINAPI ThreadFunc(LPVOID ptr);
};

cassa::cassa(Clients& clients)
{
    //Событие с ручным управлением, изначально активное
    hUpdateEvt = CreateEvent(NULL, TRUE, TRUE, NULL);
    //Событие с ручным управлением, изначально неактивное
    hStopEvt = CreateEvent(NULL, TRUE, TRUE, NULL);
    //Мютекс, пока свободный
    hMutex = CreateMutex(NULL, FALSE, NULL);
    //поток пока не запущен
    hThread = INVALID_HANDLE_VALUE;

    all_clients = &clients;
}
//закрытие деструкторов
cassa::~cassa()
{
    CloseHandle(hUpdateEvt);
    CloseHandle(hStopEvt);
    CloseHandle(hMutex);
}

void   cassa::startWork()
{
    if (hThread != INVALID_HANDLE_VALUE) return; //уже запущен
    ResetEvent(hStopEvt); //перед началом работы сбрасываем флаг остановки
    hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL); //запускаемся в работу
}

//остановка работы кассы
void cassa::stopwork()
{
    if (hThread == INVALID_HANDLE_VALUE) return;
    SetEvent(hStopEvt);

    if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
    {
        TerminateThread(hThread, 1);
    }
    CloseHandle(hThread);
    hThread = INVALID_HANDLE_VALUE;

}

//проверка на то работает ли касса или нет
bool cassa::isWorking()
{
    return !(WaitForSingleObject(hStopEvt, 0) == WAIT_OBJECT_0);
}


void cassa::changeTime()
{
    RMutexLocker mutex(hMutex);
    timer -= 1;
}


DWORD WINAPI cassa::ThreadFunc(LPVOID ptr)
{
    cassa* _this = (cassa*)ptr;
    if (!_this) return 1; //забыли нам дать объект, не будем работать


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, 4);

    for (;;)
    {
        if (_this->all_clients->getClients() == 0) {

            if (WaitForSingleObject(_this->hStopEvt, 1000) != WAIT_TIMEOUT) {
                _this->timer = 0;
                break;
            }

            continue; // если нет клиентов оставляем кассиров на рабочем месте (пусть пашут)
        }

        _this->timer = dis(gen);
        _this->all_clients->endWork();

        while (_this->gettime() > 0)
        {

            Sleep(1000);
            _this->changeTime();
                
        }

        if (WaitForSingleObject(_this->hStopEvt, 1000) != WAIT_TIMEOUT) { // условие выхода из бесконечного цикла
            _this->timer = 0;
            break;
        }

        //SetEvent(_this->hStopEvt);
    }
 
    return 0;
}

int cassa::gettime()
{
    RMutexLocker mutex(hMutex);
    return timer;
}
