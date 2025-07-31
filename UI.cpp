#include "UI.h"
#include "Restaurant.h"

int UI::selectMode()
{
	int mode;
	cout << "Select mode: 1- Interactive  2- Silent \nChoice: ";
	cin >> mode;
	return mode;
}

void UI::printCurrentStatus(Restaurant* rest, int timestep)
{
	cout << "\nCurrent Timestep: " << timestep << "\n";

	cout << "Frist 10 events : ";
	rest->printFTenEvents();


	cout << "Waiting VIP Orders:\n";
	rest->printVIPOrders();

	cout << "\nWaiting Normal Orders:\n";
	rest->PrintNormalOrders();

	cout << "\nWaiting Vegan Orders:\n";
	rest->PrintVeganOrders();

	cout << "\nIn-Serve Orders:\n";
	rest->printInServeOrders();

	cout << "\nDelivered Orders:\n";
	rest->printAll_delivered_order();


	cout << "\nAvailable VIP Chiefs:\n";
	rest->printAllVIPChiefs();

	cout << "\nAvailable Normal Chiefs:\n";
	rest->PrintReadyNormalChefs();

	cout << "\nAvailable Vegan Chefs:\n";
	rest->print_all_available_vegan_chiefs();

	cout << "\nIn Break Chiefs:\n";
	rest->printInBreakChiefs();
}