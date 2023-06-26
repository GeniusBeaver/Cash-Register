#pragma hdrstop
#include "Header.h"
#include <random>

class cassa
{
    HANDLE hUpdateEvt; //������� "������ ����������"
    HANDLE hStopEvt;   //������� "����� ������"

    HANDLE hMutex;  //������ ������

    HANDLE hThread; //���������� ������
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
    //������� � ������ �����������, ���������� ��������
    hUpdateEvt = CreateEvent(NULL, TRUE, TRUE, NULL);
    //������� � ������ �����������, ���������� ����������
    hStopEvt = CreateEvent(NULL, TRUE, TRUE, NULL);
    //������, ���� ���������
    hMutex = CreateMutex(NULL, FALSE, NULL);
    //����� ���� �� �������
    hThread = INVALID_HANDLE_VALUE;

    all_clients = &clients;
}
//�������� ������������
cassa::~cassa()
{
    CloseHandle(hUpdateEvt);
    CloseHandle(hStopEvt);
    CloseHandle(hMutex);
}

void   cassa::startWork()
{
    if (hThread != INVALID_HANDLE_VALUE) return; //��� �������
    ResetEvent(hStopEvt); //����� ������� ������ ���������� ���� ���������
    hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL); //����������� � ������
}

//��������� ������ �����
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

//�������� �� �� �������� �� ����� ��� ���
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
    if (!_this) return 1; //������ ��� ���� ������, �� ����� ��������


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

            continue; // ���� ��� �������� ��������� �������� �� ������� ����� (����� �����)
        }

        _this->timer = dis(gen);
        _this->all_clients->endWork();

        while (_this->gettime() > 0)
        {

            Sleep(1000);
            _this->changeTime();
                
        }

        if (WaitForSingleObject(_this->hStopEvt, 1000) != WAIT_TIMEOUT) { // ������� ������ �� ������������ �����
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
