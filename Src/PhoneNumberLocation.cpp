
#include "StdAfx.h"

#include "PhoneNumberLocation.h"

PhoneNumberLocation *PhoneNumberLocation::instance_ = NULL;

PhoneNumberLocation *PhoneNumberLocation::Instance()
{
	if ( instance_ == NULL)
	{
		instance_ = new PhoneNumberLocation();	
	}
	return instance_;
}

PhoneNumberLocation::PhoneNumberLocation()
{
	Load();
}

PhoneNumberLocation::~PhoneNumberLocation()
{

}

void PhoneNumberLocation::GetLocationInfo(const string &phone_number,string &province,string &city,string &sp)
{
	if (phone_number.size() == 11)
	{
		map<string,string>::iterator iter = phonenumberleft7_locationinfo_.find(phone_number.substr(0,7));
		if (iter != phonenumberleft7_locationinfo_.end())
		{
			string location_info = iter->second;

			vector<string>item_list;
			string::size_type item_beg_pos = 0;
			string::size_type item_end_pos = 0;
			while(1)
			{
				item_end_pos = location_info.find(",",item_beg_pos);
				string item;
				if(item_end_pos == string::npos)
				{
					if (item_beg_pos == string::npos)
					{
						break;
					}
					else
					{
						item = location_info.substr(item_beg_pos);
						item_beg_pos = string::npos;
					}
				}
				else
				{
					item = location_info.substr(item_beg_pos,item_end_pos - item_beg_pos);
					item_beg_pos = item_end_pos + 1/*strlen(",")*/;
				}
				item_list.push_back(item);
			}

			if (item_list.size() == 5)
			{
				province = item_list.at(0);
				city = item_list.at(1);
				sp = item_list.at(2);

				if (string::npos != sp.find("联通"))
				{
					sp = "联通";
				}
				if (string::npos != sp.find("移动"))
				{
					sp = "移动";
				}
				if (string::npos != sp.find("电信"))
				{
					sp = "电信";
				}
			}
		}
	}
}

void PhoneNumberLocation::Load()
{
	const char *path = "e:\\netspy\\para\\pnl.txt";
	const int size20M = 20 * 1024 * 1024;
	FILE *f = fopen(path,"r");
	if (!f)
	{
		return ;
	}
	char *rd = new char[size20M];
	memset(rd,0,size20M);
	fread(rd,1,size20M,f);
	fclose(f);
	string data(rd);
	delete [] rd;

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = data.find("\n",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = data.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = data.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + 1/*strlen("\n")*/;
		}
		
		string::size_type tmp_pos = item.find(",");
		if (tmp_pos == string::npos)
		{
			continue;
		}

		string phone_number(item.substr(0,tmp_pos));
		string location_info(item.substr(tmp_pos + 1/*strlen(",")*/));
		phonenumberleft7_locationinfo_.insert(std::make_pair(phone_number,location_info));
	}	
}

