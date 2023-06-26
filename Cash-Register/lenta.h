#pragma once
#include "cassa.h"
#include <vector>


class lenta {

private: 

	Clients* clients = new Clients();
	std::vector<cassa*> shop;
	std::vector<bool> iswork = {true, true, true};

	HANDLE hThread; //���������� ������

public:

	lenta() : shop({new cassa(*clients), new cassa(*clients), new cassa(*clients)}) { hThread = INVALID_HANDLE_VALUE; }
	//��������� ���������� ������ `hThread`, ������� ���������� �������� �� ������� ����� � ����������� �� � ��������� � ������� ��������� ����
	void startwork() 
	{
		hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL); //����������� � ������
	}
	//���������� ������ "ON" ���� "OFF"  
	std::string checkWork(int number)
	{
		if (shop[number]->isWorking())
			return "ON";
		else
			return "OFF";
	}
	//���������� ����� ��������� � ������ ������ ����� � ��������� �������
	int checkTime(int number)
	{
		return shop[number]->gettime();
	}
	//���������� ���-�� ��������
	int getClients()
	{
		return clients->getClients();
	}
	// ���������� ���-�� ���������� ����
	int checkCl()
	{
		int _temp = 0;
		if (shop[0]->isWorking())
			++_temp;
		if (shop[1]->isWorking())
			++_temp;
		if (shop[2]->isWorking())
			++_temp;

		return _temp;
	}
	// ������ ��������� ����� �������� ��� �� ��������
	bool changeWork(int number)
	{
		if (iswork[number]) {
			shop[number]->stopwork();
		} else shop[number]->startWork();

		iswork[number] = !iswork[number];
		return iswork[number];
	}

private:
	//�������� ����� �� ��������� � ��������� ������
	static DWORD WINAPI ThreadFunc(LPVOID ptr)
	{
		lenta* _this = (lenta*)ptr;
		if (!_this) return 1; //������ ��� ���� ������, �� ����� ��������

		while (_this->clients->getClients() > 0)
		{
			if (!_this->shop[0]->isWorking() && _this->iswork[0]) {
				_this->shop[0]->startWork();
			}

			else if (!_this->shop[1]->isWorking() && _this->iswork[1]) {
				_this->shop[1]->startWork();
			}

			else if (!_this->shop[2]->isWorking() && _this->iswork[2]) {
				_this->shop[2]->startWork();
			}
			Sleep(1000);
		}

		//_this->shop[0]->stopwork();
		//_this->shop[1]->stopwork();
		//_this->shop[2]->stopwork();

		return 0;
	}
};
