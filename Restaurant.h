#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Order.h"
#include "Chief.h" 

class Restaurant
{
private:
	//Sarah
	LinkedQueue<Event*> EventsList;

	priQueue<Order*> VIPWaitList;

	LinkedQueue<Chief*> InBreakN;
	LinkedQueue<Chief*> InBreakG;
	LinkedQueue<Chief*> InBreakVIP;


	//Nouran
    LinkedQueue<Order*> N_waitlist;
    
public:
	Event* getNextEvent();
	void addEvent(Event* newEvent);
	void printEvents()const;
	int countEvents() const;

	Order* getNextVIPOrder() ;
	void addToVIPWait(Order* pOrder); 
	int  countVIPOrders()const;            
	void printVIPOrders()const;

	            //////////
	/*void addToNormalWait(Order* pOrder);
	void addToVeganWait(Order* pOrder);*/
	bool cancelOrder(int orderID);
	bool promoteOrder(int orderID, double extraMoney);


	Chief* getReadyChief(char chiefType);
	void addChiefToBreak(Chief* pChief);
	int  countInBreakChiefs()const ;
	void printInBreakChiefs()const;


	// nouran

	bool CancelOrder(int id);

	Order* GetOrder(int id);







};

