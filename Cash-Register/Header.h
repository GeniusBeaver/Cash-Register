#include <windows.h>

#define ID_TIMER1 1020
#define IDC_STATIC -1
#define IDC_CASSA_1 1001
#define IDC_CASSA_2 1002
#define IDC_CASSA_3 1003
#define IDC_CASSA_TIME_1 1007
#define IDC_CASSA_TIME_2 1008
#define IDC_CASSA_TIME_3 1009
#define IDC_CLIENTS 1004
#define IDC_TIME 1005
#define IDC_NOW_CLIENTS 1006

#define IDC_BUTTON_1 1010
#define IDC_BUTTON_2 1011
#define IDC_BUTTON_3 1012
#define IDC_START 1013

//��������������� ����� ������� �������� �������� ������.
class RMutexLocker
{
    HANDLE hMutex;
public:
    RMutexLocker(HANDLE mutex)
        :hMutex(mutex)
    {
        WaitForSingleObject(hMutex, INFINITE);
    }
    //���������� 
    ~RMutexLocker()
    {
        ReleaseMutex(hMutex);
    }
};


class Clients
{
private:
    int clients = 10;

    HANDLE hMutex;  //������ ������
    
public:
    //�������� �������� ��� �������
    Clients()
    {
        hMutex = CreateMutex(NULL, FALSE, NULL);
    }
    //������� ��������� ���������� ��������� �������
    ~Clients()
    {
        CloseHandle(hMutex);
    }
    //���������� ���-�� ��������
    int getClients()
    {
        RMutexLocker mutex(hMutex);
        return clients;
    }
    // ��������� ���-�� ������� �� 1
    void endWork()
    {
        RMutexLocker mutex(hMutex);
        --clients;
    }
};