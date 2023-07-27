#include "StdAfx.h"
#include "InstalledPPzhushou.h"

installed::PPzhushou::PPzhushou(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "pp助手";
}

installed::PPzhushou::~PPzhushou()
{
	
}

int installed::PPzhushou::IsPPzhushou(HttpSession *http_session)
{
	//检查软件更新
	if( ( http_session->m_Requestline.m_Host.Find("sjzs-api.25pp.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/resource.app.getType") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::PPzhushou::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::PPzhushou::SoftwareUpdate(const char *packet)
{
	//数据部分
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("{\"sign\"") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("{\"sign\""));
	}
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	int retval = 0;
	//存放拆分出来的小单元数组
	CStringArray strArray;
	strArray.RemoveAll();
    //使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_dataEnd,value))
	{
		//data
		const Json::Value data = value["data"];
        //data - packageName
		const Json::Value packageNames = data["packageNames"];

		for (int j = 0;j <packageNames.size();j++)
		{
			string out = packageNames[j].asString();
			strArray.Add(out.c_str());
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