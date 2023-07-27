#include "StdAfx.h"
#include "InstalledYingYongBao.h"

installed::YingYongBao::YingYongBao(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "应用宝";
}

installed::YingYongBao::~YingYongBao()
{
	
}

int installed::YingYongBao::IsYingYongBao(HttpSession *http_session)
{
	//取消host的判断 只使用url
	//if (http_session->m_Requestline.m_Host.Find("223.202.25.30") != -1 || http_session->m_Requestline.m_Host.Find("ams.lenovomm.com") != -1)
	//{
		if (http_session->m_Requestline.m_Host.Find("oth.eve.mdt.qq.com:8080") != -1 && http_session->m_Requestline.m_URI.Find("/analytics/upload") != -1) //
		{
			//有压缩
			return kSoftwareUpdate2;
		} 
		else if(http_session->m_Requestline.m_URI.Find("/ams/api/queryupgradeSmart") != -1 && http_session->m_Requestline.m_URI.Find("/ams/api/queryupgradeSmartv2") == -1)
		{
			//明文
			return kSoftwareUpdate1;
		}	
	//}
	return 0;
}

int installed::YingYongBao::Process(const char *packet,int action)
{
	//没有压缩
	if (action == kSoftwareUpdate1)
	{
		return SoftwareUpdate1(packet);
	}
	//有压缩
	if (action == kSoftwareUpdate2)
	{
		return SoftwareUpdate2(packet);
	}
	return 0;
}

//没有压缩
int installed::YingYongBao::SoftwareUpdate1(const char *packet)
{
	int retval = 0;
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("data");	 
	
	ms_->Replace(src_data,"[{","");
	ms_->Replace(src_data,"}]","&"); //--
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"\"","");

	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);;
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
		installed_ = ms_->GetValueFromMapByKey("packagename");
	 	if (!item.empty())
	 	{
	 		installed_ = GetAppName(item);
	 		if (installed_.size() < 50)
	 		{
	 			output_text_ = "webname=" + installed_ + "\r\n" + "type=手机\r\n" + "msg=" + app_ + "\r\n" + "state=安装\r\n";
	 			WriteIndexFile();
	 			retval = 1;
	 		}
	 	}
	}
	return retval;
}

//有压缩处理
int installed::YingYongBao::SoftwareUpdate2(const char *packet)
{
	//解压缩处理
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
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("data");	 
	
	ms_->Replace(src_data,"[{","");
	ms_->Replace(src_data,"}]","&"); //--
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"\"","");

	string item;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);;
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
		installed_ = ms_->GetValueFromMapByKey("packagename");
	 	if (!item.empty())
	 	{
	 		installed_ = GetAppName(item);
	 		if (installed_.size() < 50)
	 		{
	 			output_text_ = "webname=" + installed_ + "\r\n" + "type=手机\r\n" + "msg=" + app_ + "\r\n" + "state=安装\r\n";
	 			WriteIndexFile();
	 			retval = 1;
	 		}
	 	}
	}
 	return retval;
}