#include "Chief.h"
#include <iostream>
using namespace std;


Chief::Chief()
{
	Chief_ID = -1;                
	chief_specialization = 'N';  
	speed = 0;
	orders_before_break = 0;
	break_duration = 0;
	available = true;
}

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

bool Chief::get_available() const
{
	return available;
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
void Chief::set_available(bool a) {
	available = a;
}

void Chief::print() const {
	cout << "Cheif Id" << getChiefID() << ", Cheif specialization: " << getchiefspecialization()
		<< ", Chief speed: " << getspeed() << "orders before break" << getordersbeforebreak() << ",break duration" << getbreakduration() << endl;
}