#pragma once
#include "Event.h"
class Order 
{
private:
	int OrderID;        
	char OrderType;   
	double price;  
	int Ordersize;
	int priority;
	int requestT;

public:
	Order(int id, char orderType, double p, int s) {
		OrderID = id; OrderType = orderType;
		price = p; Ordersize = s; 
		requestT = 0;
		priority = 0;
	}
	int getOrderID() const;
	char getOrderType() const;
	double getPrice() const;
	int getOrderSize() const;
	int getPriority() const;
	int getRequestT() const;

	void calcPriority();
	void print() const;
};
