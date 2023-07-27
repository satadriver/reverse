
#ifndef HTTP_CONTACTS_H_H_H
#define HTTP_CONTACTS_H_H_H

#include "../../HttpPro.h"

class Contacts : public HttpPro
{
public:
	Contacts();
	~Contacts();

//	void WriteIndexFile();
private:
	string longitude_;
	string latitude_;
};

#endif