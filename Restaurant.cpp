#include <iostream>
#include <string>
#include "Event.h" 
#include "Chief.h"
#include "Restaurant.h"
#include "LinkedQueue.h"
#include "UI.h"
#include <cmath>

using namespace std;

Restaurant::Restaurant() {
    AutoP = 0;
    totalWT = 0, totalST = 0;
    deliveredC = 0, promotedC = 0;
}

void Restaurant::SimulationT(int mode)
{
    int timestep = 1;
    char input;

    if (mode == 2)
        cout << "Silent mode on.\n";

    while (!EventsList.isEmpty() || !VIPWaitList.isEmpty() || !WaitNorm.isEmpty()
        || !Waiting_Vegan.isEmpty() || !Waiting_Dessert.isEmpty() || !Waiting_Drink.isEmpty()
        || !In_service_orders.isEmpty())
    {
        ExecuteTimestep(timestep);

        if (mode == 1) {
            UI::printCurrentStatus(this, timestep);

            cout << "\nPress 'Y' if you want to continue: ";
            cin >> input;

            if (input != 'y' && input != 'Y') {
                cout << "Simulation stopped\n";
                break;
            }
        }

        timestep++;
    }
    if (mode == 2)
        cout << "Output file:\n";
}

void Restaurant::ExecuteTimestep(int timestep)
{
    // Execute events
    Event* ev = nullptr;
    while (EventsList.dequeue(ev))
    {
        ev->Execute(this);
    }
    while (EventsList.peek(ev) && ev->getEventTime() == timestep)
    {
        EventsList.dequeue(ev);
        ev->Execute(this);
        delete ev;
    }
    /*Event* ev = nullptr;
    while (EventsList.peek(ev)) {
        if (ev->getEventTime() > timestep)
            break;

        EventsList.dequeue(ev);
        ev->Execute(this);
        delete ev;
    }*/
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
            InjuredChief(c, timestep);
            if (c->getordersbeforebreak() <= 0 && !c->isInjured()) {
                // Chief goes to break
                c->setBreakEndTime(timestep + c->getbreakduration());
                c->set_available(false);

                if (c->getchiefspecialization() == "N")
                    InBreakN.enqueue(c);
                else if (c->getchiefspecialization() == "G")
                    InBreakG.enqueue(c);
                else if (c->getchiefspecialization() == "V")
                    InBreakVIP.enqueue(c);
                else if (c->getchiefspecialization() == "D")
                    InBreakDes.enqueue(c);
                else
                    InBreakDri.enqueue(c);
                //reset
                c->setordersbeforebreak(c->getMaxOrdersBeforeBreak());
            }
            else if (!c->isInjured()) {
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
            currentOrder->setOrderType("V");
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
    int cSpeed;

    //VIP orders
    while (!VIPWaitList.isEmpty())
    {
        if (!(VIPChiefs.dequeue(chief, cSpeed) || readyNormalChiefs.dequeue(chief, cSpeed) || Ready_Vegan_chief.dequeue(chief, cSpeed) ||
            Ready_Dessert_chief.dequeue(chief, cSpeed) || Ready_Drink_chief.dequeue(chief, cSpeed)))
        {
            break;
        }
        VIPWaitList.dequeue(order, cSpeed);
        if (order && chief) assignOrderToChef(order, chief, timestep);
    }

    //vegan orders
    while (!Waiting_Vegan.isEmpty() && Ready_Vegan_chief.dequeue(chief, cSpeed))
    {
        Waiting_Vegan.dequeue(order);
        if (order && chief) assignOrderToChef(order, chief, timestep);
    }

    //normal orders
    while (!WaitNorm.isEmpty())
    {
        if (readyNormalChiefs.dequeue(chief, cSpeed)) {}
        else if (VIPChiefs.dequeue(chief, cSpeed)) {}
        else break;

        WaitNorm.dequeue(order);
        if (order && chief) assignOrderToChef(order, chief, timestep);
    }

    //dessert orders
    while (!Waiting_Dessert.isEmpty() && Ready_Dessert_chief.dequeue(chief, cSpeed))
    {
        Waiting_Dessert.dequeue(order);
        if (order && chief) assignOrderToChef(order, chief, timestep);
    }

    //drink orders
    while (!Waiting_Drink.isEmpty() && Ready_Drink_chief.dequeue(chief, cSpeed))
    {
        Waiting_Drink.dequeue(order);
        if (order && chief) assignOrderToChef(order, chief, timestep);
    }
}
void Restaurant::assignOrderToChef(Order* order, Chief* chef, int timestep)
{
    order->setAssignT(timestep); // GT
    order->setWaititngT(timestep - order->getRequestT()); //WT = GT - RT

    int ST = (int)ceil((double)order->getOrderSize() / chef->getcurrSpeed());
    order->setServeT(ST);
    order->setFinishT(timestep + ST); // FT = GT + ST

    order->set_assign_chief(chef);

    chef->set_available(false);

    // workload
    int restO = chef->getordersbeforebreak() - 1;
    if (restO > 0)
        chef->setordersbeforebreak(restO);
    else
        chef->setordersbeforebreak(0);

    Insert_order(order);
}

void Restaurant::makeChiefready(Chief* ch, int time)
{
    if (!ch) return;

    ch->set_available(true);

    int Speed;
    if (ch->isInjured())
        Speed = ch->getcurrSpeed();
    else
        Speed = ch->getspeed();

    if (ch->getchiefspecialization() == "N")
        readyNormalChiefs.enqueue(ch, Speed);
    else if (ch->getchiefspecialization() == "G")
        Ready_Vegan_chief.enqueue(ch, Speed);
    //Bonus
    else if (ch->getchiefspecialization() == "D")
        Ready_Dessert_chief.enqueue(ch, ch->getspeed());

    else if (ch->getchiefspecialization() == "K")
        Ready_Drink_chief.enqueue(ch, ch->getspeed());
    else
        VIPChiefs.enqueue(ch, Speed);
}
void Restaurant::updateChiefStatus(int timestep)
{
    Chief* ch;

    int sizeN = InBreakN.getCount();
    for (int i = 0; i < sizeN; i++) {
        InBreakN.dequeue(ch);

        if (ch->isInjured()) {
            if (timestep >= ch->getinjET()) {
                ch->recover(timestep);
                makeChiefready(ch, timestep);
            }
            else if (readyNormalChiefs.isEmpty() && VIPChiefs.isEmpty()) {
                ch->forceBack(timestep);
                makeChiefready(ch, timestep);
            }
            else InBreakN.enqueue(ch);
        }
        else if (ch->getBreakEndTime() <= timestep) {
            makeChiefready(ch, timestep);
        }
        else InBreakN.enqueue(ch);
    }

    int sizeG = InBreakG.getCount();
    for (int i = 0; i < sizeG; i++) {
        InBreakG.dequeue(ch);

        if (ch->isInjured()) {
            if (timestep >= ch->getinjET()) {
                ch->recover(timestep);
                makeChiefready(ch, timestep);
            }
            else if (Ready_Vegan_chief.isEmpty()) {
                ch->forceBack(timestep);
                makeChiefready(ch, timestep);
            }
            else InBreakG.enqueue(ch);
        }
        else if (ch->getBreakEndTime() <= timestep) {
            makeChiefready(ch, timestep);
        }
        else InBreakG.enqueue(ch);
    }

    int sizeVIP = InBreakVIP.getCount();
    for (int i = 0; i < sizeVIP; i++) {
        InBreakVIP.dequeue(ch);

        if (ch->isInjured()) {
            if (timestep >= ch->getinjET()) {
                ch->recover(timestep);
                makeChiefready(ch, timestep);
            }
            else if (VIPChiefs.isEmpty() && readyNormalChiefs.isEmpty() && Ready_Vegan_chief.isEmpty()) {
                ch->forceBack(timestep);
                makeChiefready(ch, timestep);
            }
            else InBreakVIP.enqueue(ch);
        }
        else if (ch->getBreakEndTime() <= timestep) {
            makeChiefready(ch, timestep);
        }
        else InBreakVIP.enqueue(ch);
    }

    int sizedes = InBreakDes.getCount();
    for (int i = 0; i < sizedes; i++) {
        InBreakDes.dequeue(ch);

        if (ch->isInjured()) {
            if (timestep >= ch->getinjET()) {
                ch->recover(timestep);
                makeChiefready(ch, timestep);
            }
            else if (Ready_Dessert_chief.isEmpty()) {
                ch->forceBack(timestep);
                makeChiefready(ch, timestep);
            }
            else InBreakDes.enqueue(ch);
        }
        else if (ch->getBreakEndTime() <= timestep) {
            makeChiefready(ch, timestep);
        }
        else InBreakDes.enqueue(ch);
    }

    int sizedri = InBreakDri.getCount();
    for (int i = 0; i < sizedri; i++) {
        InBreakDri.dequeue(ch);

        if (ch->isInjured()) {
            if (timestep >= ch->getinjET()) {
                ch->recover(timestep);
                makeChiefready(ch, timestep);
            }
            else if (Ready_Drink_chief.isEmpty()) {
                ch->forceBack(timestep);
                makeChiefready(ch, timestep);
            }
            else InBreakDri.enqueue(ch);
        }
        else if (ch->getBreakEndTime() <= timestep) {
            makeChiefready(ch, timestep);
        }
        else InBreakDri.enqueue(ch);
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

Order* Restaurant::getNextVIPOrder() {
    Order* next = nullptr;
    int priority = 0;
    if (VIPWaitList.dequeue(next, priority)) {
        return next;
    }
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////

void Restaurant::addToVIPWait(Order* pOrder) {
    if (pOrder->getOrderType() == "V") {
        VIPWaitList.enqueue(pOrder, pOrder->getPriority());
    }
}

int Restaurant::countVIPOrders() const {
    return VIPWaitList.getCount();
}

void Restaurant::printVIPOrders()
{
    Order* vo;
    int pr;
    priQueue<Order*> temp;

    while (!VIPWaitList.isEmpty())
    {
        VIPWaitList.dequeue(vo, pr);
        vo->print();
        temp.enqueue(vo, pr);

    }
    while (!temp.isEmpty())
    {
        temp.dequeue(vo, pr);
        VIPWaitList.enqueue(vo, pr);
    }
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
            currentOrder->setOrderType("V");
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

Chief* Restaurant::getChiefInBreak(string chiefType) {
    Chief* chiefInBreak = nullptr;
    if (chiefType == "N" && !InBreakN.isEmpty()) {
        InBreakN.dequeue(chiefInBreak);
    }
    else if (chiefType == "G" && !InBreakG.isEmpty()) {
        InBreakG.dequeue(chiefInBreak);
    }
    else if (chiefType == "V" && !InBreakVIP.isEmpty()) {
        InBreakVIP.dequeue(chiefInBreak);
    }
    //Bonus
    else if (chiefType == "D" && !InBreakDes.isEmpty())
    {
        InBreakDes.dequeue(chiefInBreak);
    }
    else if (chiefType == "K" && !InBreakDri.isEmpty())
    {
        InBreakDri.dequeue(chiefInBreak);
    }
    return chiefInBreak;
}

void Restaurant::addChiefToBreak(Chief* Chief) {
    string type = Chief->getchiefspecialization();
    if (type == "N") {
        InBreakN.enqueue(Chief);
    }
    else if (type == "G") {
        InBreakG.enqueue(Chief);
    }
    else if (type == "V") {
        InBreakVIP.enqueue(Chief);
    }
    //Bonus
    else if (type == "D")
    {
        InBreakDes.enqueue(Chief);
    }
    else if (type == "K")
    {
        InBreakDri.enqueue(Chief);
    }
}

void Restaurant::LoadInputFile(string fileName) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "Could not open the file." << endl;
        return;
    }

    int nNormal, nVegan, nVIP, nDes, nDri;
    inputFile >> nNormal >> nVegan >> nVIP >> nDes >> nDri;


    int sNormal, sVegan, sVIP, sDes, sDri;
    inputFile >> sNormal >> sVegan >> sVIP >> sDes >> sDri;

    // Break 
    int BO, BN, BG, BV, BDes, BDri;
    inputFile >> BO >> BN >> BG >> BV >> BDes >> BDri;


    inputFile >> AutoP;

    for (int i = 0; i < nNormal; i++) {
        Chief* ch = new Chief(i + 1, "N", sNormal, BO, BN);
        readyNormalChiefs.enqueue(ch, ch->getspeed());
    }
    for (int i = 0; i < nVegan; i++) {
        Chief* ch = new Chief(i + 1, "G", sVegan, BO, BG);
        Ready_Vegan_chief.enqueue(ch, ch->getspeed());
    }
    for (int i = 0; i < nVIP; i++) {
        Chief* ch = new Chief(i + 1, "V", sVIP, BO, BV);
        VIPChiefs.enqueue(ch, ch->getspeed());
    }
    for (int i = 0; i < nDes; i++) {
        Chief* ch = new Chief(i + 1, "D", sDes, BO, BDes);
        Ready_Dessert_chief.enqueue(ch, ch->getspeed());
    }
    for (int i = 0; i < nDri; i++) {
        Chief* ch = new Chief(i + 1, "K", sDri, BO, BDri);
        Ready_Drink_chief.enqueue(ch, ch->getspeed());
    }
    int numEvents;
    inputFile >> numEvents;

    for (int i = 0; i < numEvents; i++) {
        string eType;
        inputFile >> eType;

        Event* ev = nullptr;

        if (eType == "R") {
            string oType;
            int ts, id, size;
            double money;
            inputFile >> oType >> ts >> id >> size >> money;
            ev = new Arrival(oType, size, money, ts, id);
        }
        else if (eType == "X") {
            int ts, id;
            inputFile >> ts >> id;
            ev = new Cancel(ts, id);
        }
        else if (eType == "P") {
            int ts, id;
            double extra;
            inputFile >> ts >> id >> extra;
            ev = new Promotion(extra, ts, id);
        }

        if (ev) {
            EventsList.enqueue(ev);
        }
    }
    cout << "DEBUG: Loaded " << EventsList.getCount() << " events\n";
    inputFile.close();
}
int Restaurant::countInBreakChiefs() const
{
    return InBreakN.getCount() + InBreakG.getCount() + InBreakVIP.getCount() + InBreakDes.getCount() + InBreakDri.getCount();
}
void Restaurant::printInBreakChiefs()
{
    Chief* c;
    LinkedQueue<Chief*> tempN;
    cout << "Normal Chiefs in Break:\n";
    while (!InBreakN.isEmpty())
    {
        InBreakN.dequeue(c);
        tempN.enqueue(c);
        c->print();
    }
    while (!tempN.isEmpty())
    {
        tempN.dequeue(c);
        InBreakN.enqueue(c);
    }

    LinkedQueue<Chief*> tempG;
    cout << "Vegan Chiefs in Break:\n";
    while (!InBreakG.isEmpty())
    {
        InBreakG.dequeue(c);
        tempG.enqueue(c);
        c->print();
    }
    while (!tempG.isEmpty())
    {
        tempG.dequeue(c);
        InBreakG.enqueue(c);
    }

    LinkedQueue<Chief*> tempV;
    cout << "VIP Chiefs in Break:\n";
    while (!InBreakVIP.isEmpty())
    {
        InBreakVIP.dequeue(c);
        tempV.enqueue(c);
        c->print();

    }
    while (!tempV.isEmpty())
    {
        tempV.dequeue(c);
        InBreakVIP.enqueue(c);
    }

    LinkedQueue<Chief*> tempD;
    cout << "Dessert Chiefs in Break:\n";
    while (!InBreakDes.isEmpty())
    {
        InBreakDes.dequeue(c);
        tempD.enqueue(c);
        c->print();
    }
    while (!tempD.isEmpty())
    {
        tempD.dequeue(c);
        InBreakDes.enqueue(c);
    }

    LinkedQueue<Chief*> tempDr;
    cout << "Drink Chiefs in Break:\n";
    while (!InBreakDri.isEmpty())
    {
        InBreakDri.dequeue(c);
        tempDr.enqueue(c);
        c->print();

    }
    while (!tempDr.isEmpty())
    {
        tempDr.dequeue(c);
        InBreakDri.enqueue(c);
    }
}
////////Bonus////////
void Restaurant::InjuredChief(Chief* ch, int timestep)
{
    if (!ch) return;
    int injure = rand() % 100;
    if (injure < 5) {
        ch->needRest(5, timestep);
        addChiefToBreak(ch);
    }
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


void Restaurant::PrintReadyNormalChefs() {
    LinkedQueue<Chief*> tempQueue;
    Chief* chefPtr = nullptr;
    int cSpeed;
    cout << "Ready Normal Chiefs:\n";

    while (!readyNormalChiefs.isEmpty()) {
        readyNormalChiefs.dequeue(chefPtr, cSpeed);
        chefPtr->print();  // use Chef's print function
        tempQueue.enqueue(chefPtr);  // keep them safe
    }

    // Restore the original queue

    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(chefPtr);
        readyNormalChiefs.enqueue(chefPtr, chefPtr->getspeed());
    }
}

void  Restaurant::PrintNormalOrders()const {
    LinkedQueue<Order*> temp = WaitNorm;
    Order* current = nullptr;

    while (!temp.isEmpty()) {
        temp.dequeue(current);
        current->print();

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

bool Restaurant::Searchfor_vegan_order(Order* vo)
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

void Restaurant::PrintVeganOrders()
{
    Order* vo;
    LinkedQueue<Order*>temp;
    while (!Waiting_Vegan.isEmpty())
    {
        Waiting_Vegan.dequeue(vo);
        vo->print();
        temp.enqueue(vo);

    }
    while (!temp.isEmpty())
    {
        temp.dequeue(vo);
        Waiting_Vegan.enqueue(vo);
    }
}

// vegan_chiefs
bool Restaurant::Insert_vegan_chief(Chief* ch)
{
    Ready_Vegan_chief.enqueue(ch, ch->getspeed());
    return true;
}

bool Restaurant::Remove_vegan_chief(Chief* ch)
{
    LinkedQueue<Chief*>temp;
    Chief* vegan_chief;
    bool Removed = false;
    int cSpeed;

    while (Ready_Vegan_chief.dequeue(vegan_chief, cSpeed))
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
        Ready_Vegan_chief.enqueue(vegan_chief, vegan_chief->getspeed());
    }
    return Removed;
}

bool Restaurant::Searchfor_available_vegan_chief(Chief* ch)
{
    LinkedQueue<Chief*> temp;
    Chief* chief;
    bool found = false;
    int cSpeed;

    if (Ready_Vegan_chief.isEmpty())
    {
        return false;
    }
    while (Ready_Vegan_chief.dequeue(chief, cSpeed))
    {
        temp.enqueue(chief);
        if (chief == ch)
        {
            found = true;
        }
    }
    while (temp.dequeue(chief))
    {
        Ready_Vegan_chief.enqueue(chief, chief->getspeed());
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
    int cSpeed;

    if (Ready_Vegan_chief.isEmpty())
    {
        return;
    }
    while (Ready_Vegan_chief.dequeue(ch, cSpeed))
    {
        temp.enqueue(ch);
        ch->print();
    }
    while (temp.dequeue(ch))
    {
        Ready_Vegan_chief.enqueue(ch, ch->getspeed());
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

void Restaurant::printInServeOrders()
{
    Order* O;
    int FT;
    while (!In_service_orders.isEmpty())
    {
        In_service_orders.dequeue(O, FT);
        O->print();
        cout << "Priority : " << O->getPriority() << endl;
    }
}

void Restaurant::OutputFile(ofstream& out)
{
    if (DeliveredOrders.isEmpty())
    {
        out << "No delivered orders.\n";
        return;
    }
    ArrayStack<Order*> tempStack;
    Order* tempOrder = nullptr;
    out << "Delivered Orders (Most recent first):\n";
    int count_vegano = 0;
    int count_normalo = 0;
    int count_VIPo = 0;
    int count_desserto = 0;
    int count_drinko = 0;
    int count_veganc = Ready_Vegan_chief.getCount();
    int count_normalc = readyNormalChiefs.getCount();
    int count_VIPc = VIPChiefs.getCount();
    int count_drinkc = Ready_Drink_chief.getCount();
    int count_dessertc = Ready_Dessert_chief.getCount();
    int totalchiefs = count_veganc + count_normalc + count_VIPc + count_drinkc + count_dessertc;
    int count_total_delivered = 0;
    double totalWaiting = 0;
    double totalserve = 0;
    float avgwaiting = 0;
    float avgserve = 0;
    while (!DeliveredOrders.isEmpty())
    {
        DeliveredOrders.pop(tempOrder);
        out << "FT: " << tempOrder->getFinishT()
            << " | ID: " << tempOrder->getOrderID()
            << " | RT: " << tempOrder->getRequestT()
            << " | GT: " << tempOrder->getAssignT()
            << " | WT: " << tempOrder->getWaitingT()
            << " | ST: " << tempOrder->getServeT() << "\n";
        out << "---------------------------------------------------------";
        out << endl;
        count_total_delivered++;
        if (tempOrder->getOrderType() == "G")
        {
            count_vegano++;
        }
        else if (tempOrder->getOrderType() == "V")
        {
            count_VIPo++;
        }
        else if (tempOrder->getOrderType() == "N")
        {
            count_normalo++;
        }
        else if (tempOrder->getOrderType() == "D")
        {
            count_desserto++;
        }
        else if (tempOrder->getOrderType() == "K")
        {
            count_drinko++;
        }
        else
            return;
        totalWaiting += tempOrder->getWaitingT();
        totalserve += tempOrder->getServeT();

        tempStack.push(tempOrder);
    }
    while (!tempStack.isEmpty())
    {
        tempStack.pop(tempOrder);
        DeliveredOrders.push(tempOrder);
    }

    if (count_total_delivered > 0)
    {
        avgwaiting = totalWaiting / count_total_delivered;
        avgserve = totalserve / count_total_delivered;
    }
    out << "Orders : " << count_total_delivered << "[" << "Norm :" << count_normalo << "," << "Veg :" << count_vegano << "," << "VIP :"
        << count_VIPo << "," << "Dessert :" << count_desserto << "," << "Drink :" << count_drinko << "]" << endl;
    out << "Chiefs : " << totalchiefs << "[" << "Norm :" << count_normalc << "," << "Veg :" << count_veganc << "," << "VIP :" << count_VIPc << ","
        << "Dessert : " << count_dessertc << "," << "Drink :" << count_drinkc << "]" << endl;
    out << "Avgwait = " << avgwaiting << "," << "Avgserve = " << avgserve << endl;
    out << "Auto_promoted : " << AutoP << " %" << endl;
}
////////////////////bouns/////////////////////////////////////////////////

//Dessert orders
bool Restaurant::Insert_dessert_order(Order* des)
{
    Waiting_Dessert.enqueue(des);
    return true;
}
bool Restaurant::Remove_dessert_order(Order* des)
{
    LinkedQueue<Order*>temp;
    Order* dessert;
    bool Removed = false;
    while (Waiting_Dessert.dequeue(dessert))
    {
        if (dessert == des)
        {
            Removed = true;
            continue;
        }
        temp.enqueue(dessert);
    }
    while (temp.dequeue(dessert))
    {
        Waiting_Dessert.enqueue(dessert);
    }
    return Removed;
}
bool Restaurant::Searchfor_dessert_order(Order* des)
{
    LinkedQueue<Order*> temp;
    Order* O;
    bool found = false;
    if (Waiting_Dessert.isEmpty())
    {
        return false;
    }
    while (Waiting_Dessert.dequeue(O))
    {
        temp.enqueue(O);
        if (O == des)
        {
            found = true;
        }
    }
    while (temp.dequeue(O))
    {
        Waiting_Dessert.enqueue(O);
    }
    return found;
}
Order Restaurant::get_next_dessert_order(Node<Order>* curr)
{
    Order next;
    if (!curr->getNext())
    {
        return Order();
    }
    next = curr->getNext()->getItem();
    return next;
}
void Restaurant::Print_dessert_Orders()
{
    Order* des;
    LinkedQueue<Order*>temp;
    while (!Waiting_Dessert.isEmpty())
    {
        Waiting_Dessert.dequeue(des);
        temp.enqueue(des);
        des->print();
    }
    while (!temp.isEmpty())
    {
        temp.dequeue(des);
        Waiting_Dessert.enqueue(des);
    }
}
//////////////////////////////////////////
//Drink orders
bool Restaurant::Insert_Drink_order(Order* Dri)
{
    Waiting_Drink.enqueue(Dri);
    return true;
}
bool Restaurant::Remove_Drink_order(Order* Dri)
{
    LinkedQueue<Order*>temp;
    Order* drink;
    bool Removed = false;
    while (Waiting_Drink.dequeue(drink))
    {
        if (drink == Dri)
        {
            Removed = true;
            continue;
        }
        temp.enqueue(drink);
    }
    while (temp.dequeue(drink))
    {
        Waiting_Drink.enqueue(drink);
    }
    return Removed;
}
bool Restaurant::Searchfor_Drink_order(Order* Dri)
{
    LinkedQueue<Order*> temp;
    Order* O;
    bool found = false;
    if (Waiting_Drink.isEmpty())
    {
        return false;
    }
    while (Waiting_Drink.dequeue(O))
    {
        temp.enqueue(O);
        if (O == Dri)
        {
            found = true;
        }
    }
    while (temp.dequeue(O))
    {
        Waiting_Drink.enqueue(O);
    }
    return found;
}
Order Restaurant::get_next_Drink_order(Node<Order>* curr)
{
    Order next;
    if (!curr->getNext())
    {
        return Order();
    }
    next = curr->getNext()->getItem();
    return next;
}
void Restaurant::Print_Drink_Orders()
{
    Order* drink;
    LinkedQueue<Order*>temp;
    while (!Waiting_Drink.isEmpty())
    {
        Waiting_Drink.dequeue(drink);
        temp.enqueue(drink);
        drink->print();
    }
    while (!temp.isEmpty())
    {
        temp.dequeue(drink);
        Waiting_Drink.enqueue(drink);
    }
}

//Dessert chiefs
bool Restaurant::Insert_Dessert_chief(Chief* ch)
{
    Ready_Dessert_chief.enqueue(ch, ch->getspeed());
    return true;
}
bool Restaurant::Remove_Dessert_chief(Chief* ch)
{
    priQueue<Chief*> temp;int cSpeed;
    Chief* dessert_chief;
    bool Removed = false;
    while (Ready_Dessert_chief.dequeue(dessert_chief, cSpeed))
    {
        if (dessert_chief == ch)
        {
            Removed = true;
            continue;
        }
        temp.enqueue(dessert_chief, dessert_chief->getspeed());
    }
    while (temp.dequeue(dessert_chief, cSpeed))
    {
        Ready_Dessert_chief.enqueue(dessert_chief, dessert_chief->getspeed());
    }
    return Removed;
}
bool Restaurant::Searchfor_available_Dessert_chief(Chief* ch)
{
    priQueue<Chief*> temp;
    Chief* chief;
    bool found = false; int cSpeed;
    if (Ready_Dessert_chief.isEmpty())
    {
        return false;
    }
    while (Ready_Dessert_chief.dequeue(chief, cSpeed))
    {
        temp.enqueue(chief, chief->getspeed());
        if (chief == ch)
        {
            found = true;
        }
    }
    while (temp.dequeue(chief, cSpeed))
    {
        Ready_Dessert_chief.enqueue(chief, chief->getspeed());
    }
    return found;
}
Chief Restaurant::get_next_available_Dessert_chief(Node<Chief>* curr_chief)
{
    Chief Available_next_chief;
    if (!curr_chief->getNext())
    {
        return Chief();
    }
    Available_next_chief = curr_chief->getNext()->getItem();
    return Available_next_chief;
}
void Restaurant::print_all_available_Dessert_chiefs()
{
    priQueue<Chief*> temp;
    Chief* ch; int cSpeed;
    if (Ready_Dessert_chief.isEmpty())
    {
        return;
    }
    while (Ready_Dessert_chief.dequeue(ch, cSpeed))
    {
        ch->print();
        temp.enqueue(ch, ch->getspeed());

    }
    while (temp.dequeue(ch, cSpeed))
    {
        Ready_Dessert_chief.enqueue(ch, ch->getspeed());
    }
}
////////////////////////////////////////////
//Drink chiefs
bool Restaurant::Insert_Drink_chief(Chief* ch)
{
    Ready_Drink_chief.enqueue(ch, ch->getspeed());
    return true;
}
bool Restaurant::Remove_Drink_chief(Chief* ch)
{
    priQueue<Chief*> temp;
    int cSpeed;
    Chief* drink_chief;
    bool Removed = false;
    while (Ready_Drink_chief.dequeue(drink_chief, cSpeed))
    {
        if (drink_chief == ch)
        {
            Removed = true;
            continue;
        }
        temp.enqueue(drink_chief, drink_chief->getspeed());
    }
    while (temp.dequeue(drink_chief, cSpeed))
    {
        Ready_Drink_chief.enqueue(drink_chief, drink_chief->getspeed());
    }
    return Removed;
}
bool Restaurant::Searchfor_available_Drink_chief(Chief* ch)
{
    priQueue<Chief*> temp;
    Chief* chief;
    int cSpeed;
    bool found = false;
    if (Ready_Drink_chief.isEmpty())
    {
        return false;
    }
    while (Ready_Drink_chief.dequeue(chief, cSpeed))
    {
        temp.enqueue(chief, chief->getspeed());
        if (chief == ch)
        {
            found = true;
        }
    }
    while (temp.dequeue(chief, cSpeed))
    {
        Ready_Drink_chief.enqueue(chief, chief->getspeed());
    }
    return found;
}
Chief Restaurant::get_next_available_Drink_chief(Node<Chief>* curr_chief)
{
    Chief Available_next_chief;
    if (!curr_chief->getNext())
    {
        return Chief();
    }
    Available_next_chief = curr_chief->getNext()->getItem();
    return Available_next_chief;
}
void Restaurant::print_all_available_Drink_chiefs()
{
    priQueue<Chief*> temp;
    Chief* ch;int cSpeed;
    if (Ready_Drink_chief.isEmpty())
    {
        return;
    }
    while (Ready_Drink_chief.dequeue(ch, cSpeed))
    {
        ch->print();
        temp.enqueue(ch, ch->getspeed());

    }
    while (temp.dequeue(ch, cSpeed))
    {
        Ready_Drink_chief.enqueue(ch, ch->getspeed());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//vip cheifs 
bool Restaurant::addChef(Chief* chef) {
    VIPChiefs.enqueue(chef, chef->getspeed());
    return true;
}
bool Restaurant::isEmpty() const {
    return VIPChiefs.isEmpty();
}

bool Restaurant::getNextChef(Chief* chef) {
    int cSpeed;
    return VIPChiefs.dequeue(chef, cSpeed);
}

bool Restaurant::peekNextChef(Chief*& chef) {
    int cSpeed;
    return VIPChiefs.peek(chef, cSpeed);
}

void Restaurant::printAllVIPChiefs() const {
    if (VIPChiefs.isEmpty()) {
        cout << "No VIP chefs available.\n";
        return;
    }
    priQueue<Chief*> tempQueue = VIPChiefs;
    Chief* temp = nullptr;
    int cSpeed;
    cout << "Available VIP Chiefs:\n";
    while (tempQueue.dequeue(temp, cSpeed)) {
        temp->print();
    }
}

//delivered Orders
bool Restaurant::push(Order* order)
{
    DeliveredOrders.push(order);
    return true;
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

void Restaurant::printAll_delivered_order() {
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
///////////////////////////////////////////////////

