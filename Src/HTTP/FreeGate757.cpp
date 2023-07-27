
#include "StdAfx.h"
#include "FreeGate757.h"

FreeGate757 *FreeGate757::instance = NULL;

FreeGate757 *FreeGate757::GetInstance()
{
	if (instance == NULL)
	{
		instance = new FreeGate757();
	}
	return instance;
}

FreeGate757::FreeGate757()
{

}

FreeGate757::~FreeGate757()
{

}

void FreeGate757::Insert(const CString &ip, time_t t)
{
	camap.insert(make_pair(ip, t));
}

bool FreeGate757::FindIp(const CString &ip)
{
	map<CString , time_t>::iterator iter = camap.find(ip);
	if (iter != camap.end())
	{
		time_t t = iter->second;
		camap.erase(iter);
		if ((time(0) - t) < 240)
		{
			return true;
		}
	}
	return false;
}
