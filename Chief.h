#pragma once
#include <iostream>
using namespace std;

class Chief
{
private:
	int Chief_ID;
	char chief_specialization;
	int speed;
	int orders_before_break;
	int break_duration;
	bool available;

public:
	Chief();
	Chief(int id, char type, int spd, int ordersBeforeBreak, int breakTime) {
		Chief_ID = id;
		chief_specialization = type;
		speed = spd;
		orders_before_break = ordersBeforeBreak;
		break_duration = breakTime;
	}
	int  getChiefID()const;
	char getchiefspecialization()const;
	int  getspeed()const;
	int  getordersbeforebreak()const;
	int  getbreakduration()const;
	void setChiefID(int id);
	void setchiefspecialization(char t);
	void setspeed(int s);
	void ordersbeforebreak(int b);
	void setbreakduration(int d);
	void print()const;
};

