#include <iostream>
#include <fstream>
#include <string>
#include "Event.h" 
#include "Chief.h"
#include "Restaurant.h"
#include "LinkedQueue.h"
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


bool Restaurant::cancelEvent(int orderID)
{
    return CancelOrder(orderID);
}

bool Restaurant::promoteOrder(int orderID, double extraMoney)
{
    LinkedQueue<Order*> tempQueue;
    Order* currentOrder = nullptr;
    bool found = false;

    while (!WaitNorm.isEmpty()) {
        WaitNorm.dequeue(currentOrder);

        if (currentOrder->getOrderID() == orderID && !found) {
            found = true;
            currentOrder->setPrice(currentOrder->getPrice() + extraMoney);
            currentOrder->calcPriority();
            VIPWaitList.enqueue(currentOrder, currentOrder->getPriority());
        }
        else
            tempQueue.enqueue(currentOrder);
    }
    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(currentOrder);
        WaitNorm.enqueue(currentOrder);
    }
    return found;
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
     char type = Chief->getchiefspecialization();
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Restaurant::addNormalOrder(Order* order) {
    WaitNorm.enqueue(order);
}
bool Restaurant::CancelOrder(int id)
{
    LinkedQueue<Order*> tempQueue;
    Order* currentOrder = nullptr;
    bool found = false;

    while (!WaitNorm.isEmpty()) {
        WaitNorm.dequeue(currentOrder);

        if (currentOrder->getOrderID() == id) {
            found = true;
            continue;
        }

        tempQueue.enqueue(currentOrder);
    }
    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(currentOrder);
        WaitNorm.enqueue(currentOrder);
    }
    return found;
}

Order* Restaurant::GetNormalOrder(int id)
{
    LinkedQueue<Order*> tempQueue;
    Order* currentOrder = nullptr;
    Order* foundOrder = nullptr;

    while (!WaitNorm.isEmpty()) {
        WaitNorm.dequeue(currentOrder);

        if (currentOrder->getOrderID() == id) {
            foundOrder = currentOrder;
        }

        tempQueue.enqueue(currentOrder);
    }
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(currentOrder);
        WaitNorm.enqueue(currentOrder);
    }
    return foundOrder;
}

void Restaurant::LoadInputFile(string fileName) {
    ifstream inputFile;
    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        cout << "Could not open the file." << endl;
        return;
    }

    int nNormal, nVegan, nVIP;
    inputFile >> nNormal >> nVegan >> nVIP;

    int sNormal, sVegan, sVIP;
    inputFile >> sNormal >> sVegan >> sVIP;

    int bAfter, bTime;
    inputFile >> bAfter >> bTime;

    for (int i = 0; i < nNormal; i++) {
        Chief* ch = new Chief(i + 1, 'N', sNormal, bAfter, bTime);
        readyNormalChefs.enqueue(ch);
    }
    for (int i = 0; i < nVegan; i++) {
        Chief* ch = new Chief(i + 1, 'G', sVegan, bAfter, bTime);
        Ready_Vegan_chief.enqueue(ch);
    }
    for (int i = 0; i < nVIP; i++) {
        Chief* ch = new Chief(i + 1, 'V', sVIP, bAfter, bTime);
        VIPChiefs.enqueue(ch);
    }
    inputFile.close();
}

void Restaurant::PrintReadyNormalChefs() {
    LinkedQueue<Chief*> tempQueue;
    Chief* chefPtr = nullptr;

    cout << "Ready Normal Chefs:\n";

    while (!readyNormalChefs.isEmpty()) {
        readyNormalChefs.dequeue(chefPtr);
        chefPtr->print();  // use Chef's print function
        tempQueue.enqueue(chefPtr);  // keep them safe
    }

    // Restore the original queue

    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(chefPtr);
        readyNormalChefs.enqueue(chefPtr);
    }
}

void  Restaurant::PrintAll()const  {
    LinkedQueue<Order*> temp = WaitNorm;
    Order* current = nullptr;
   
    while (!temp.isEmpty()) {
        temp.dequeue(current);
        cout << "Order ID: " << current->getOrderID();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//vegan_orders
bool Restaurant::Insert_vegan_order(Order* vo)
{
    Waiting_Vegan.enqueue(vo);
    return true;
}

bool Restaurant::Remove_vegan_order(Order* vo)
{
    LinkedQueue<Order*>temp;
    Order* vegan;
    bool Removed = false;
    while (Waiting_Vegan.dequeue(vegan))
    {
        if (vegan == vo)
        {
            Removed = true;
            continue;
        }
        temp.enqueue(vegan);
    }
    while (temp.dequeue(vegan))
    {
        Waiting_Vegan.enqueue(vegan);
    }
    return Removed;
}

bool Restaurant:: Searchfor_vegan_order(Order* vo)
{
    LinkedQueue<Order*> temp;
    Order* O;
    bool found = false;
    if (Waiting_Vegan.isEmpty())
    {
        return false;
    }
    while (Waiting_Vegan.dequeue(O))
    {
        temp.enqueue(O);
        if (O == vo)
        {
            found = true;
        }
    }
    while (temp.dequeue(O))
    {
        Waiting_Vegan.enqueue(O);
    }
    return found;
}

Order Restaurant::get_next_vegan_order(Node<Order>* curr)
{
    Order next;
    if (!curr->getNext())
    {
        return Order();
    }
    next = curr->getNext()->getItem();
    return next;
}


// vegan_chiefs
bool Restaurant:: Insert_vegan_chief(Chief* ch)
{
    Ready_Vegan_chief.enqueue(ch);
    return true;
}

bool Restaurant::Remove_vegan_chief(Chief* ch)
{
    LinkedQueue<Chief*>temp;
    Chief* vegan_chief;
    bool Removed = false;
    while (Ready_Vegan_chief.dequeue(vegan_chief))
    {
        if (vegan_chief == ch)
        {
            Removed = true;
            continue;
        }
        temp.enqueue(vegan_chief);
    }
    while (temp.dequeue(vegan_chief))
    {
        Ready_Vegan_chief.enqueue(vegan_chief);
    }
    return Removed;
}

bool Restaurant::Searchfor_available_vegan_chief(Chief* ch)
{
    LinkedQueue<Chief*> temp;
    Chief* chief;
    bool found = false;
    if (Ready_Vegan_chief.isEmpty())
    {
        return false;
    }
    while (Ready_Vegan_chief.dequeue(chief))
    {
        temp.enqueue(chief);
        if (chief == ch)
        {
            found = true;
        }
    }
    while (temp.dequeue(chief))
    {
        Ready_Vegan_chief.enqueue(chief);
    }
    return found;
}

Chief Restaurant::get_next_available_vegan_chief(Node<Chief>* curr_chief)
{
    Chief Available_next_chief;
    if (!curr_chief->getNext())
    {
        return Chief();
    }
    Available_next_chief = curr_chief->getNext()->getItem();
    return Available_next_chief;
}

void Restaurant::print_all_available_vegan_chiefs()
{
    LinkedQueue<Chief*>temp;
    Chief* ch;
    if (Ready_Vegan_chief.isEmpty())
    {
        return;
    }
    while (Ready_Vegan_chief.dequeue(ch))
    {
        temp.enqueue(ch);
        cout << " Chief : " << ch->getChiefID() << endl;
    }
    while (temp.dequeue(ch))
    {
        Ready_Vegan_chief.enqueue(ch);
    }
}


//In_service_orders
bool Restaurant::Insert_order(Order* O)
{
    In_service_orders.enqueue(O, O->getFinishT());
    return true;
}

bool Restaurant::Remove_done_order_from_Inservice(int current_time)
{
    priQueue<Order*>temp;
    Order* O;
    int ft;
    bool removed = false;
    while (In_service_orders.dequeue(O, ft))
    {
        if (O->getFinishT() <= current_time)
        {
            removed = true;
            continue;
        }
        temp.enqueue(O, ft);
    }
    while (temp.dequeue(O, ft))
    {
        In_service_orders.enqueue(O, ft);
    }
    return removed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//vip cheifs 
bool Restaurant ::addChef(Chief* chef) {
    VIPChiefs.enqueue(chef);
    return true;   
}
bool Restaurant::isEmpty() const {
    return VIPChiefs.isEmpty();
}

bool Restaurant::getNextChef(Chief* chef) {
    return VIPChiefs.dequeue(chef);
}

bool Restaurant::peekNextChef(Chief* chef) const {
    return VIPChiefs.peek(chef);
}

void Restaurant::printAllVIPChiefs() const {
    if (VIPChiefs.isEmpty()) {
        cout << "No VIP chefs available.\n";
        return;
    }
    LinkedQueue<Chief*> tempQueue = VIPChiefs;
    Chief* temp = nullptr;
    cout << "Available VIP Chefs:\n";
    while (tempQueue.dequeue(temp)) {
        cout << "ChefID: " << temp->getChiefID()
            << " | Speed: " << temp->getspeed()
            << " | Available: " << (temp->get_available() ? "Yes" : "No") << "\n";
    }
}

//delivered Orders
bool Restaurant::push(Order * order) {
    return DeliveredOrders.push(order);
}

bool Restaurant::pop(Order*& order) {
    if (DeliveredOrders.isEmpty())
        return false;
    DeliveredOrders.pop(order);
    return true;
}

bool Restaurant::peek(Order*& order) const {
    if (DeliveredOrders.isEmpty())
        return false;
    DeliveredOrders.peek(order); 
    return true;
}

void Restaurant::printAll_delivered_order(ArrayStack<Order*>& DeliveredOrders)  {
      if (DeliveredOrders.isEmpty()) {
            cout << "No delivered orders.\n";
            return;
        }
        ArrayStack<Order*> tempStack;
        Order* tempOrder;
        cout << "Delivered Orders (Most recent first):\n";
        while (!DeliveredOrders.isEmpty()) {
            DeliveredOrders.pop(tempOrder);
            cout << "FT: " << tempOrder->getFinishT()
                << " | ID: " << tempOrder->getOrderID()
                << " | RT: " << tempOrder->getRequestT()
                << " | WT: " << tempOrder->getWaitingT()
                << " | ST: " << tempOrder->getServeT() << "\n";
            tempStack.push(tempOrder);
        }
        while (!tempStack.isEmpty()) {
            tempStack.pop(tempOrder);
            DeliveredOrders.push(tempOrder);
        }
    }




