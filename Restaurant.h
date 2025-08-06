#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Order.h"
#include "Chief.h" 
#include "ArrayStack.h"
#include <fstream>
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
	priQueue<Chief*>VIPChiefs;
	LinkedQueue<Chief*> InBreakN;
	LinkedQueue<Chief*> InBreakG;
	LinkedQueue<Chief*> InBreakVIP;

	//Nouran
	LinkedQueue<Order*> WaitNorm;

	//Mariam
	LinkedQueue<Order*> Waiting_Vegan;
	priQueue<Chief*> Ready_Vegan_chief;
	priQueue<Order*>In_service_orders;


	//Basmala

	ArrayStack<Order*> DeliveredOrders;

	priQueue<Chief*> readyNormalChiefs;
	///////////////////Bouns/////////////////////////////////

	priQueue<Chief*> Ready_Dessert_chief;
	priQueue<Chief*> Ready_Drink_chief;

	LinkedQueue<Order*>Waiting_Dessert;
	LinkedQueue<Order*>Waiting_Drink;

	LinkedQueue<Chief*> InBreakDes;
	LinkedQueue<Chief*> InBreakDri;

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

	Order* getNextVIPOrder();
	void addToVIPWait(Order* pOrder);
	int  countVIPOrders()const;
	void printVIPOrders()const;
	bool cancelEvent(int id);
	bool promoteOrder(int orderID, double extraMoney);

	Chief* getChiefInBreak(string chiefType);
	void addChiefToBreak(Chief* pChief);
	int  countInBreakChiefs()const;
	void printInBreakChiefs()const;

	///////////////////Bouns///////////////////////
	void InjuredChief(Chief* ch, int timestep);
	//////////////////////////////////////////////
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
	void PrintVeganOrders();
	void printInServeOrders();
	void OutputFile(ofstream& out);

	////////////////////////Bonus/////////////////////

	//Desserts orders
	bool Insert_dessert_order(Order* des);
	bool Remove_dessert_order(Order* des);
	bool Searchfor_dessert_order(Order* des);
	Order get_next_dessert_order(Node<Order>* curr);
	void Print_dessert_Orders();
	//Drink orders
	bool Insert_Drink_order(Order* Dri);
	bool Remove_Drink_order(Order* Dri);
	bool Searchfor_Drink_order(Order* Dri);
	Order get_next_Drink_order(Node<Order>* curr);
	void Print_Drink_Orders();

	//Dessert chiefs
	bool Insert_Dessert_chief(Chief* ch);
	bool Remove_Dessert_chief(Chief* ch);
	bool Searchfor_available_Dessert_chief(Chief* ch);
	Chief get_next_available_Dessert_chief(Node<Chief>* curr_chief);
	void print_all_available_Dessert_chiefs();
	//Drink chiefs
	bool Insert_Drink_chief(Chief* ch);
	bool Remove_Drink_chief(Chief* ch);
	bool Searchfor_available_Drink_chief(Chief* ch);
	Chief get_next_available_Drink_chief(Node<Chief>* curr_chief);
	void print_all_available_Drink_chiefs();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
		// basmala
	bool addChef(Chief* chef);
	bool getNextChef(Chief* chef);
	void printAllVIPChiefs() const;
	bool peekNextChef(Chief*& chef);
	bool isEmpty() const;

	bool push(Order* order);
	ArrayStack<Order*>& getDeliveredOrders();
	bool pop(Order*& order);
	bool peek(Order*& order) const;
	void printAll_delivered_order();
};

