
#include "StdAfx.h"
#include "ProtocolConfig.h"


ProtocolConfig::ProtocolConfig() :
http_request_method_(0)
{
	
}

ProtocolConfig::~ProtocolConfig()
{
	
}

int ProtocolConfig::Load(const string &path1)
{
	path = (char *)path1.c_str();
	TiXmlDocument *doc = new TiXmlDocument(path);
	
	doc->LoadFile();
	
	TiXmlElement *root = doc->RootElement();
	
	if (root)
	{
		if (LoadPro(root))
		{
			TiXmlElement *conditions = root->FirstChildElement("conditions");
			if (conditions)
			{
				if (LoadConditions(conditions))
				{
					;
				}
				else
				{
					printf("%s LoadConditions() error.\r\n",path);
					Unload();
					return 0;
				}
			}

			
			TiXmlElement *processes = conditions->NextSiblingElement("processes");
			if (processes)
			{
				if (LoadProcesses(processes))
				{
					;
				}
				else
				{
					printf("%s LoadProcesses() error.\r\n",path);
					Unload();
					return 0;
				}
			}
		}
		else
		{
			printf("%s LoadPro() error.\r\n",path);
			Unload();
			return 0;
		}
	}
	
	return 1;
}

int ProtocolConfig::LoadPro(TiXmlElement *root)
{
	TiXmlAttribute *attribute = NULL;
	if (strcmp(root->Value(),"protocol") == 0)
	{
		attribute = root->FirstAttribute();
		if (attribute)
		{
			if (strcmp(attribute->Name(),"pro") == 0)
			{
				pro_ = attribute->Value();
			}
			else
			{
				return 0;
			}
		}
		else
		{	
			return 0;
		}
	}
	else
	{
		return 0;
	}
	return 1;
}

int ProtocolConfig::LoadConditions(TiXmlElement *conditions)
{
	TiXmlAttribute *attribute = NULL;

	attribute = conditions->FirstAttribute();
	if (strcmp(attribute->Name(),"http_request_method") == 0)
	{
		if (strcmp(attribute->Value(),"post") == 0)
		{
			http_request_method_ = kPOST;
		}
		else if (strcmp(attribute->Value(),"get") == 0)
		{
			http_request_method_ = kGET;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	TiXmlElement *condition = conditions->FirstChildElement("condition");
	
	while(condition)
	{
		ProtocolCondition *pc = new ProtocolCondition();
		attribute = condition->FirstAttribute();
		while(attribute)
		{
			if (strcmp(attribute->Name(),"position") == 0)
			{
				pc->position = attribute->Value();
			}
			else if (strcmp(attribute->Name(),"value") == 0)
			{
				pc->value = attribute->Value();
			}
			else if(strcmp(attribute->Name(),"compare_mode") == 0)
			{
				if (strcmp(attribute->Value(),"find") == 0)
				{
					pc->compare_mode = kFind;
				}
				else if (strcmp(attribute->Value(),"equal") == 0)
				{
					pc->compare_mode = kEqual;
				}
				else
				{
					delete pc;
					return 0;
				}
			}
			else
			{
				delete pc;
				return 0;
			}
			
			attribute = attribute->Next();
		}
		condition_.push_back(pc);
		condition = condition->NextSiblingElement("condition");
	}
	return 1;
}

int ProtocolConfig::LoadProcesses(TiXmlElement *processes)
{
	TiXmlAttribute *attribute = NULL;
	
	TiXmlElement *process = processes->FirstChildElement("process");
	while(process)
	{
		ProtocolProcess *pp = new ProtocolProcess();

		attribute = process->FirstAttribute();
		if (strcmp(attribute->Name(),"position") == 0)
		{
			pp->position = attribute->Value();
		}
		else
		{
			delete pp;
			return 0;
		}

		TiXmlElement *operations = process->FirstChildElement("operations");
		if (operations)
		{
			if (LoadOperations(operations,pp))
			{
				;
			}
			else
			{
				delete pp;
				return 0;
			}
		}
		else
		{
			delete pp;
			return 0;
		}


		TiXmlElement *items = operations->NextSiblingElement("items");
		if (items)
		{
			if (LoadItems(items,pp))
			{
				;
			}
			else
			{
				delete pp;
				return 0;
			}
		}
		else
		{
			delete pp;
			return 0;
		}

		process_.push_back(pp);
		process = process->NextSiblingElement("process");
	}	
	return 1;
}

int ProtocolConfig::LoadOperations(TiXmlElement *operations, ProtocolProcess *pp)
{
	TiXmlAttribute *attribute = NULL;
	TiXmlElement *operation = operations->FirstChildElement("operation");
	
	while(operation)
	{
		ProtocolOperation *po = new ProtocolOperation();
		attribute = operation->FirstAttribute();
		while(attribute)
		{
			if (strcmp(attribute->Name(),"type") == 0)
			{
				po->type = attribute->Value();
			}
			else if(strcmp(attribute->Name(),"param1") == 0)
			{
				po->param1 = attribute->Value();
			}
			else if (strcmp(attribute->Name(),"param2") == 0)
			{
				po->param2 = attribute->Value();
			}
			else
			{
				delete po;
				return 0;
			}
			attribute = attribute->Next();
		}
		
		pp->operations_.push_back(po);
		operation = operation->NextSiblingElement("operation");
		
	}
	return 1;
}

int ProtocolConfig::LoadItems(TiXmlElement *items, ProtocolProcess *pp)
{

	TiXmlAttribute *attribute = NULL;
	TiXmlElement *item = items->FirstChildElement("item");
	
	while(item)
	{
		ProtocolItem *pi = new ProtocolItem();
		attribute = item->FirstAttribute();
		while(attribute)
		{
			if (strcmp(attribute->Name(),"key") == 0)
			{
				pi->key = attribute->Value();
			}/*
			else if (strcmp(attribute->Name(),"transcode_mode") == 0)
			{
				pi->transcode_mode = attribute->Value();
			}*/
			else if(strcmp(attribute->Name(),"trim_left") == 0)
			{
				pi->trim_left = attribute->Value();
			}
			else if (strcmp(attribute->Name(),"allow_null") == 0)
			{
				if (strcmp(attribute->Value(),"true") == 0)
				{
					pi->allow_null = true;
				}
				else if (strcmp(attribute->Value(),"false") == 0)
				{
					pi->allow_null = false;
				}
				else
				{
					delete pi;
					return 0;
				}
			}
			else
			{
				delete pi;
				return 0;
			}
			attribute = attribute->Next();
		}
		
		pp->items_.push_back(pi);
		item = item->NextSiblingElement("item");
		
	}
	return 1;
}

int ProtocolConfig::Unload()
{
	for (list<ProtocolCondition *>::iterator ipc = condition_.begin(); ipc != condition_.end(); ++ipc)
	{
		delete (*ipc);
	}

	for (list<ProtocolProcess *>::iterator ipp = process_.begin(); ipp != process_.end(); ++ipp)
	{
		for (list<ProtocolOperation *>::iterator ipo = (*ipp)->operations_.begin(); ipo != (*ipp)->operations_.end(); ++ipo)
		{
			delete (*ipo);
		}

		for (list<ProtocolItem *>::iterator ipi = (*ipp)->items_.begin(); ipi != (*ipp)->items_.end(); ++ipi)
		{
			delete (*ipi);
		}
		delete (*ipp);
	}

	return 1;
}