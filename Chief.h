#pragma once
using namespace std;
class Chief
{
private:
	string name;
	char chief_specialization;
	int C_ID;
	int speed;
	int break_duration;
   
public:
	string get_name()const;
	char get_chief_specialization()const;
	int get_speed()const;
	int get_break_duration()const;
	void set_name(string n);
	void set_chief_specialization(char t);
	void set_speed(int s);
	void set_break_duration(int d);
	int getCheifID() const;
	void print() const;
};

