#pragma once
#include <iostream>
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
	virtual char getType() const = 0;
	virtual ~Event() {}
};

class Arrival : public Event {
private:
	char orderType;  
	int size; 
	double price;
public:
	Arrival(char ordertype, int Size, double Price, int eTime, int orderId) :
		Event(eTime, orderId) {
		orderType = ordertype;
		size = Size;
		price = Price;
	}
	char getOrderType() const;
	int getSize() const;
	double getPrice() const;
	char getType() const override;

	void print() const override;
	void Execute(Restaurant* Rest) override;

};

class Cancel: public Event{

public:
	Cancel(int eTime,int orderId): Event(eTime, orderId){}
	
	void print() const override;
	void Execute(Restaurant* Rest) override;
	char getType() const override;

};

class Promotion:public Event {
private:
	double extraMoney;
public:
	Promotion(double extram,int eTime, int orderId) :
		Event(eTime, orderId) {
		extraMoney = extram;
	}
	void print() const override;
	void Execute(Restaurant* Rest) override;
	char getType() const override;

};
