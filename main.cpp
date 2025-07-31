#include <iostream>
#include "Event.h"
#include "Restaurant.h"
#include "Order.h"
#include "Chief.h"
#include "priQueue.h"
#include "LinkedQueue.h"
#include "UI.h"
using namespace std;

int main()
{

    cout << " Welcome ^_^ "<<endl;
    Restaurant r;
    int mode = UI::selectMode(); 
    r.SimulationT(mode);
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
    Event* e1 = new Arrival('N',1, 101, 3, 50);
    Event* e2 = new Arrival('V',2, 102, 2, 150);
    Event* e3 = new Arrival('G',3, 103, 1, 90);
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






    //insert vegan orders
    Order* vo1 = new Order();
    vo1->setOrderID(70);
    vo1->setOrderType('v');
    vo1->setPrice(65);
    vo1->setOrderSize(1);
    vo1->setFinishT(12);
    r.Insert_vegan_order(vo1);

    Order* vo2 = new Order();
    vo2->setOrderID(65);
    vo2->setOrderType('v');
    vo2->setPrice(100);
    vo2->setOrderSize(2);
    vo2->setFinishT(15);
    r.Insert_vegan_order(vo2);

    Order* vo3 = new Order();
    vo3->setOrderID(60);
    vo3->setOrderType('v');
    vo3->setPrice(200);
    vo3->setOrderSize(3);
    vo3->setFinishT(20);
    r.Insert_vegan_order(vo3);

    Order* vo4 = new Order();
    vo4->setOrderID(40);
    vo4->setOrderType('v');
    vo4->setPrice(250);
    vo4->setOrderSize(4);
    vo4->setFinishT(25);
    r.Insert_vegan_order(vo4);

    Order* vo5 = new Order();
    vo5->setOrderID(80);
    vo5->setOrderType('v');
    vo5->setPrice(300);
    vo5->setOrderSize(5);
    vo5->setFinishT(30);
    r.Insert_vegan_order(vo5);

    Order* vo6 = new Order();
    vo6->setOrderID(30);
    vo6->setOrderType('v');
    vo6->setPrice(350);
    vo6->setOrderSize(3);
    vo6->setFinishT(23);
    r.Insert_vegan_order(vo6);

    Order* vo7 = new Order();
    vo7->setOrderID(15);
    vo7->setOrderType('v');
    vo7->setPrice(450);
    vo7->setOrderSize(5);
    vo7->setFinishT(30);
    r.Insert_vegan_order(vo7);

    Order* vo8 = new Order();
    vo8->setOrderID(10);
    vo8->setOrderType('v');
    vo8->setPrice(120);
    vo8->setOrderSize(1);
    vo8->setFinishT(10);
    r.Insert_vegan_order(vo8);


    Order* vo9 = new Order();
    vo9->setOrderID(90);
    vo9->setOrderType('v');
    vo9->setPrice(500);
    vo9->setOrderSize(6);
    vo9->setFinishT(40);
    r.Insert_vegan_order(vo9);

    Order* vo10 = new Order();
    vo10->setOrderID(100);
    vo10->setOrderType('v');
    vo10->setPrice(600);
    vo10->setOrderSize(7);
    vo10->setFinishT(45);
    r.Insert_vegan_order(vo10);

    //remove vegan order 
    r.Remove_vegan_order(vo1);

    //add vegan chiefs
    Chief* vchief1 = new Chief(4, 'v', 3, 2, 5);
    Chief* vchief2 = new Chief(7, 'v', 5, 1, 3);
    Chief* vchief3 = new Chief(6, 'v', 2, 3, 6);
    r.Insert_vegan_chief(vchief1);
    r.Insert_vegan_chief(vchief2);
    r.Insert_vegan_chief(vchief3);
    r.print_all_available_vegan_chiefs();

    // insert normal orders
    Order* no1 = new Order();
    no1->setOrderID(201);
    no1->setOrderType('N');
    no1->setPrice(80);
    no1->setOrderSize(2);
    no1->setFinishT(15);
    r.addNormalOrder(no1);

    Order* no2 = new Order();
    no2->setOrderID(202);
    no2->setOrderType('N');
    no2->setPrice(120);
    no2->setOrderSize(3);
    no2->setFinishT(20);
    r.addNormalOrder(no2);

    Order* no3 = new Order();
    no3->setOrderID(203);
    no3->setOrderType('N');
    no3->setPrice(200);
    no3->setOrderSize(4);
    no3->setFinishT(25);
    r.addNormalOrder(no3);

    Order* no4 = new Order();
    no4->setOrderID(204);
    no4->setOrderType('N');
    no4->setPrice(150);
    no4->setOrderSize(2);
    no4->setFinishT(18);
    r.addNormalOrder(no4);

    Order* no5 = new Order();
    no5->setOrderID(205);
    no5->setOrderType('N');
    no5->setPrice(300);
    no5->setOrderSize(5);
    no5->setFinishT(30);
    r.addNormalOrder(no5);

    Order* no6 = new Order();
    no6->setOrderID(206);
    no6->setOrderType('N');
    no6->setPrice(400);
    no6->setOrderSize(6);
    no6->setFinishT(35);
    r.addNormalOrder(no6);

    Order* no7 = new Order();
    no7->setOrderID(207);
    no7->setOrderType('N');
    no7->setPrice(250);
    no7->setOrderSize(3);
    no7->setFinishT(28);
    r.addNormalOrder(no7);

    Order* no8 = new Order();
    no8->setOrderID(208);
    no8->setOrderType('N');
    no8->setPrice(180);
    no8->setOrderSize(2);
    no8->setFinishT(22);
    r.addNormalOrder(no8);

    Order* no9 = new Order();
    no9->setOrderID(209);
    no9->setOrderType('N');
    no9->setPrice(500);
    no9->setOrderSize(7);
    no9->setFinishT(40);
    r.addNormalOrder(no9);

    Order* no10 = new Order();
    no10->setOrderID(210);
    no10->setOrderType('N');
    no10->setPrice(600);
    no10->setOrderSize(8);
    no10->setFinishT(50);
    r.addNormalOrder(no10);


    // insert VIP orders
    Order* vip1 = new Order();
    vip1->setOrderID(301);
    vip1->setOrderType('V');
    vip1->setPrice(200);
    vip1->setOrderSize(3);
    vip1->setFinishT(20);
    r.addToVIPWait(vip1);

    Order* vip2 = new Order();
    vip2->setOrderID(302);
    vip2->setOrderType('V');
    vip2->setPrice(300);
    vip2->setOrderSize(4);
    vip2->setFinishT(25);
    r.addToVIPWait(vip2);

    Order* vip3 = new Order();
    vip3->setOrderID(303);
    vip3->setOrderType('V');
    vip3->setPrice(500);
    vip3->setOrderSize(5);
    vip3->setFinishT(30);
    r.addToVIPWait(vip3);

    Order* vip4 = new Order();
    vip4->setOrderID(304);
    vip4->setOrderType('V');
    vip4->setPrice(350);
    vip4->setOrderSize(3);
    vip4->setFinishT(22);
    r.addToVIPWait(vip4);

    Order* vip5 = new Order();
    vip5->setOrderID(305);
    vip5->setOrderType('V');
    vip5->setPrice(450);
    vip5->setOrderSize(6);
    vip5->setFinishT(35);
    r.addToVIPWait(vip5);

    Order* vip6 = new Order();
    vip6->setOrderID(306);
    vip6->setOrderType('V');
    vip6->setPrice(700);
    vip6->setOrderSize(7);
    vip6->setFinishT(40);
    r.addToVIPWait(vip6);

    Order* vip7 = new Order();
    vip7->setOrderID(307);
    vip7->setOrderType('V');
    vip7->setPrice(800);
    vip7->setOrderSize(8);
    vip7->setFinishT(45);
    r.addToVIPWait(vip7);

    Order* vip8 = new Order();
    vip8->setOrderID(308);
    vip8->setOrderType('V');
    vip8->setPrice(600);
    vip8->setOrderSize(5);
    vip8->setFinishT(32);
    r.addToVIPWait(vip8);

    Order* vip9 = new Order();
    vip9->setOrderID(309);
    vip9->setOrderType('V');
    vip9->setPrice(900);
    vip9->setOrderSize(9);
    vip9->setFinishT(50);
    r.addToVIPWait(vip9);

    Order* vip10 = new Order();
    vip10->setOrderID(310);
    vip10->setOrderType('V');
    vip10->setPrice(1000);
    vip10->setOrderSize(10);
    vip10->setFinishT(55);
    r.addToVIPWait(vip10);

    //vip 
    cout<< "\nTotal before VIP Orders:"<< r.countVIPOrders()<<endl;
    r.printVIPOrders();

    Order* nextVIP = r.getNextVIPOrder();
    if (nextVIP) {
        cout << "\nNext VIP Order: ";
        nextVIP->print();
    }

    //inbreakchiefs
    Chief* chef = new Chief(3, 'V', 4, 6, 12);
    r.addChiefToBreak(chef);
    cout << "\n Count of VIP Chiefs :"<< r.countInBreakChiefs()<<endl;
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
        cout << "\nPromotion done." << endl;
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
    cout << "\nTotal after VIP Orders:" << r.countVIPOrders() << endl;
}