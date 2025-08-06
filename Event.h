#pragma once
#include <iostream>
#include <string>
using namespace std;
class Restaurant;

class Event
{
protected:
	int eventTime, orderID;
public:
	Event(int eTime, int orderId)
	{
		eventTime = eTime;
		orderID = orderId;
	}
	int getEventTime() const;
	int getOrderID() const;

	virtual void Execute(Restaurant* Rest) = 0;
	virtual void print() const;
	virtual string getType() const = 0;
	virtual ~Event() {}
};

class Arrival : public Event {
private:
	string orderType;
	int size;
	double price;
public:
	Arrival(string ordertype, int Size, double Price, int eTime, int orderId) :
		Event(eTime, orderId) {
		orderType = ordertype;
		size = Size;
		price = Price;
	}
	string getOrderType() const;
	int getSize() const;
	double getPrice() const;
	string getType() const override;

	void print() const override;
	void Execute(Restaurant* Rest) override;

};

class Cancel : public Event {

public:
	Cancel(int eTime, int orderId) : Event(eTime, orderId) {}

	void print() const override;
	void Execute(Restaurant* Rest) override;
	string getType() const override;

};

class Promotion :public Event {
private:
	double extraMoney;
public:
	Promotion(double extram, int eTime, int orderId) :
		Event(eTime, orderId) {
		extraMoney = extram;
	}
	void print() const override;
	void Execute(Restaurant* Rest) override;
	string getType() const override;

};
