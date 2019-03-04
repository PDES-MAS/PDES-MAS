#ifndef _SSVPORTACCESS_H_
#define _SSVPORTACCESS_H_

using namespace std;
#include "LpId.h"
#include "SsvId.h"
#include <iostream>
#include <cstring>
#include <string>

/******************************************
SsvPortAccess.h

Author:  
Date: (06/02/05)


Description: This class is used to compute and store the access cost of the variables in the CLP

 *******************************************/
namespace pdesmas{
class SsvPortAccess :public pdesmas::Serialisable
{

private:
	SsvId ssvId;
	Direction direction;

public:
	/* constructors */
	SsvPortAccess(); /* default constructor */
    ~SsvPortAccess();
	SsvPortAccess(SsvId, Direction);

	SsvPortAccess(const SsvPortAccess&); //copy contructor 
	SsvPortAccess& operator=(const SsvPortAccess&);  //an assignment operator; 

	void display() const;

	/* accessors */
	SsvId Get_ssvID() const;
	Direction Get_portID() const;

	/* modifers */
	void Set_portID(Direction);

	/* binary operations */
	friend bool operator<(const SsvPortAccess&, const SsvPortAccess&);
	friend bool operator ==(const SsvPortAccess&, const SsvPortAccess&);
	friend bool operator!=(const SsvPortAccess&, const SsvPortAccess&);
	friend bool operator>(const SsvPortAccess&, const SsvPortAccess&);
	
	/* 
	stream operators */
	friend std::ostream& operator<<(std::ostream&, const SsvPortAccess& );

	//overload the base methods 
	void Serialise(std::ostream&) const;			// Serialize the container
	void Deserialise(std::istream&);				// Deserialize the received message into the container
};
}
#endif
