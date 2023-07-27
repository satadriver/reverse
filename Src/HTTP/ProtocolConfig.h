
#ifndef PROTOCOL_CONFIG_H_H_H
#define PROTOCOL_CONFIG_H_H_H
#pragma warning(disable:4786)

#include "../gstdh.h"

struct ProtocolCondition
{
	string position;
	string value;
	int compare_mode;

};

struct ProtocolOperation
{
	string type;
	string param1;
	string param2;
	
};

struct ProtocolItem
{
	string key;
	
	string trim_left;
	string right_trim;
	string removed;
	
	bool to_gbk;
	bool allow_null;
};

struct ProtocolProcess
{
	string position;

	list<ProtocolOperation *>operations_;
	list<ProtocolItem *>items_;
};


class ProtocolConfig
{
public:
	ProtocolConfig();
	~ProtocolConfig();

//	int Load(const char *path);
	int Load(const string &path);
	int Unload();




	int LoadPro(TiXmlElement *root);
	int LoadConditions(TiXmlElement *conditions);
	int LoadProcesses(TiXmlElement *processes);
	int LoadOperations(TiXmlElement *operations, ProtocolProcess *pp);
	int LoadItems(TiXmlElement *items, ProtocolProcess *pp);




	char *path;

	string pro_;

	int http_request_method_;

	list<ProtocolCondition *>condition_;

	list<ProtocolProcess *>process_;

	enum CompareMode
	{
		kFind = 1,
			kEqual = 2
	};
	
	enum HttpRequestMethod
	{
		kPOST = 1,
			kGET = 2
	};
};
#endif