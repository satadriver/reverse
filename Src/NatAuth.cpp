
#include "StdAfx.h"
#include "NatAuth.h"

NatAuth *NatAuth::instance_ = NULL;

NatAuth *NatAuth::GetInstance()
{
	if (instance_ == NULL )
	{
		instance_ = new NatAuth();
	}
	return instance_;
}

NatAuth::NatAuth()
{

}

NatAuth::~NatAuth()
{

}


void NatAuth::LoadAllMap()
{
	const char *filepath = "e:\\netspy\\pcap\\radius\\tmp\\natradius";
	LoadMap(filepath);
}

void NatAuth::LoadAddMap()
{
	WIN32_FIND_DATA wfd;
	const char *path = "e:\\netspy\\pcap\\radius\\tmp\\radius*.nat";
	HANDLE hFind = ::FindFirstFile(path,&wfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	else
	{
		do
		{
			if (wfd.cFileName[0] == '.')
			{
				continue;
			}
			
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				//
				continue;
			}
			

			string filepath = "e:\\netspy\\pcap\\radius\\tmp\\"  + string(wfd.cFileName);
			LoadMap(filepath.c_str());
			DeleteFile(filepath.c_str());

		}while(::FindNextFile(hFind,&wfd));
		FindClose(hFind);
	}
}


void NatAuth::OutputAllMap()
{
	const char *filepath = "e:\\netspy\\pcap\\radius\\tmp\\natradius";
	FILE *fp = ::fopen(filepath,"w+");
	if (fp)
	{
		for (map<CString,CString>::iterator iter = mac_auth_.begin(); iter != mac_auth_.end(); ++iter)
		{
			::fwrite(iter->first,1,iter->first.GetLength(),fp);
			::fwrite("\x01",1,strlen("\x01"),fp);
			::fwrite(iter->second,1,iter->second.GetLength(),fp);
			::fwrite("\r\n",1,strlen("\r\n"),fp);
		}
		::fclose(fp);
	}

}

void NatAuth::LoadMap(const char *filepath)
{
	FILE *fp = ::fopen(filepath,"rb");
	if (fp)
	{
		while ( !feof(fp))
		{
			char tmp[512] = {0};
			::fgets(tmp,512,fp);
			
			CString line(tmp);
			line.Replace("\r","");
			line.Replace("\n","");

			int pos = line.Find("\x01");
			if (pos != -1)
			{
				CString tmp1 = line.Mid(0, pos);
				CString tmp2 = line.Mid(pos + strlen("\x01"));
				mac_auth_.insert(make_pair(tmp1,tmp2));
			}
			
			/*
			string::size_type pos = line.find("\t");
			if (pos != string::npos)
			{
				string mac = line.substr(0,pos);
				int m = atoi(mac.c_str());
				string auth = line.substr(pos + 1);
				mac_auth_.insert(make_pair(m,auth));
			}*/
			
		}		
		::fclose(fp);
	} 
}