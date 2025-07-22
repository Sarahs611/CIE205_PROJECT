#include "main.h"
#include <iostream>
#include "Event.h"
#include "Restaurant.h"
#include "Order.h"
#include "Chief.h"
#include "priQueue.h"
#include "LinkedQueue.h"
using namespace std;

int main() {

	cout << " Welcome ^_^ ";
	Restaurant r;
	r.LoadInputFile("input.txt");

    int currentTime = 0;

    while (r.countEvents() > 0) {
        currentTime++;
        cout << "TIME: " << currentTime;

        Event* nextEvent = r.getNextEvent();
        while (nextEvent && nextEvent->getEventTime() <= currentTime) {
            cout << "\n Event in progress..\n";
            nextEvent->print();

            nextEvent->Execute(&r); //Execute Arrival, Promotion, or Cancel
            delete nextEvent; 
            nextEvent = r.getNextEvent();
        }
    }
    //display ready chiefs
    r.PrintReadyNormalChefs();
    r.print_all_available_vegan_chiefs();
    r.printAllVIPChiefs();

    // Add events to EventsList
    cout << "Adding events : \n";
    Event* e1 = new Arrival(1, 'N', 101, 3, 50);  
    Event* e2 = new Arrival(2, 'V', 102, 2, 150); 
    Event* e3 = new Arrival(3, 'G', 103, 1, 90);  
    Event* e4 = new Promotion(4, 101, 50);       
    Event* e5 = new Cancel(5, 103);   

    r.addEvent(e1);r.addEvent(e2);r.addEvent(e3);
    r.addEvent(e4);
    r.addEvent(e5);

    cout << "Display events: \n";
    r.printEvents();
    cout << "Total Events: " << r.countEvents() << endl;

    Event* nextEvent = nullptr;
    while ((nextEvent = r.getNextEvent()) != nullptr) {
        nextEvent->print();
        nextEvent->Execute(&r);
        delete nextEvent;
    }

    //vip 
    r.countVIPOrders();
    r.printVIPOrders();

    Order* nextVIP = r.getNextVIPOrder();
    if (nextVIP) {
        cout << "Next VIP Order: ";
        nextVIP->print();
    }

    //inbreakchiefs
    Chief* chef = new Chief(3, 'V', 4, 6, 12);
    r.addChiefToBreak(chef);
    r.countInBreakChiefs();
    r.printInBreakChiefs();

    //test
    Order* newOrder = new Order();
    newOrder->setOrderID(200);
    newOrder->setOrderType('N');
    newOrder->setPrice(50);
    newOrder->setOrderSize(3);
    r.addNormalOrder(newOrder);


    bool promoted = r.promoteOrder(200, 100);
    if (promoted) {
        cout << "Promotion done." << endl;
    }
    else {
        cout << "Promotion failed." << endl;
    }

    bool cancelled = r.cancelEvent(200);
    if (cancelled) {
        cout << "Cancel done." << endl;
    }
    else {
        cout << "Cancel failed." << endl;
    }
}