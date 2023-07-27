#include "StdAfx.h"
#include "Installed.h"
#include "Installed360AnQuanWeiShi.h"
#include "Installed360ShouJiAnQuanWeiShi.h"
#include "Installed91ZhuShou.h"
#include "InstalledSouGouZhuShou.h"
#include "InstalledWanDouJia.h"
#include "InstalledXiaoMiShangDian.h"
#include "InstalledMeiZuYingYongShangDian.h"
#include "InstalledKuaiYong.h"
#include "InstalledShouJiZhuShou360.h"
#include "InstalledLianXiangShangDian.h"
#include "InstalledALiYun.h"
#include "InstalledYingYongBao.h"
#include "InstalledAnZhuoShiChang.h"
#include "InstalledBaiDuShouJiZhuShou.h"
#include "InstalledZhongXingShangDian.h"
#include "InstalledVivoShangDian.h"
#include "InstalledOppoShangDian.h"
#include "InstalledPPzhushou.h"
#include "InstalledHiCloud.h"
#include "InstalledMmShangDian.h"
#include "InstalledDouGuoMeiShi.h"
#include "InstalledAFanTi.h"

map<string,string>installed::Installed::apk_app_ = map<string,string>();

installed::Installed::Installed(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

installed::Installed::~Installed()
{
	
}

void installed::Installed::InitApkAppMap()
{
	FILE *f = fopen("e:\\netspy\\para\\applist.txt","r");
	if (!f)
	{
		return ;
	}
	char applist[10000] = {0};
	int len = fread(&applist,1,10000,f);

	fclose(f);
	string data(applist);

	StringReplace(data,"\r","&");
	StringReplace(data,"\n","&");

	const char sep_item = '&';
	const char sep_value = '=';

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = data.find(sep_item,item_beg_pos);
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
			item_beg_pos = item_end_pos + sizeof(sep_item);
		}
		
		size_t value_pos = item.find(sep_value);
		if (value_pos == string::npos)
		{
			continue;
		}
		string key(item.substr(0,value_pos));
		string val(item.substr(value_pos + 1));
		apk_app_.insert(std::make_pair(key,val));
	}
}

void installed::Installed::InitUselessAPK()
{
	FILE *f = fopen("e:\\netspy\\para\\reverse\\userless_apk.txt","r");
	if (!f)
	{
		return ;
	}
	char userless_apk[10000] = {0};
	int len = fread(&userless_apk,1,10000,f);
	
	fclose(f);
	string data(userless_apk);
	
	StringReplace(data,"\r",";");
	StringReplace(data,"\n",";");
	
	const char sep_item = ';';
	const char sep_value = '=';
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = data.find(sep_item,item_beg_pos);
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
			item_beg_pos = item_end_pos + sizeof(sep_item);
		}
		
		size_t value_pos = item.find(sep_value);
		if (value_pos == string::npos)
		{
			continue;
		}
		string key(item.substr(0,value_pos));
		string val(item.substr(value_pos + 1));
		apk_app_.insert(std::make_pair(key,val));
	}
	
}

void installed::Installed::StringReplace(string &src_string,const string &old_string ,const string &new_string)
{
	string::size_type pos = 0;
	string::size_type old_len = old_string.size();
	string::size_type new_len = new_string.size();
	
	while( (pos = src_string.find(old_string,pos)) != string::npos )
	{
		src_string.replace(pos,old_len,new_string);
		pos += new_len;
	}
}


int installed::Installed::ProcessInstalled(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
    //360安全卫士
	action = installed::AnQuanWeiShi360::IsAnQuanWeiShi360(http_session);
	if (action)
	{
		AnQuanWeiShi360 aqws360(http_session,"software_installed");
		return aqws360.Process(packet,action);
	}
    //360手机安全卫士
	action = installed::ShouJiAnQuanWeiShi360::IsShouJiAnQuanWeiShi360(http_session);
	if (action)
	{
		ShouJiAnQuanWeiShi360 sjaqws360(http_session,"software_installed");
		return sjaqws360.Process(packet,action);
	}
	//91助手
	action = installed::ZhuShou91::IsZhuShou91(http_session);
	if (action)
	{
		ZhuShou91 zhushou91(http_session,"software_installed");
		return zhushou91.Process(packet,action);
	}
	//搜狗手机助手
	action = installed::SouGouZhuShou::IsSouGouZhuShou(http_session);
	if (action)
	{
		SouGouZhuShou sgzs(http_session,"software_installed");
		return sgzs.Process(packet,action);
	}
    //豌豆荚
	action = installed::WanDouJia::IsWanDouJia(http_session);
	if (action)
	{
		WanDouJia wandoujia(http_session,"software_installed");
		return wandoujia.Process(packet,action);
	}
    //魅族商店
	action = installed::MeiZuYingYongShangDian::IsMeiZuYingYongShangDian(http_session);
	if (action)
	{
		MeiZuYingYongShangDian mzyysd(http_session,"software_installed");
		return mzyysd.Process(packet,action);
	}
    //小米商店
	action = installed::XiaoMiShangDian::IsXiaoMiShangDian(http_session);
	if (action)
	{
		XiaoMiShangDian xmsd(http_session,"software_installed");
		return xmsd.Process(packet,action);
	}
	//快用
	action = installed::KuaiYong::IsKuaiYong(http_session);
	if (action)
	{
		KuaiYong kuaiyong(http_session,"software_installed");
		return kuaiyong.Process(packet,action);
	}
	//360 手机助手
	action = installed::ShouJiZhuShou360::IsShouJiZhuShou360(http_session);
	if (action)
	{
		ShouJiZhuShou360 sjzs(http_session,"software_installed");
		return sjzs.Process(packet,action);
	}
	//联想商店
	action = installed::LianXiangShangDian::IsLianXiangShangDian(http_session);
	if (action)
	{
		LianXiangShangDian lxsd(http_session,"software_installed");
		return lxsd.Process(packet,action);
	}
	//阿里云
	action = installed::ALiYun::IsALiYun(http_session);
	if (action)
	{
		ALiYun aly(http_session,"software_installed");
		return aly.Process(packet,action);
	}
    //应用宝
	action = installed::YingYongBao::IsYingYongBao(http_session);
	if (action)
	{
		YingYongBao yyb(http_session,"software_installed");
		return yyb.Process(packet,action);
	}   
	//安卓市场	
	action = installed::AnZhuoShiChang::IsAnZhuoShiChang(http_session);
	if (action)
	{
		AnZhuoShiChang azsc(http_session,"software_installed");
		return azsc.Process(packet,action);
	}  
	//百度手机助手	
	action = installed::BaiDuShouJiZhuShou::IsBaiDuShouJiZhuShou(http_session);
	if (action)
	{
		BaiDuShouJiZhuShou bdsjzs(http_session,"software_installed");
		return bdsjzs.Process(packet,action);
	}  
	//中兴应用商店
	action = installed::ZhongXingShangDian::IsZhongXingShangDian(http_session);
	if (action)
	{
		ZhongXingShangDian zxyysd(http_session,"software_installed");
		return zxyysd.Process(packet,action);
	} 
	//vivo应用商店
	action = installed::VivoShangDian::IsVivoShangDian(http_session);
	if (action)
	{
		VivoShangDian vivoshangdian(http_session,"software_installed");
		return vivoshangdian.Process(packet,action);
	} 
	//oppo应用商店 (暂时无解-二进制流)
	action = installed::OppoShangDian::IsOppoShangDian(http_session);
	if (action)
	{
		OppoShangDian opposhangdian(http_session,"software_installed");
		return opposhangdian.Process(packet,action);
	} 
	//pp助手
	action = installed::PPzhushou::IsPPzhushou(http_session);
	if (action)
	{
		PPzhushou ppzhushou(http_session,"software_installed");
		return ppzhushou.Process(packet,action);
	} 
	//华为应用市场
	action = installed::HiCloud::IsHiCloud(http_session);
	if (action)
	{
		HiCloud hicloud(http_session,"software_installed");
		return hicloud.Process(packet,action);
	}
	//MM应用商场(中国移动)
	action = installed::MMShangDian::IsMMShangDian(http_session);
	if(action)
	{
		MMShangDian mmshangdian(http_session,"software_installed");
		return mmshangdian.Process(packet,action);
	}
	//豆果美食
	action = installed::DouGuoMeiShi::IsDouGuoMeiShi(http_session);
	if (action)
	{
		DouGuoMeiShi douguomeishi(http_session,"software_installed");
		return douguomeishi.Process(packet,action);
	}
	//阿凡提
	action = installed::AFanTi::IsAFanTi(http_session);
	if (action)
	{
		AFanTi afanti(http_session,"software_installed");
		return afanti.Process(packet,action);
	}
	return 0;
}

string installed::Installed::GetAppName(const string &apk)
{
	map<string,string>::iterator iter = apk_app_.begin();

	for (;iter != apk_app_.end(); ++ iter)
	{
		if (apk.find(iter->first) != string::npos)
		{
			return iter->second;
		}
	}
	return apk;
}