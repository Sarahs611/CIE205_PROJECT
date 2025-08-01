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
	int breakEndT = -1;

public:
	Chief();
	Chief(int id, char type, int spd, int ordersBeforeBreak, int breakTime) {
		Chief_ID = id;
		chief_specialization = type;
		speed = spd;
		orders_before_break = ordersBeforeBreak;
		break_duration = breakTime;
		available = true;
	}
	int  getChiefID()const;
	char getchiefspecialization()const;
	int  getspeed()const;
	int  getordersbeforebreak()const;
	int  getbreakduration()const;
	bool get_available() const;
	int getBreakEndTime() const;

	void setChiefID(int id);
	void setchiefspecialization(char t);
	void setspeed(int s);
	void setordersbeforebreak(int b);
	void setbreakduration(int d);
	void set_available(bool a);
	void setBreakEndTime(int t);

	void print()const;
};

