#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Order.h"
#include "Chief.h" 
#include "ArrayStack.h"

#include<string>
class Restaurant
{
private:
	//Sarah

	int AutoP;
	double totalWT, totalST;
	int deliveredC = 0, promotedC;

	LinkedQueue<Event*> EventsList;

	priQueue<Order*> VIPWaitList;

	LinkedQueue<Chief*> InBreakN;
	LinkedQueue<Chief*> InBreakG;
	LinkedQueue<Chief*> InBreakVIP;

	//Nouran
    LinkedQueue<Order*> WaitNorm;
	LinkedQueue<Chief*> readyNormalChefs;


    //Mariam
	LinkedQueue<Order*> Waiting_Vegan;
	LinkedQueue<Chief*> Ready_Vegan_chief;
	priQueue<Order*>In_service_orders;

	//Basmala
	LinkedQueue<Chief*> VIPChiefs;
	ArrayStack<Order*> DeliveredOrders;

public:
	Restaurant();
	void SimulationT(int mode);
	void ExecuteTimestep(int timestep);
	void assignOrders(int timestep); 
	void assignOrderToChef(Order* order, Chief* chef, int timestep);
	void checkAutoPromote(int timestep);
	void updateChiefStatus(int timestep);
	void  makeChiefready(Chief* ch, int times);
    void printFTenEvents() const;

	Event* getNextEvent();
	void addEvent(Event* newEvent);
	void printEvents()const;
	int countEvents() const;

	Order* getNextVIPOrder() ;
	void addToVIPWait(Order* pOrder); 
	int  countVIPOrders()const;            
	void printVIPOrders()const;
	bool cancelEvent(int id);
	bool promoteOrder(int orderID, double extraMoney);

	Chief* getChiefInBreak(char chiefType);
	void addChiefToBreak(Chief* pChief);
	int  countInBreakChiefs()const ;
	void printInBreakChiefs()const;

	// nouran
	bool CancelOrder(int id);
	Order* GetNormalOrder(int id);
	void addNormalOrder(Order* order);
	void LoadInputFile(string filename);
	void PrintReadyNormalChefs();
	void PrintNormalOrders()const;

	//mariam
	bool Insert_vegan_order(Order* vo);
	bool Remove_vegan_order(Order* vo);
	bool Searchfor_vegan_order(Order* vo);
	Order get_next_vegan_order(Node<Order>* curr);
	bool Insert_order(Order* O);
	bool Remove_done_order_from_Inservice(int current_time);
	bool Insert_vegan_chief(Chief* ch);
	bool Remove_vegan_chief(Chief* ch);
	bool Searchfor_available_vegan_chief(Chief* ch);
	Chief get_next_available_vegan_chief(Node<Chief>* curr_chief);
	void print_all_available_vegan_chiefs();
	void PrintVeganOrders() const;
	void printInServeOrders() const;

    // basmala
	bool addChef(Chief* chef);
	bool getNextChef(Chief* chef);
	void printAllVIPChiefs() const;
	bool peekNextChef(Chief* chef) const;
	bool isEmpty() const;

	bool push(Order* order);
	ArrayStack<Order*>& getDeliveredOrders();
	bool pop(Order*& order);
	bool peek(Order*& order) const;
	void printAll_delivered_order();

	
};

