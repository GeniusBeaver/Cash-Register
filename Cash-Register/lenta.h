#pragma once
#include "cassa.h"
#include <vector>


class lenta {

private: 

	Clients* clients = new Clients();
	std::vector<cassa*> shop;
	std::vector<bool> iswork = {true, true, true};

	HANDLE hThread; //дескриптор потока

public:

	lenta() : shop({new cassa(*clients), new cassa(*clients), new cassa(*clients)}) { hThread = INVALID_HANDLE_VALUE; }
	//запускает дескриптор потока `hThread`, который перемещает клиентов из очереди кассы в зависимости от её состояния и наличия свободных касс
	void startwork() 
	{
		hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL); //запускаемся в работу
	}
	//возвращает строку "ON" либо "OFF"  
	std::string checkWork(int number)
	{
		if (shop[number]->isWorking())
			return "ON";
		else
			return "OFF";
	}
	//возвращает время прошедшее с начала работы кассы с указанным номером
	int checkTime(int number)
	{
		return shop[number]->gettime();
	}
	//возвращает кол-во клиентов
	int getClients()
	{
		return clients->getClients();
	}
	// возвращает кол-во работающих касс
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
	// меняем состояние кассы работает или не работает
	bool changeWork(int number)
	{
		if (iswork[number]) {
			shop[number]->stopwork();
		} else shop[number]->startWork();

		iswork[number] = !iswork[number];
		return iswork[number];
	}

private:
	//проверка кассы на занятость в отдельном потоке
	static DWORD WINAPI ThreadFunc(LPVOID ptr)
	{
		lenta* _this = (lenta*)ptr;
		if (!_this) return 1; //забыли нам дать объект, не будем работать

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
