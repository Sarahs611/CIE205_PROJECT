#include "Chief.h"
#include <iostream>
using namespace std;


int Chief::getChiefID()const
{
	return Chief_ID;
}
char Chief::getchiefspecialization()const
{
	return chief_specialization;
}
int Chief::getspeed()const
{
	return speed;
}
int Chief::getordersbeforebreak()const
{
	return orders_before_break;
}

int Chief::getbreakduration()const
{
	return break_duration;
}

void Chief::setChiefID(int id)
{
	Chief_ID = id;
}
void Chief::setchiefspecialization(char t)
{
	chief_specialization = t;
}
void Chief::setspeed(int s)
{
	speed = s;
}
void Chief::ordersbeforebreak(int bb)
{
	orders_before_break = bb;
}
void Chief::setbreakduration(int d)
{
	break_duration = d;
}

void Chief::print() const {
	cout << "Cheif Id" << Chief_ID << ", Cheif specialization: " << chief_specialization
		<< ", Chief speed: " << speed << "orders before break" << orders_before_break << ",break duration" << break_duration << endl;
}