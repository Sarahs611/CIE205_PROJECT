#include "Chief.h"
#include <iostream>
using namespace std;

char Chief::getCheifType() const
{
    return CType;
}

int Chief::getCheifID() const
{
    return C_ID;
}

void Chief::print() const {
    cout << "Chief ID: " << C_ID << ", Type: " << CType;
}
