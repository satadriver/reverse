
#ifndef PHONE_NUMBER_LOCATION_H_H_H
#define PHONE_NUMBER_LOCATION_H_H_H

#include "gstdh.h"



class PhoneNumberLocation
{
public:
	static PhoneNumberLocation *Instance();
	~PhoneNumberLocation();

	void GetLocationInfo(const string &phone_number,string &province,string &city,string &sp);

private:
	PhoneNumberLocation();
	void Load();

	map<string,string >phonenumberleft7_locationinfo_;

	static PhoneNumberLocation *instance_;
};
#endif
