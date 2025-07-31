#pragma once
#include "Event.h"
#include "Chief.h"
#include <iostream>
using namespace std;

class Order
{
private:
	int OrderID; char OrderType; double price;
	int Ordersize; int priority; int GT;
	int requestT; int FinishT;
	int waitT; int serveT;
	Chief* assign_chief;

public:
	int getOrderID() const;
	char getOrderType() const;
	double getPrice() const;
	int getOrderSize() const;
	int getPriority() const;
	int getRequestT() const;
	int getFinishT()const;
	int getWaitingT() const;
	int getServeT() const;
	Chief* get_assign_chief()const;
	int getAssignT() const;

	void setOrderID(int id);
	void setOrderType(char type);
	void setPrice(double p);
	void setOrderSize(int size);
	void setPriority(int pri);
	void setRequestT(int RT);
	void setFinishT(int FT);
	void setWaititngT(int t);
	void setServeT(int t);
	void set_assign_chief(Chief* ch);
	void setAssignT(int time);

	void calcPriority();
	void print() const;
};