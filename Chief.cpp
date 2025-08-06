#include "Chief.h"
#include <iostream>
using namespace std;

Chief::Chief()
{
}

Chief::Chief(int id, string type, int spd, int ordersBeforeBreak, int breakTime)
{
	Chief_ID = id;
	chief_specialization = type;
	speed = spd;
	orders_before_break = ordersBeforeBreak;
	maxOrdersBeforeBreak = ordersBeforeBreak;
	break_duration = breakTime;
	breakEndT = -1;
	curr_Speed = spd;
	available = true; injured = false; ComeBackEarly = false;
	inj_RestT = 0;inj_EndT = 0;
}

int Chief::getChiefID()const
{
	return Chief_ID;
}
string Chief::getchiefspecialization()const
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

int Chief::getMaxOrdersBeforeBreak() const
{
	return maxOrdersBeforeBreak;
}

int Chief::getbreakduration()const
{
	return break_duration;
}

bool Chief::get_available() const
{
	return available;
}

int Chief::getBreakEndTime() const
{
	return breakEndT;
}

int Chief::getcurrSpeed() const
{
	return curr_Speed;
}

int Chief::getinjRT() const
{
	return inj_RestT;
}

int Chief::getinjET() const
{
	return inj_EndT;
}

bool Chief::isInjured() const
{
	return injured;
}

bool Chief::ComeBackE() const
{
	return ComeBackEarly;
}

void Chief::setChiefID(int id)
{
	Chief_ID = id;
}
void Chief::setchiefspecialization(string t)
{
	chief_specialization = t;
}
void Chief::setspeed(int s)
{
	speed = s;
}
void Chief::setordersbeforebreak(int bb)
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

void Chief::setBreakEndTime(int t)
{
	breakEndT = t;
}

void Chief::print() const {
	cout << "Chef ID: " << getChiefID() << " | Specialization: " << getchiefspecialization() << " | Speed: " << getspeed()
		<< " | Orders before break: " << getordersbeforebreak() << " | Break duration: " << getbreakduration() << endl;
}

void Chief::recover(int currentTime) {
	if (injured && currentTime >= inj_EndT) {
		injured = false;
		ComeBackEarly = false;
		curr_Speed = speed;
		available = true;
		// reset workload
		setordersbeforebreak(getMaxOrdersBeforeBreak());
	}
}

void Chief::needRest(int restTime, int currentTime)
{
	injured = true;
	inj_RestT = restTime;
	inj_EndT = currentTime + restTime;
	ComeBackEarly = false;
	available = false;
}

void Chief::forceBack(int currentTime) {
	if (injured && currentTime < inj_EndT) {
		ComeBackEarly = true;
		curr_Speed = speed / 2;
		available = true;

		setordersbeforebreak(getMaxOrdersBeforeBreak());
	}
}