#include "Order.h"
#include <iostream>
using namespace std;

int Order::getOrderID() const {
	return OrderID;
}

char Order::getOrderType() const {
	return OrderType;
}

double Order::getPrice() const {
	return price;
}

int Order::getOrderSize() const
{
	return Ordersize;
}

int Order::getPriority() const {
	return priority;
}
int Order::getRequestT() const
{
	return requestT;
}

int Order::getFinishT() const
{
	return FinishT;
}
int Order::get_assign_chief()const
{
	return  assign_chief->getChiefID();
}


void Order::setOrderID(int id) {
	OrderID = id;
}

void Order::setOrderType(char type) {
	OrderType = type;
}

void Order::setPrice(double p) {
	price = p;
}

void Order::setOrderSize(int size) {
	Ordersize = size;
}

void Order::setPriority(int pri) {
	priority = pri;
}

void Order::setRequestT(int RT) {
	requestT = RT;
}

void Order::setFinishT(int FT) {
	FinishT = FT;
}

void Order::set_assign_chief(Chief* ch)
{
	assign_chief = ch;
}


void Order::calcPriority()
{
	priority = (price / Ordersize) + (100 - requestT);

}

void Order::print() const {
	cout << "OrderID: " << OrderID << ", Order Type: " << OrderType
		<< ", Price: $" << price << ", Order Size: " << Ordersize
		<< "Requested time" << requestT << "Finish Time " << FinishT;
}



