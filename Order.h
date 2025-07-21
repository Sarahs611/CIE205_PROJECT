#pragma once
#include "Event.h"
#include "Chief.h"
class Order
{
private:
	int OrderID; char OrderType; double price;
	int Ordersize; int priority;
	int requestT; int FinishT;
	Chief* assign_chief;


public:
	int getOrderID() const;
	char getOrderType() const;
	double getPrice() const;
	int getOrderSize() const;
	int getPriority() const;
	int getRequestT() const;
	int getFinishT()const;
	int get_assign_chief()const;

	void setOrderID(int id);
	void setOrderType(char type);
	void setPrice(double p);
	void setOrderSize(int size);
	void setPriority(int pri);
	void setRequestT(int RT);
	void setFinishT(int FT);
	void set_assign_chief(Chief* ch);


	void calcPriority();
	void print() const;
};
