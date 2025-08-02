#include <iostream>
#include <fstream>
#include <string>
#include "Event.h" 
#include "Chief.h"
#include "Restaurant.h"
#include "LinkedQueue.h"
#include "UI.h"
#include <cmath>
using namespace std;


Restaurant::Restaurant(){
    AutoP = 0;
   totalWT = 0, totalST = 0;
   deliveredC = 0, promotedC = 0;
}

void Restaurant::SimulationT(int mode)
{
    int timestep = 1;

    if (mode == 2)
        cout << "Silent mode on.\n";
    while (!EventsList.isEmpty() || !VIPWaitList.isEmpty() || !WaitNorm.isEmpty()
        || !Waiting_Vegan.isEmpty() || !In_service_orders.isEmpty() || !InBreakN.isEmpty()
        || !InBreakG.isEmpty() || !InBreakVIP.isEmpty()) {

        ExecuteTimestep(timestep);

        if (mode == 1) {
            UI::printCurrentStatus(this,timestep);
            cout << "PRESS ENTER TO CONTINUE \n";
            cin.get(); 
        }
        timestep++;
    }
    if (mode == 2)
        cout << "Output file:\n";

    OutputFile();
}

void Restaurant::ExecuteTimestep(int timestep)
{
    // Execute events
    Event* ev = nullptr;
    int c = EventsList.getCount();
    for (int i = 0; i < c; i++)
    {
        if (!EventsList.peek(ev)) break;

        if (ev->getEventTime() > timestep)
            break;

        EventsList.dequeue(ev);
        if (ev)
            ev->Execute(this);

        delete ev;
    }

    //for n-orders waited too long 
    checkAutoPromote(timestep);

    assignOrders(timestep);

   // updateInServiceOrders
    priQueue<Order*> temp;
    Order* order = nullptr;
    int prio;

    while (!In_service_orders.isEmpty())
    {
        In_service_orders.dequeue(order, prio);

        if (order->getFinishT() <= timestep)
        {
            // Order is done
            push(order); 
            totalWT += order->getWaitingT();
            totalST += order->getServeT();
            deliveredC++;

            Chief* c = order->get_assign_chief();
            if (c->getordersbeforebreak() <= 0) {
                // Chef goes to break
                c->setBreakEndTime(timestep + c->getbreakduration());
                c->set_available(false);

                if (c->getchiefspecialization() == 'N')
                    InBreakN.enqueue(c);
                else if (c->getchiefspecialization() == 'G') 
                    InBreakG.enqueue(c);
                else if (c->getchiefspecialization() == 'V') 
                    InBreakVIP.enqueue(c);
            //reset
                c->setordersbeforebreak(5);
            }
            else {
                makeChiefready(c, timestep); 
            }
        }
        else
        {
            temp.enqueue(order, order->getFinishT());  //keep it
        }
    }   
    while (!temp.isEmpty())
    {
        temp.dequeue(order, prio);
        In_service_orders.enqueue(order, prio);
    }

    //chefs coming from/going break 
    updateChiefStatus(timestep);
}
void Restaurant::checkAutoPromote(int timestep)
{
    LinkedQueue<Order*> temp;
    Order* currentOrder = nullptr;

    while (!WaitNorm.isEmpty()) {
        WaitNorm.dequeue(currentOrder);

        int waitTime = timestep - currentOrder->getRequestT();
        if (waitTime >= AutoP) {
            currentOrder->calcPriority();
            currentOrder->setOrderType('V');
            addToVIPWait(currentOrder);
            promotedC++;
        }
        else {
            temp.enqueue(currentOrder);
        }
    }
    while (!temp.isEmpty()) {
        temp.dequeue(currentOrder);
        WaitNorm.enqueue(currentOrder);
    }
}
void Restaurant::assignOrders(int timestep)
{
    Order* order = nullptr;
    Chief* chief = nullptr;
    int prio;

    //VIP Orders to vip chief, Normal , then Vegan
    while (!VIPWaitList.isEmpty()) {

        if (!VIPChiefs.isEmpty()) {
            VIPChiefs.dequeue(chief);
        }
        else if (!readyNormalChefs.isEmpty()) {
            readyNormalChefs.dequeue(chief);
        }
        else if (!Ready_Vegan_chief.isEmpty()) {
            Ready_Vegan_chief.dequeue(chief);
        }
        else {
            break; // no cheif
        }

        VIPWaitList.dequeue(order, prio);
        assignOrderToChef(order, chief, timestep);
    }

    while (!Waiting_Vegan.isEmpty() && !Ready_Vegan_chief.isEmpty()) {
        Waiting_Vegan.dequeue(order);
        Ready_Vegan_chief.dequeue(chief);
        assignOrderToChef(order, chief, timestep);
    }

    while (!WaitNorm.isEmpty()) {
        if (!readyNormalChefs.isEmpty()) {
            readyNormalChefs.dequeue(chief);
        }
        else if (!VIPChiefs.isEmpty()) {
            VIPChiefs.dequeue(chief); 
        }
        else {
            break; 
        }
        WaitNorm.dequeue(order);
        assignOrderToChef(order, chief, timestep);
    }
}
void Restaurant::assignOrderToChef(Order* order, Chief* chef, int timestep)
{
    order->setAssignT(timestep); // GT
    order->setWaititngT(timestep - order->getRequestT()); //WT = GT - RT

    int ST = ceil(order->getOrderSize() / chef->getspeed());
    order->setServeT(ST); 
    order->setFinishT(timestep + ST); // FT = GT + ST

    order->set_assign_chief(chef); 

    //workload
    chef->setordersbeforebreak(chef->getordersbeforebreak() - 1);

    if (chef->getordersbeforebreak() < 0) {
        chef->setordersbeforebreak(0);
    }

    Insert_order(order);
}

void Restaurant::makeChiefready(Chief* ch, int time)
{
    if (!ch) return;

    ch->set_available(true);

    if (ch->getchiefspecialization() == 'N')
        readyNormalChefs.enqueue(ch);
    else if (ch->getchiefspecialization() == 'G')
        Ready_Vegan_chief.enqueue(ch);
    else
        VIPChiefs.enqueue(ch);
}
void Restaurant::updateChiefStatus(int timestep)
{
    Chief* ch;

    int sizeN = InBreakN.getCount();
    for (int i = 0; i < sizeN; i++) {
        InBreakN.dequeue(ch);
        if (ch->getBreakEndTime() <= timestep) {
            ch->set_available(true);
            readyNormalChefs.enqueue(ch);
        }
        else {
            InBreakN.enqueue(ch);
        }
    }

    int sizeG = InBreakG.getCount();
    for (int i = 0; i < sizeG; i++) {
        InBreakG.dequeue(ch);
        if (ch->getBreakEndTime() <= timestep) {
            ch->set_available(true);
            Ready_Vegan_chief.enqueue(ch);
        }
        else {
            InBreakG.enqueue(ch);
        }
    }

    int sizeVIP = InBreakVIP.getCount();
    for (int i = 0; i < sizeVIP; i++) {
        InBreakVIP.dequeue(ch);
        if (ch->getBreakEndTime() <= timestep) {
            ch->set_available(true);
            VIPChiefs.enqueue(ch);
        }
        else {
            InBreakVIP.enqueue(ch);
        }
    }
}

void Restaurant::printFTenEvents() const
{
    LinkedQueue<Event*> temp = EventsList;
    Event* ev = nullptr;

    for (int i = 0; i < 10 && !temp.isEmpty(); i++) {
        temp.dequeue(ev);

        cout << ev->getType() << "," << ev->getEventTime() << ","
            << ev->getOrderID() << endl;
    }
}

/////////////////////////////////\/\/\/\/\/\/\////////////////////////////////////////////
void Restaurant::addEvent(Event* newEvent) {
    EventsList.enqueue(newEvent);
}

void Restaurant::printEvents() const
{
     EventsList.Print();
}

Event* Restaurant::getNextEvent() {
    Event* next = nullptr;
    if (EventsList.peek(next)) {
        EventsList.dequeue(next);
        return next;
    }
    return nullptr;
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

void Restaurant::addToVIPWait(Order* pOrder) {
    if (pOrder->getOrderType() == 'V') {
        VIPWaitList.enqueue(pOrder, pOrder->getPriority());
    }
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
            currentOrder->setOrderType('V');
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

Chief* Restaurant::getChiefInBreak(char chiefType) {
     Chief* chiefInBreak = nullptr;
    if (chiefType == 'N' && !InBreakN.isEmpty()) {
       InBreakN.peek(chiefInBreak);
    }
    else if (chiefType == 'G' && !InBreakG.isEmpty()) {
       InBreakG.peek(chiefInBreak);
    }
    else if (chiefType == 'V' && !InBreakVIP.isEmpty()) {
       InBreakVIP.peek(chiefInBreak);
    }
        return chiefInBreak;
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
   
    cout << "In-Break normal Chiefs\n"; InBreakN.Print();

    cout << "in-Break vegan Chiefs\n"; InBreakG.Print();

    cout << "In-Break VIP Chiefs\n"; InBreakVIP.Print();
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
    inputFile >> bAfter >> bTime>>AutoP;

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

void  Restaurant::PrintNormalOrders()const  {
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

ArrayStack<Order*>& Restaurant::getDeliveredOrders()
{
    return DeliveredOrders;
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

void Restaurant::printAll_delivered_order()  {
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
                << " | GT: " << tempOrder->getAssignT()
                << " | WT: " << tempOrder->getWaitingT()
                << " | ST: " << tempOrder->getServeT() << "\n";
            tempStack.push(tempOrder);
        }
        while (!tempStack.isEmpty()) {
            tempStack.pop(tempOrder);
            DeliveredOrders.push(tempOrder);
        }
    }




