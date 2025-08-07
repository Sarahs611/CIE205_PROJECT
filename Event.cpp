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
    Event* ev = nullptr;
    cout << "Event Time: " << ev->getEventTime()
        << ", Order ID: " << ev->getOrderID() << endl;
}

string Arrival::getOrderType() const {
    return orderType;
}
int Arrival::getSize() const {
    return size;
}
double Arrival::getPrice() const {
    return price;
}

string Arrival::getType() const
{
    return "R";
}

void Arrival::print() const
{
    Event::print();
    Arrival* av = nullptr;

    cout << "Type: " << av->getOrderType() << ", Size: " << av->size
        << ", Price: " << av->getPrice() << endl;
}

void Arrival::Execute(Restaurant* Rest)
{


    Order* newOrder = new Order();

    newOrder->setOrderID(orderID);
    newOrder->setOrderType(orderType);
    newOrder->setOrderSize(size);
    newOrder->setPrice(price);
    newOrder->setRequestT(eventTime);

    if (orderType == "N")
        Rest->addNormalOrder(newOrder);
    else if (orderType == "G")
        Rest->Insert_vegan_order(newOrder);
    else if (orderType == "V") {
        newOrder->calcPriority();
        Rest->addToVIPWait(newOrder);
    }
    else if (orderType == "D")
    {
        Rest->Insert_dessert_order(newOrder);
    }
    else
        Rest->Insert_Drink_order(newOrder);
}

void Cancel::print() const
{
    Event::print();
}

void Cancel::Execute(Restaurant* Rest)
{
    Rest->CancelOrder(orderID);
}

string Cancel::getType() const
{
    return "X";
}

void Promotion::print() const
{
    Event::print();
    Promotion* p = nullptr;

    cout << "Extra Money: " << p->extraMoney << endl;
}

void Promotion::Execute(Restaurant* Rest) {
    Rest->promoteOrder(orderID, extraMoney);
}

string Promotion::getType() const
{
    return "P";
}


