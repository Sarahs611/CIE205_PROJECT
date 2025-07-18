#include <iostream>
#include "Event.h"  
#include "Restaurant.h"
using namespace std;


void Restaurant::addEvent(Event* newEvent) {
    EventsList.enqueue(newEvent);
}

Event* Restaurant::getNextEvent() {
    Event* next = nullptr;
    if (EventsList.peek(next)) {
        EventsList.dequeue(next);
        return next;
    }
    return nullptr;
}

void Restaurant::printEvents() const {
    cout << "events list: "; EventsList.Print();
}

int Restaurant::countEvents() const {
    return EventsList.getCount();
}



Order* Restaurant::getNextVIPOrder()  {
     Order* next = nullptr;
     int priority = 0;
      if (VIPWaitList.dequeue(next, priority)) {
           return next;
       }
  return nullptr;   
}

void Restaurant::addToVIPWait(Order* pOrder){
    VIPWaitList.enqueue(pOrder, pOrder->getPriority());
}

int Restaurant::countVIPOrders() const{
  return VIPWaitList.getCount();
}

void Restaurant::printVIPOrders() const
{
    VIPWaitList.print();
}


bool Restaurant::cancelOrder(int orderID)
{
    return true;
}

bool Restaurant::promoteOrder(int orderID, double extraMoney)
{
    return true;
}


Chief* Restaurant::getReadyChief(char chiefType) {
     Chief* readyChief = nullptr;
    if (chiefType == 'N' && !InBreakN.isEmpty()) {
       InBreakN.dequeue(readyChief);
    }
    else if (chiefType == 'G' && !InBreakG.isEmpty()) {
       InBreakG.dequeue(readyChief);
    }
    else if (chiefType == 'V' && !InBreakVIP.isEmpty()) {
       InBreakVIP.dequeue(readyChief);
    }
        return readyChief;
}

void Restaurant::addChiefToBreak(Chief* Chief){
     char type = Chief->getCheifType(); 
  if (type == 'N') {
      InBreakN.enqueue(Chief);
        }
      else if (type == 'G') {
           InBreakG.enqueue(Chief);
      }
      else if (type == 'V') {
           InBreakVIP.enqueue(Chief);
      }
}

int Restaurant::countInBreakChiefs() const
{
    return InBreakN.getCount()+ InBreakG.getCount()+ InBreakVIP.getCount();
}
void Restaurant::printInBreakChiefs() const{
   
    cout << "In-Break normal Chiefs"; InBreakN.Print();

    cout << "in-Break vegan Chiefs"; InBreakG.Print();

    cout << "In-Break VIP Chiefs"; InBreakVIP.Print();
}

