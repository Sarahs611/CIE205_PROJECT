#include "Order.h"
#include <iostream>
using namespace std;

int Order::getOrderID() const {
	return OrderID; }

char Order::getOrderType() const {
	return OrderType; }

double Order::getPrice() const { 
	return price;
}

int Order::getSize() const
{
	return size;
}

int Order::getPriority() const {
	return priority;
}
int Order::getRequestT() const
{
	return requestT;
}

void Order::calcPriority()
{
	priority = (price / size) + (100 - requestT);

}

void Order::print() const {
    cout << "OrderID: " << OrderID<< ", Type: " << OrderType
        << ", Price: $" << price<< ", Priority: " << priority;
}
