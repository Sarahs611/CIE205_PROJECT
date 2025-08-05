#pragma once
#include <iostream>
using namespace std;

class Chief
{
private:
	int Chief_ID;
	string chief_specialization;
	int speed;
	int orders_before_break;
	int maxOrdersBeforeBreak;
	int break_duration;
	bool available;
	int breakEndT = -1;

////////////BOUNS/////////////////////
	int curr_Speed;
	bool injured;
	int inj_RestT;
	int inj_EndT;  
	bool ComeBackEarly;

public:
	Chief();
	Chief(int id,string type, int spd, int ordersBeforeBreak, int breakTime);
	int  getChiefID()const;
	string getchiefspecialization()const;
	int  getspeed()const;
	int  getordersbeforebreak()const;
	int getMaxOrdersBeforeBreak() const;
	int  getbreakduration()const;
	bool get_available() const;
	int getBreakEndTime() const;
	int getcurrSpeed() const;
	int getinjRT()const;
	int getinjET()const;
	bool isInjured() const;
	bool ComeBackE() const;

	void setChiefID(int id);
	void setchiefspecialization(string t);
	void setspeed(int s);
	void setordersbeforebreak(int b);
	void setbreakduration(int d);
	void set_available(bool a);
	void setBreakEndTime(int t);

	void print()const;
	void recover(int currentTime);
	void needRest(int restTime, int currentTime);
	void forceBack(int currentTime);


};

