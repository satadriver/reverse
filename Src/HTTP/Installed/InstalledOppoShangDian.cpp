#include "StdAfx.h"
#include "InstalledOppoShangDian.h"

installed::OppoShangDian::OppoShangDian(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "oppo应用商店";
}

installed::OppoShangDian::~OppoShangDian()
{
	
}

int installed::OppoShangDian::IsOppoShangDian(HttpSession *http_session)
{
	//软件更新
	if( ( http_session->m_Requestline.m_Host.Find("i3.store.nearme.com.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/client/check_upgrade.pb") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::OppoShangDian::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}

//软件更新
int installed::OppoShangDian::SoftwareUpdate(const char *packet)
{
	string src_dataPre(packet);

//***********************************测试文件记得删除begin***************************************//
// 	CStdioFile file;
// 	file.Open("E:\\oppo.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate); 
// 	file.SeekToEnd();                
// 	file.WriteString(src_dataPre.c_str());  
// 	file.WriteString("\r\n");	     
// 	file.Close();
//***********************************测试文件记得删除end***************************************//


AfxMessageBox(src_dataPre.c_str());
	ms_->SplitStringToMap(src_dataPre,'&','=');
	//获取包
    string src_data;
	src_data= ms_->GetValueFromMapByKey("packages",true);
	int retval = 0;

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_data.find(",",item_beg_pos);
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
			item_beg_pos = item_end_pos + strlen(",");
		}
		
		string::size_type tmp_pos = item.find("|");
		if (tmp_pos != string::npos)
		{

			installed_ = item.substr(0, tmp_pos);
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