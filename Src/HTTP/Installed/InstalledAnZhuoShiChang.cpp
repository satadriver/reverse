#include "StdAfx.h"
#include "InstalledAnZhuoShiChang.h"

installed::AnZhuoShiChang::AnZhuoShiChang(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "��׿�г�";
}

installed::AnZhuoShiChang::~AnZhuoShiChang()
{
	
}

int installed::AnZhuoShiChang::IsAnZhuoShiChang(HttpSession *http_session)
{
	/************************************************************************/
	/* Host: fabu.baidu.com		POST /tipappcheck?uid=08SAiYu6B8Y6Ovuq_aBguYiXv8_javiT �Ӱ�׿�г����ذ�װ��ʱ���ץ�İ� ��װ���Ժ�õ�һ��*/
	/************************************************************************/
	if (http_session->m_Requestline.m_Host.Find("fabu.baidu.com") != -1 && http_session->m_Requestline.m_URI.Find("/tipappcheck?uid") != -1)  
	{
		//ѹ����
		return kSoftwareUpdate2;
	} 
	return 0;
}

int installed::AnZhuoShiChang::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate2)
	{
		return SoftwareUpdate2(packet);
	}
	return 0;
}

//����ѹ����
int installed::AnZhuoShiChang::SoftwareUpdate2(const char *packet)
{	
	//��ѹ 2
	string src_data;
	unsigned long out_length = 0;
	unsigned char* data_buff = NULL;
	vector<Byte *> data_vec;
	//��ѹ����
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
	//ת��
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	delete []data_buff;
	data_buff = NULL;
	//�ж��Ƿ�Ϊ��
	if (src_data.empty())
	{
		return 0;
	}	
	int retval = 0;	
	//��Ų�ֳ�����С��Ԫ����
	CStringArray strArray;
	strArray.RemoveAll();
    //ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		for (int j = 0;j <value.size();j++)
		{
			Json::Value arrayObj = value[j];
			int k = 4; //0 ��λ���ǰ��� 4 ��λ���Ǻ�������
			string out = arrayObj[k].asString();
			strArray.Add(out.c_str());
		}
	}
	//ȡ������
	int nArrayCount = strArray.GetSize();
	for (int k = 0;k <nArrayCount;k++)
	{
		string installed_ = strArray[k].GetBuffer(0);
		if (!installed_.empty())
		{
			//installed_ = GetAppName(installed_);
			if (installed_.size() < 50)
			{
				output_text_ =
					"webname=" + installed_ + "\r\n" +
					"type=�ֻ�\r\n" +
					"msg=" + app_ + "\r\n" +
					"state=��װ\r\n";
				WriteIndexFile();
				retval = 1;
			}	
		}		
	}
	return retval;
}

