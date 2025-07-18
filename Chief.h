#pragma once
using namespace std;
class Chief

{
private:
	char CType;  int C_ID;
public:
	Chief(int id, char type) {
		C_ID = id;
		CType = type;
	}
	char getCheifType() const;
	int getCheifID() const;
	void print() const;
};

