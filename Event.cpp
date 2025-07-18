#include "Event.h"
#include "Order.h"
#include "Restaurant.h"
#include <iostream>
using namespace std;

int Event::getEventTime() const
{
     return eventTime; 
}

int Event::getOrderID() const
{
    return orderID;
}

void Event::print() const
{
    cout << "Event Time: " << eventTime
        << ", Order ID: " << orderID << endl;
}

char Arrival:: getOrderType() const { 
    return orderType;
}
int Arrival:: getSize() const { 
    return size;
}
double Arrival::getPrice() const {
    return price;
}

void Arrival::print() const
{
    Event::print();
    cout << "Type: " << orderType << ", Size: " << size
        << ", Price: " << price;
}

void Arrival::Execute(Restaurant* Rest)
{
    Order* newOrder = new Order(orderID, orderType, price,size);

    /*if (orderType == 'N')
        Rest->addToNormalWait(newOrder);
    else if (orderType == 'G')
        Rest->addToVeganWait(newOrder);*/
    /*else*/ if (orderType == 'V')
        Rest->addToVIPWait(newOrder);
}

void Cancel::print() const
{
    Event::print();
}

void Cancel::Execute(Restaurant* Rest)
{
    Rest->cancelOrder(orderID);
}

void Promotion::print() const
{
    Event::print();
    cout << "Extra Money: " << extraMoney;
}

void Promotion::Execute(Restaurant* Rest) {
    Rest->promoteOrder(orderID, extraMoney);
}


