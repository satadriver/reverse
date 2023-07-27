
#include "StdAfx.h"
#include "Shopping.h"

#include "ShoppingBiShengKe.h"
#include "ShoppingDangDang.h"
#include "ShoppingFanKe.h"
#include "ShoppingGuoMei.h"
#include "ShoppingJingDong.h"
#include "ShoppingJuMeiYouPin.h"
#include "ShoppingKFC.h"
#include "ShoppingLeFeng.h"
#include "ShoppingNuoMi.h"
#include "ShoppingPaiPai.h"
#include "ShoppingSuNingYiGou.h"
#include "ShoppingTaoBao.h"
#include "ShoppingTMall.h"
#include "ShoppingTuan800.h"
#include "ShoppingWeiPinHui.h"
#include "ShoppingWoWoTuanGou.h"
#include "ShoppingYiHaoDian.h"
#include "ShoppingMoGuJie.h"
#include "ShoppingKaoLaHaiGou.h"
#include "ShoppingHanDuYiShe.h"
#include "ShoppingMaiLeGou.h"
#include "ShoppingMeiLiShuo.h"
#include "ShoppingMingXieKu.h"
#include "ShoppingYouGou.h"
#include "ShoppingMiYa.h"
#include "ShoppingZhe800.h"
#include "ShoppingLeTao.h"
#include "ShoppingZhenPin.h"
#include "ShoppingMoonBsSa.h"
#include "ShoppingHaiLanZhiJia.h"
#include "ShoppingYoHoBuy.h"
#include "ShoppingLaShouWang.h"
#include "ShoppingMiZhe.h"
#include "ShoppingMaiHaoHao.h"
#include "ShoppingKuaiLeGou.h"
#include "ShoppingHaoLeMai.h"
#include "ShoppingAiQiYiShangCheng.h"
#include "ShoppingYinYueShangCheng.h"

//容器
map<string,string>shopping::Shopping::addressCode_addressName_ = map<string,string>();

shopping::Shopping::Shopping(HttpSession *http_session, const string &pro_name):HttpPro(http_session, pro_name)
{
	
}

shopping::Shopping::~Shopping()
{
	
}

void shopping::Shopping::InitIdentityCardAddressCodeMap()
{
	//文件路径
	CString strPathName = "e:\\netspy\\para\\IdentityCardAddressCode.txt";
	CStdioFile file;
	//打开文件
    if(!file.Open(strPathName,CFile::modeRead))
	{
		::AfxMessageBox(_T("IdentityCardAddressCode文件打开失败!"));
		return;
	}
	//读文件
	CString strText = _T("");
	while(file.ReadString(strText))
	{
		string item(strText.GetBuffer(0));

		const char sep_value = ' ';	
		size_t value_pos = item.find(sep_value);
		if (value_pos == string::npos)
		{
			return;
		}
		string key(item.substr(0,value_pos));
		string val(item.substr(value_pos + 1));
		if (key.length() > 0 && val.length() > 0)
		{
			addressCode_addressName_.insert(std::make_pair(key,val));	
		}	
	}
// 	FILE *f = fopen("e:\\netspy\\para\\IdentityCardAddressCode.txt","r");
// 	if (!f)
// 	{
// 		return ;
// 	}
// 	char applist[100000] = {0};
// 	int len = fread(&applist,1,100000,f);
// 	
// 	fclose(f);
// 	string data(applist);
// 	
// 	StringReplace(data,"\r","&");
// 	StringReplace(data,"\n","&");
// 	
// 	const char sep_item = '&';
// 	const char sep_value = ' ';
// 	
// 	string::size_type item_beg_pos = 0;
// 	string::size_type item_end_pos = 0; 
// 	while(1)
// 	{
// 		item_end_pos = data.find(sep_item,item_beg_pos);
// 		string item;
// 		if(item_end_pos == string::npos)
// 		{
// 			if (item_beg_pos == string::npos)
// 			{
// 				break;
// 			}
// 			else
// 			{
// 				item = data.substr(item_beg_pos);
// 				item_beg_pos = string::npos;
// 			}
// 		}
// 		else
// 		{
// 			item = data.substr(item_beg_pos,item_end_pos - item_beg_pos);
// 			item_beg_pos = item_end_pos + sizeof(sep_item);
// 		}
// 		
// 		size_t value_pos = item.find(sep_value);
// 		if (value_pos == string::npos)
// 		{
// 			continue;
// 		}
// 		string key(item.substr(0,value_pos));
// 		string val(item.substr(value_pos + 1));
// 		addressCode_addressName_.insert(std::make_pair(key,val));
// 	}
}


void shopping::Shopping::StringReplace(string &src_string,const string &old_string ,const string &new_string)
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


string shopping::Shopping::GetAddress(const string &addressCode)
{
	map<string,string>::iterator iter = addressCode_addressName_.begin();
	
	for (;iter != addressCode_addressName_.end(); ++ iter)
	{
		if (addressCode.find(iter->first) != string::npos)
		{
			return iter->second;
		}
	}
	return addressCode;
}

int shopping::Shopping::ProcessShopping(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	int n = 0;
	try
	{
		//淘宝(https)
		action = TaoBao::IsTaoBao(http_session);
		if (action)
		{
			n = 1;
			TaoBao taobao(http_session,"shopping_taobao");
			return taobao.Process(packet,action);
		}
		//天猫(https无法解析)
		action = TMall::IsTMall(http_session);
		if (action)
		{
			n = 2;
			TMall tmall(http_session,"shopping_tmall");
			return tmall.Process(packet,action);
		}
		//京东商城
		action = JingDong::IsJingDong(http_session);
		if (action)
		{
			n = 3;
			JingDong jingdong(http_session,"shopping_jingdong");
			return jingdong.Process(packet,action);
		}
		//拍拍
		action = PaiPai::IsPaiPai(http_session);
		if (action)
		{
			n = 4;
			PaiPai paipai(http_session,"shopping_paipai");
			return paipai.Process(packet,action);
		}
		//国美电器(无解析)
		action = GuoMei::IsGuoMei(http_session);
		if (action)
		{
			n = 5;
			GuoMei guomei(http_session,"shopping_guomei");
			return guomei.Process(packet,action);
		}
		//当当网
		action = DangDang::IsDangDang(http_session);
		if (action)
		{
			n = 6;
			DangDang dangdang(http_session,"shopping_dangdang");
			return dangdang.Process(packet,action);
		}
		//凡客
		action = FanKe::IsFanKe(http_session);
		if (action)
		{
			n = 7;
			FanKe fanke(http_session,"shopping_fanke");
			return fanke.Process(packet,action);
		}
		//团800
		action = Tuan800::IsTuan800(http_session);
		if (action)
		{
			n = 8;
			Tuan800 tuan800(http_session,"shopping_tuan800");
			return tuan800.Process(packet,action);
		}
		//聚美优品
		action = JuMeiYouPin::IsJuMeiYouPin(http_session);
		if (action)
		{
			n = 9;
			JuMeiYouPin jumeiyoupin(http_session,"shopping_jumeiyoupin");
			return jumeiyoupin.Process(packet,action);
		}
		//乐峰
		action = LeFeng::IsLeFeng(http_session);
		if (action)
		{
			n = 10;
			LeFeng lefeng(http_session,"shopping_lefeng");
			return lefeng.Process(packet,action);
		}
		//糯米
		action = NuoMi::IsNuoMi(http_session);
		if (action)
		{
			n = 11;
			NuoMi nuomi(http_session,"shopping_nuomi");
			return nuomi.Process(packet,action);
		}
		//苏宁电器	
		action = SuNingYiGou::IsSuNingYiGou(http_session);
		if (action)
		{
			n = 12;
			SuNingYiGou suningyigou(http_session,"shopping_suningyigou");
			return suningyigou.Process(packet,action);
		}
		//唯品会
		action = WeiPinHui::IsWeiPinHui(http_session);
		if (action)
		{
			n = 13;
			WeiPinHui weipinhui(http_session,"shopping_weipinhui");
			return weipinhui.Process(packet,action);
		}
		//喔喔团购
		action = WoWoTuanGou::IsWoWoTuanGou(http_session);
		if (action)
		{
			n = 14;
			WoWoTuanGou wowotuan(http_session,"shopping_wowotuangou");
			return wowotuan.Process(packet,action);
		}
		//一号店
		action = YiHaoDian::IsYiHaoDian(http_session);
		if (action)
		{
			n = 15;
			YiHaoDian yihaodian(http_session,"shopping_yihaodian");
			return yihaodian.Process(packet,action);
		}
		//KFC	
		action = KFC::IsKFC(http_session);
		if (action)
		{
			n = 16;
			KFC kfc(http_session,"shopping_kfc");
			return kfc.Process(packet,action);
		}
		//必胜客
		action = BiShengKe::IsBiShengKe(http_session);
		if (action)
		{
			n = 17;
			BiShengKe bishengke(http_session,"shopping_bishengke");
			return bishengke.Process(packet,action);
		}
		
		/*
		action = SuNingYiGou::IsSuNingYiGou(http_session);
		if (action)
		{
			n = 18;
			SuNingYiGou suningyigou(http_session,"SUNINGYIGOU");
			return suningyigou.Process(packet,action);
		}
		*/   
		//蘑菇街
		action = MoGuJie::IsMoGuJie(http_session);
		if (action)
		{
			MoGuJie mogujie(http_session,"shopping_mogujie");
			return mogujie.Process(packet,action);
		}
		//网易考拉海购
		action = KaoLaHaiGou::IsKaoLaHaiGou(http_session);
		if (action)
		{
			KaoLaHaiGou kaolahaigou(http_session,"shopping_kaolahaigou");
			return kaolahaigou.Process(packet,action);
		}
		//麦乐购
		action = MaiLeGOu::IsMaiLeGOu(http_session);
		if (action)
		{
			MaiLeGOu mailegou(http_session,"shopping_mailegou");
			return mailegou.Process(packet,action);
		}
		//优购网
		action = YouGou::IsYouGou(http_session);
		if (action)
		{
			n = 21;
			YouGou yougou(http_session,"shopping_yougou");
			return yougou.Process(packet,action);
		}
		//美丽说
		action = MeiLiShuo::IsMeiLiShuo(http_session);
		if (action)
		{
			MeiLiShuo meilishuo(http_session,"shopping_meilishuo");
			return meilishuo.Process(packet,action);
		}
		//名鞋库
		action = MingXieKu::IsMingXieKu(http_session);
		if (action)
		{
			MingXieKu mingxieku(http_session,"shopping_mingxieku");
			return mingxieku.Process(packet,action);
		}
		//韩都衣舍 24
		action = HanDUYiShe::IsHanDUYiShe(http_session);
		if (action)
		{
			HanDUYiShe handuyishe(http_session,"shopping_handuyishe");
			return handuyishe.Process(packet,action);
		}
		//蜜芽 25
		action = MiYa::IsMiYa(http_session);
		if (action)
		{
			MiYa miya(http_session,"shopping_miya");
			return miya.Process(packet,action);			
		}
		//折800 26
		action = Zhe800::IsZhe800(http_session);
		if (action)
		{
			Zhe800 zhe800(http_session,"shopping_zhe800");
			return zhe800.Process(packet,action);			
		}
		//乐淘 27
		action = LeTao::IsLeTao(http_session);
		if (action)
		{
			LeTao letao(http_session,"shopping_letao");
			return letao.Process(packet,action);			
		}
		//珍品 28
		action = ZhenPin::IsZhenPin(http_session);
		if (action)
		{
			ZhenPin zhenpin(http_session,"shopping_zhenpin");
			return zhenpin.Process(packet,action);			
		}
		//梦芭莎 29
		action = MoonBaSa::IsMoonBaSa(http_session);
		if (action)
		{
			MoonBaSa moonbasa(http_session,"shopping_moonbasa");
			return moonbasa.Process(packet,action);			
		}
		//海澜之家 30
		action = HaiLanZhiJia::IsHaiLanZhiJia(http_session);
		if (action)
		{
			HaiLanZhiJia hailanzhijia(http_session,"shopping_hailanzhijia");
			return hailanzhijia.Process(packet,action);			
		}
		//Yohu!Buy 31
		action = YoHoBuy::IsYoHoBuy(http_session);
		if (action)
		{
			YoHoBuy yohobuy(http_session,"shopping_yohobuy");
			return yohobuy.Process(packet,action);			
		}
		//拉手网 32
		action = LaShouWang::IsLaShouWang(http_session);
		if (action)
		{
			LaShouWang lashouwang(http_session,"shopping_lashouwang");
			return lashouwang.Process(packet,action);			
		}
		//米折 33
		action = MiZhe::IsMiZhe(http_session);
		if (action)
		{
			MiZhe mizhe(http_session,"shopping_mizhe");
			return mizhe.Process(packet,action);			
		}
		//卖好好 34
		action = MaiHaoHao::IsMaiHaoHao(http_session);
		if (action)
		{
			MaiHaoHao maihaohao(http_session,"shopping_maihaohao");
			return maihaohao.Process(packet,action);			
		}
		//快乐购 35
		action = KuaiLeGou::IsKuaiLeGou(http_session);
		if (action)
		{
			KuaiLeGou kuailegou(http_session,"shopping_kuailegou");
			return kuailegou.Process(packet,action);			
		}
		//好乐买 36
		action = HaoLeMai::IsHaoLeMai(http_session);
		if (action)
		{
			HaoLeMai haolemai(http_session,"shopping_haolemai");
			return haolemai.Process(packet,action);			
		}
		//爱奇艺商城 37
		action = AiQiYiShangCheng::IsAiQiYiShangCheng(http_session);
		if (action)
		{
			AiQiYiShangCheng aiqiyishangcheng(http_session,"shopping_aiqiyishangcheng");
			return aiqiyishangcheng.Process(packet,action);			
		}
		//音悦商城 38
		action = YinYueShangCheng::IsYinYueShangCheng(http_session);
		if (action)
		{
			YinYueShangCheng yinyueshangcheng(http_session,"shopping_yinyueshangcheng");
			return yinyueshangcheng.Process(packet,action);			
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessShopping Err : %d",n);
	}
	return 0;
}

