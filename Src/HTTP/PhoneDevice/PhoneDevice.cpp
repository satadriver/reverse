#include "StdAfx.h"
#include "PhoneDevice.h"
#include "PhoneDeviceXinLangWeiBo.h"
#include "PhoneDeviceMoJiTianQi.h"
#include "PhoneDeviceJinShanDianYiSheng.h"
#include "PhoneDeviceDaZhongDianPing.h"
#include "PhoneDeviceWangYiXinWen.h"
#include "PhoneDeviceMeiTuXiuXiu.h"
#include "PhoneDeviceAiQiYi.h"
#include "PhoneDeviceBaiDuTieBa.h"
#include "PhoneDeviceBaoFengYingYin.h"
#include "PhoneDeviceFengHuangXinWen.h"
#include "PhoneDeviceZhiFuBaoQianBao.h"
#include "PhoneDeviceYouKu.h"
#include "PhoneDeviceTengXunShiPin.h"
#include "PhoneDeviceTaoBao.h"
#include "PhoneDeviceLofter.h"
#include "PhoneDeviceMobike.h"
#include "PhoneDevice2345ZhuShou.h"
#include "PhoneDevicePPZhuShou.h"
#include "PhoneDeviceHiCloud.h"
#include "PhoneDevice360zhushou.h"
#include "PhoneDeviceAnZhuoShiChang.h"
#include "PhoneDeviceWanDouJia.h"
#include "PhoneDeviceYingYongBao.h"
#include "PhoneDeviceBiLin.h"
#include "PhoneDeviceChengPinKuaiPai.h"
#include "PhoneDeviceDouGuoMeiShi.h"
#include "PhoneDeviceZte.h"
#include "PhoneDeviceMiGu.h"
#include "PhoneDevice58TongCheng.h"
#include "PhoneDeviceLianJia.h"
#include "PhoneDeviceYiRenDai.h"
#include "PhoneDeviceTouTiao.h"
#include "PhoneDeviceSuiEXingWiFi.h"
#include "PhoneDeviceHaoMaBaiShiTong.h"
#include "PhoneDeviceQuanMinKGe.h"
#include "PhoneDeviceTongHuaShun.h"
#include "PhoneDeviceZhe800.h"
#include "PhoneDeviceWIFIWangNengYaoShi.h"
#include "PhoneDeviceYueDongQuan.h"
#include "PhoneDeviceYouDaoCiDian.h"
#include "PhoneDeviceZhuanZhuan.h"
#include "PhoneDeviceJingDong.h"
#include "PhoneDeviceKuaiShou.h"

#include "PhoneDeviceOTHER.h"

phonedevice::PhoneDevice::PhoneDevice(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

phonedevice::PhoneDevice::~PhoneDevice()
{
	
}

string phonedevice::PhoneDevice::GetOS()
{
	string tmp = ms_->ToLower(model_);
	
	if ( (tmp.find("nokia") != string::npos ) || ( tmp.find("lumia") != string::npos))
	{
		return "Windows Phone";
	}

	if ( tmp.find("iphone") != string::npos)
	{
		return "iOS";
	}

	if (version_.size() > 1)
	{
		if ( ('2' <= version_[0]) && (version_[0] <= '5'))
		{
			return "Android";
		}
		
		if ( ('6' <= version_[0]) && (version_[0] <= '9'))
		{
			return "iOS";
		}
	}
	return "";
}

string phonedevice::PhoneDevice::GetOutputText()
{
	return 
		"from=" + 	  	longitude_ + "\r\n" + 	//经度 	 	
		"to=" + 	  	latitude_ + "\r\n" + 	//纬度 	 
		"servername=" + os_ + "\r\n" + 			//安装系统 	(Android IOS Lumia)
		"appname=" + 	version_ + "\r\n" + 	//系统版本 	(4.4.4)	
		"imsi=" + 	 	imsi_ + "\r\n" + 		//IMSI
		"fileurl=" + 	imei_+ "\r\n" + 		//IMEI    	
		"webname=" + 	model_ + "\r\n" + 		//手机型号  
		"state=" + 	  	app_ + "\r\n";			//软件
}

int phonedevice::PhoneDevice::ProcessPhoneDevice(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	/*
	action = phonedevice::XinLangWeiBo::IsXinLangWeiBo(http_session);
	if (action)
	{
		XinLangWeiBo xlwb(http_session,"PHONE_DEVICE");
		return xlwb.Process(packet,action);
	}
*/
	//墨迹天气
	action = phonedevice::MoJiTianQi::IsMoJiTianQi(http_session);
	if (action)
	{
		MoJiTianQi mjtq(http_session,"PHONE_DEVICE");
		return mjtq.Process(packet,action);
	}
	//金山电池医生
	action = phonedevice::JinShanDianChiYiSheng::IsJinShanDianChiYiSheng(http_session);
	if (action)
	{
		JinShanDianChiYiSheng jsdcys(http_session,"PHONE_DEVICE");
		return jsdcys.Process(packet,action);
	}
    //大众点评
	action = phonedevice::DaZhongDianPing::IsDaZhongDianPing(http_session);
	if (action)
	{
		DaZhongDianPing dzdp(http_session,"PHONE_DEVICE");
		return dzdp.Process(packet,action);
	}
    //网易新闻
	action = phonedevice::WangYiXinWen::IsWangYiXinWen(http_session);
	if (action)
	{
		WangYiXinWen wyxw(http_session,"PHONE_DEVICE");
		return wyxw.Process(packet,action);
	}
    //美图秀秀
	action = phonedevice::MeiTuXiuXiu::IsMeiTuXiuXiu(http_session);
	if (action)
	{
		MeiTuXiuXiu mtxx(http_session,"PHONE_DEVICE");
		return mtxx.Process(packet,action);
	}
	//爱奇艺
	action = phonedevice::AiQiYi::IsAiQiYi(http_session);
	if (action)
	{
		AiQiYi aqy(http_session,"PHONE_DEVICE");
		return aqy.Process(packet,action);
	}
    //百度贴吧
	action = phonedevice::BaiDuTieBa::IsBaiDuTieBa(http_session);
	if (action)
	{
		BaiDuTieBa bdtb(http_session,"PHONE_DEVICE");
		return bdtb.Process(packet,action);
	}
    //凤凰新闻
	action = phonedevice::FengHuangXinWen::IsFengHuangXinWen(http_session);
	if (action)
	{
		FengHuangXinWen fhxw(http_session,"PHONE_DEVICE");
		return fhxw.Process(packet,action);
	}
	//支付宝钱包
	action = phonedevice::ZhiFuBaoQianBao::IsZhiFuBaoQianBao(http_session);
	if (action)
	{
		ZhiFuBaoQianBao zfbqb(http_session,"PHONE_DEVICE");
		return zfbqb.Process(packet,action);
	}
    //优酷
	action = phonedevice::YouKu::IsYouKu(http_session);
	if (action)
	{
		YouKu yk(http_session,"PHONE_DEVICE");
		return yk.Process(packet,action);
	}
    //腾讯视频
	action = phonedevice::TengXunShiPin::IsTengXunShiPin(http_session);
	if (action)
	{
		TengXunShiPin txsp(http_session,"PHONE_DEVICE");
		return txsp.Process(packet,action);
	}
    //淘宝
	action = phonedevice::TaoBao::IsTaoBao(http_session);
	if (action)
	{
		TaoBao tb(http_session,"PHONE_DEVICE");
		return tb.Process(packet,action);
	}
	//网易Lofter
	action = phonedevice::Lofter::IsLofter(http_session);
	if (action)
	{
		Lofter lofter(http_session,"PHONE_DEVICE");
		return lofter.Process(packet,action);
	}
	//摩拜单车
	action = phonedevice::Mobike::IsMobike(http_session);
	if (action)
	{
		Mobike mobike(http_session,"PHONE_DEVICE");
		return mobike.Process(packet,action);
	}
	//2345手机助手
	action = phonedevice::ZhuShou2345::IsZhuShou2345(http_session);
	if (action)
	{
		ZhuShou2345 zhushou2345(http_session,"PHONE_DEVICE");
		return zhushou2345.Process(packet,action);
	}
	//pp手机助手
	action = phonedevice::ZhuShouPP::IsZhuShouPP(http_session);
	if (action)
	{
		ZhuShouPP zhushoupp(http_session,"PHONE_DEVICE");
		return zhushoupp.Process(packet,action);
	}
	//华为应用市场
	action = phonedevice::HiCloud::IsHiCloud(http_session);
	if (action)
	{
		HiCloud hicloud(http_session,"PHONE_DEVICE");
		return hicloud.Process(packet,action);
	}
	//360手机助手
	action = phonedevice::SHOUJIZHUSHOU360::IsSHOUJIZHUSHOU360(http_session);
	if (action)
	{
		SHOUJIZHUSHOU360 shoujizhushou360(http_session,"PHONE_DEVICE");
		return shoujizhushou360.Process(packet,action);
	}
	//安卓市场
	action = phonedevice::AnZhuoShiChang::IsAnZhuoShiChang(http_session);
	if (action)
	{
		AnZhuoShiChang anzhuoshichang(http_session,"PHONE_DEVICE");
		return anzhuoshichang.Process(packet,action);
	}
	//豌豆荚
	action = phonedevice::WanDouJia::IsWanDouJia(http_session);
	if (action)
	{
		WanDouJia wandoujia(http_session,"PHONE_DEVICE");
		return wandoujia.Process(packet,action);
	}
	//应用宝
	action = phonedevice::YingYongBao::IsYingYongBao(http_session);
	if (action)
	{
		YingYongBao yingyongbao(http_session,"PHONE_DEVICE");
		return yingyongbao.Process(packet,action);
	}
	//比邻
	action = phonedevice::BiLin::IsBiLin(http_session);
	if (action)
	{
		BiLin bilin(http_session,"PHONE_DEVICE");
		return bilin.Process(packet,action);
	}
	//成品快拍
	action = phonedevice::ChengPinKuaiPai::IsChengPinKuaiPai(http_session);
	if (action)
	{
		ChengPinKuaiPai chengpinkuaipai(http_session,"PHONE_DEVICE");
		return chengpinkuaipai.Process(packet,action);
	}
	//豆果美食
	action = phonedevice::DouGuoMeiShi::IsDouGuoMeiShi(http_session);
	if (action)
	{
		DouGuoMeiShi douguomeishi(http_session,"PHONE_DEVICE");
		return douguomeishi.Process(packet,action);
	}
	//中兴应用商店
	action = phonedevice::PdZte::IsPdZte(http_session);
	if (action)
	{
		PdZte pdzte(http_session,"PHONE_DEVICE");
		return pdzte.Process(packet,action);
	}
	//咪咕音乐
	action = phonedevice::PdMiGu::IsPdMiGu(http_session);
	if (action)
	{
		PdMiGu pdmigu(http_session,"PHONE_DEVICE");
		return pdmigu.Process(packet,action);
	}
	//58同城
	action = phonedevice::Pd58TongCheng::IsPd58TongCheng(http_session);
	if (action)
	{
		Pd58TongCheng pd58tongcheng(http_session,"PHONE_DEVICE");
		return pd58tongcheng.Process(packet,action);
	}
	//链家地产
	action = phonedevice::PdLianJia::IsPdLianJia(http_session);
	if (action)
	{
		PdLianJia pdlianjia(http_session,"PHONE_DEVICE");
		return pdlianjia.Process(packet,action);
	}
	//宜人贷
	action = phonedevice::PdYiRenDai::IsPdYiRenDai(http_session);
	if (action)
	{
		PdYiRenDai pdyirendai(http_session,"PHONE_DEVICE");
		return pdyirendai.Process(packet,action);
	}
	//今日头条
	action = phonedevice::PdTouTiao::IsPdTouTiao(http_session);
	if (action)
	{
		PdTouTiao pdtoutiao(http_session,"PHONE_DEVICE");
		return pdtoutiao.Process(packet,action);
	}
	//随e行
	action = phonedevice::SuiEXingWiFi::IsSuiEXingWiFi(http_session);
	if (action)
	{
		SuiEXingWiFi suiyixing(http_session,"PHONE_DEVICE");
		return suiyixing.Process(packet,action);
	}
	//号码百事通
	action = phonedevice::HaoMaBaiShiTong::IsHaoMaBaiShiTong(http_session);
	if (action)
	{
		HaoMaBaiShiTong haomabaishitong(http_session,"PHONE_DEVICE");
		return haomabaishitong.Process(packet,action);
	}
	//全民k歌
	action = phonedevice::QuanMinKGe::IsQuanMinKGe(http_session);
	if (action)
	{
		QuanMinKGe quanminkge(http_session,"PHONE_DEVICE");
		return quanminkge.Process(packet,action);
	}
	//同花顺
	action = phonedevice::TongHuaShun::IsTongHuaShun(http_session);
	if (action)
	{
		TongHuaShun tonghuashun(http_session,"PHONE_DEVICE");
		return tonghuashun.Process(packet,action);
	}
	//折800
	action = phonedevice::Zhe800::IsZhe800(http_session);
	if (action)
	{
		Zhe800 zhe800(http_session,"PHONE_DEVICE");
		return zhe800.Process(packet,action);
	}
	//wifi万能钥匙
	action = phonedevice::WiFiWangNengYaoShi::IsWiFiWangNengYaoShi(http_session);
	if (action)
	{
		WiFiWangNengYaoShi wifiwannengyaoshi(http_session,"PHONE_DEVICE");
		return wifiwannengyaoshi.Process(packet,action);
	}
	//悦动圈
	action = phonedevice::YueDongQuan::IsYueDongQuan(http_session);
	if (action)
	{
		YueDongQuan yuedongquan(http_session,"PHONE_DEVICE");
		return yuedongquan.Process(packet,action);
	}
	//有道词典
	action = phonedevice::YouDaoCiDian::IsYouDaoCiDian(http_session);
	if (action)
	{
		YouDaoCiDian youdaocidian(http_session,"PHONE_DEVICE");
		return youdaocidian.Process(packet,action);
	}
	//转转
	action = phonedevice::PdZhuanZhuan::IsPdZhuanZhuan(http_session);
	if (action)
	{
		PdZhuanZhuan pdzhuanzhuan(http_session,"PHONE_DEVICE");
		return pdzhuanzhuan.Process(packet,action);
	}
	//京东
	action = phonedevice::PdJingDong::IsPdJingDong(http_session);
	if (action)
	{
		PdJingDong pdjingdong(http_session,"PHONE_DEVICE");
		return pdjingdong.Process(packet,action);
	}
	//快手
	action = phonedevice::PdKuaiShou::IsPdKuaiShou(http_session);
	if (action)
	{
		PdKuaiShou pdkuaishou(http_session,"PHONE_DEVICE");
		return pdkuaishou.Process(packet,action);
	}
	/*
	action = phonedevice::BaoFengYingYin::IsBaoFengYingYin(http_session);
	if (action)
	{
		BaoFengYingYin baofeng(http_session,"PHONE_DEVICE");
		return baofeng.Process(packet,action);
	}
	*/
	/*
	action = phonedevice::Other::IsOther(http_session);
	if (action)
	{
		Other o(http_session,"PHONE_DEVICE");
		return o.Process(packet,action);
	}*/
	return 0;
}