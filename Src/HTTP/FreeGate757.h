#ifndef FREEGATE757_H_H_H
#define FREEGATE757_H_H_H

class FreeGate757 
{
public:
	static FreeGate757 *GetInstance();
	~FreeGate757();

	void Insert(const CString &ip, time_t t);
	bool FindIp(const CString &ip);
private:
	static FreeGate757 *instance;
	FreeGate757();
	map<CString, time_t >camap;


};
#endif