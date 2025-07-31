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
    cout << "Event Time: " << getEventTime()
        << ", Order ID: " << getOrderID() << endl;
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

char Arrival::getType() const
{
    return 'A';
}

void Arrival::print() const
{
    Event::print();
    cout << "Type: " << getOrderType() << ", Size: " << size
        << ", Price: " << getPrice()<<endl;
}

void Arrival::Execute(Restaurant* Rest)
{
    Order* newOrder = new Order();

    newOrder->setOrderID(orderID);
    newOrder->setOrderType(orderType);
    newOrder->setOrderSize(size);
    newOrder->setPrice(price);
    newOrder->setRequestT(eventTime);

    if (orderType == 'N')
        Rest->addNormalOrder(newOrder);
    else if (orderType == 'G')
        Rest->Insert_vegan_order(newOrder);
    else if (orderType == 'V') {
        newOrder->calcPriority();
        Rest->addToVIPWait(newOrder);
    }
}

void Cancel::print() const
{
    Event::print();
}

void Cancel::Execute(Restaurant* Rest)
{
    Rest->CancelOrder(orderID);
}

char Cancel::getType() const
{
    return 'X';
}

void Promotion::print() const
{
    Event::print();
    cout << "Extra Money: " << extraMoney<<endl;
}

void Promotion::Execute(Restaurant* Rest) {
    Rest->promoteOrder(orderID, extraMoney);
}

char Promotion::getType() const
{
    return 'P';
}


