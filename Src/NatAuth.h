
#ifndef NAT_AUTH_H_H_H
#define NAT_AUTH_H_H_H

#include "gstdh.h"

class NatAuth
{
public:
	static NatAuth *GetInstance();
	~NatAuth();


	void LoadAllMap();
	void LoadAddMap();

	void OutputAllMap();

	CString GetAuth(const CString &str)
	{
		map<CString,CString>::iterator iter = mac_auth_.find(str);
		if (iter != mac_auth_.end())
		{
			return iter->second;
		}
		else
		{
			return "";
		}
	}

private:
	void LoadMap(const char *filepath);
	map<CString,CString>mac_auth_;

	NatAuth();
	
	static NatAuth *instance_;

};
#endif