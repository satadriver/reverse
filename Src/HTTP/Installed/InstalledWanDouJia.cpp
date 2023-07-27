
#include "StdAfx.h"

#include "InstalledWanDouJia.h"

installed::WanDouJia::WanDouJia(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "豌豆荚";
}

installed::WanDouJia::~WanDouJia()
{
	
}

int installed::WanDouJia::IsWanDouJia(HttpSession *http_session)
{
	//取消host的判断 只使用url
	if( /*( http_session->m_Requestline.m_Host.Find("apps.wandoujia.com")  != -1 ) && */
		( http_session->m_Requestline.m_URI.Find("/v1/update") != -1 ) )	
	{
		return kSoftwareUpdate1;
	}
	/*
		1 Host: api.wandoujia.com	  POST /apm/updateinfo/v2 2016-08-15更新
	*/
	if( ( http_session->m_Requestline.m_Host.Find("api.wandoujia.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v2/update") != -1 ) )	
	{
		return kSoftwareUpdate2;
	}
	if (http_session->m_Requestline.m_Host.Find("w.api.pp.cn") != -1 &&
		http_session->m_Requestline.m_URI.Find("/api/resource.app.getType") != -1)
	{
		return kSoftwareUpdate3;
	}
	return 0;
}

int installed::WanDouJia::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate1)
	{
		return SoftwareUpdate1(packet);
	}
	if (action == kSoftwareUpdate2)
	{
		return SoftwareUpdate2(packet);
	}
	if (action == kSoftwareUpdate3)
	{
		return SoftwareUpdate3(packet);
	}
	return 0;
}


int installed::WanDouJia::SoftwareUpdate1(const char *packet)
{
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"[","&");
	ms_->Replace(src_data,"]","&");
	ms_->Replace(src_data,"}","&");
	ms_->Replace(src_data,"{","&");
	ms_->Replace(src_data,"\"","");

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 

	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = src_data.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = src_data.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("&");
		}

		ms_->SplitStringToMap(item,',',':');
		installed_ = ms_->GetValueFromMapByKey("title");
		if (!installed_.empty())
		{
			if (installed_.size() < 50)
			{
				output_text_ =
					"webname=" + installed_ + "\r\n" +
					"type=手机\r\n" +
					"msg=" + app_ + "\r\n" +
					"state=安装\r\n";
				WriteIndexFile();
				retval = 1;
			}
		}
	}
	return retval;
}

int installed::WanDouJia::SoftwareUpdate2(const char *packet)
{
	//解压 1
	// 	unsigned long zlen = http_session_->m_datalen;
	// 	unsigned long len = zlen * 5;
	// 	
	// 	char *data = new char[len];
	// 	if (-1 == Compress::DecompressGZIP((unsigned char *)(packet + 10),zlen,(unsigned char *)data,&len))
	// 	{
	// 		return 0;
	// 	}
	//解压 2
	string src_data;
	unsigned long out_length = 0;
	unsigned char* data_buff = NULL;
	vector<Byte *> data_vec;
	//解压函数
	if (-1 == Compress::Degzip2((unsigned char *)(packet + 10), (unsigned long)(packet - 10), data_vec, &out_length)) 
	{
		return 0;
	}	
	if (out_length < 10)
	{
		return -1;
	}	
	data_buff = new Bytef[out_length + 1];
	memset(data_buff, 0x00, out_length + 1);
	for(int i = 0; i < data_vec.size(); i++)
	{
		if (i == data_vec.size() - 1)
		{
			memcpy(data_buff + (i * 1024), data_vec[i], out_length - (i * 1024));
		}
		else
		{
			memcpy(data_buff + (i * 1024), data_vec[i], 1024);
		}
		delete data_vec[i];
		data_vec[i] = NULL;
	}
	src_data = string((char *)data_buff, out_length);
	delete []data_buff;
	data_buff = NULL;
	//判断是否为空
	if (src_data.empty())
	{
		return 0;
	}	
 	int retval = 0;	
	//存放拆分出来的小单元数组
	CStringArray strArray;
	strArray.RemoveAll();
    //使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		//userApps
		const Json::Value arrayObj = value["userApps"];
		for (int j = 0;j <arrayObj.size();j++)
		{
			string out = arrayObj[j]["p"].asString();
			strArray.Add(out.c_str());
		}
		//sysApps
		const Json::Value arrayObj1 = value["sysApps"];
		for (int h = 0;h <arrayObj1.size();h++)
		{
			string out1 = arrayObj1[h]["p"].asString();
			strArray.Add(out1.c_str());
		}
	}
	//取出数据
	int nArrayCount = strArray.GetSize();
	for (int k = 0;k <nArrayCount;k++)
	{
		string installed_ = strArray[k].GetBuffer(0);
		if (!installed_.empty())
		{
			installed_ = GetAppName(installed_);
			if (installed_.size() < 50)
			{
				output_text_ =
					"webname=" + installed_ + "\r\n" +
					"type=手机\r\n" +
					"msg=" + app_ + "\r\n" +
					"state=安装\r\n";
				WriteIndexFile();
				retval = 1;
			}	
		}		
	}
	return retval;
}

int installed::WanDouJia::SoftwareUpdate3(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("{\"id\":") != -1)
	{
		src_data = src_data.substr(src_data.rfind("{\"id\":"));
	}
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		const Json::Value data = value["data"];
		const Json::Value packageNames = data["packageNames"];

		for (int j= 0; j<packageNames.size() ;j++)
		{
           installed_ = packageNames[j].asString();
		   installed_ = GetAppName(installed_);
		   output_text_ =
			   "webname=" + installed_ + "\r\n" +
			   "type=手机\r\n" +
			   "msg=" + app_ + "\r\n" +
			   "state=安装\r\n";
			WriteIndexFile();
		}
	}
	return 0;
}