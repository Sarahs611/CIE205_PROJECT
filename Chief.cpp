#include "Chief.h"
#include <iostream>
using namespace std;

int Chief::getCheifID() const
{
    return C_ID;
}
string Chief::get_name()const
{
	return name;
}
char Chief::get_chief_specialization()const
{
	return chief_specialization;
}
int Chief::get_speed()const
{
	return speed;
}
int Chief::get_break_duration()const
{
	return break_duration;
}
void Chief::set_name(string n)
{
	name = n;
}
void Chief::set_chief_specialization(char t)
{
	chief_specialization = t;
}
void Chief::set_speed(int s)
{
	speed = s;
}
void Chief::set_break_duration(int d)
{
	break_duration = d;
}

void Chief::print() const {
	cout << "Cheif name " << name<< ", Cheif specialization: " << chief_specialization 
		<< ", Chief ID: " << C_ID;
}