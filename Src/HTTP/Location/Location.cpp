
#include "StdAfx.h"

#include "Location.h"

#include "LocationAiLiao.h"
#include "LocationBaiHe.h"
#include "LocationMeiTuan.h"
#include "LocationWangXin.h"
#include "LocationWoChaCha.h"
#include "LocationMeiTuan.h"
#include "LocationGanJi.h"
#include "LocationAiBa.h"
#include "LocationBaiDuTuanGou.h"
#include "LocationQiuShiBaiKe.h"
#include "LocationKuaiLeBeiYun.h"
#include "LocationMaMaBang.h"
#include "LocationPeiNiKanDianYing.h"
#include "LocationQingTingFM.h"
#include "LocationLeDongLi.h"
#include "LocationTaoBao.h"
#include "LocationBaiXingWang.h"
#include "LocationCheLunChaWeiZhang.h"
#include "LocationRenRenWang.h"
#include "LocationWeiCheWeiZhang.h"
#include "LocationWeiDaiJia.h"
#include "LocationWIFIBanLv.h"
#include "LocationWIFIWanNengYaoChi.h"
#include "LocationAiChengShi.h"
#include "LocationDingDingYouHui.h"
#include "LocationMoXiuZhuoMian.h"
#include "LocationSouHuGouFangZhuShou.h"
#include "LocationSuiEXingWLAN.h"
#include "Location365RiLi.h"
#include "LocationBlued.h"
#include "LocationGOZhuoMian.h"
#include "LocationBaiDuWangYeDingWei.h"
#include "LocationChangWuXian.h"
#include "LocationJingDianTeJia.h"
#include "LocationKaiXinWang.h"
#include "LocationKuGou.h"
#include "LocationLaMaBang.h"
#include "LocationMiLiao.h"
#include "LocationMianBaoLvXing.h"
#include "LocationNuoMi.h"
#include "LocationQianChengWuYou.h"
#include "LocationQuNaLvXing.h"
#include "LocationRenMaiTong.h"
#include "LocationShengHuoRiLi.h"
#include "LocationMoJiTianQi.h"
#include "LocationLingShengDuoDuo.h"
#include "LocationMoMo.h"
#include "LocationDaZhongDianPing.h"
#include "Location58TongCheng.h"
#include "LocationLvMaMaLvYou.h"
#include "LocationLofter.h"
#include "LocationZaker.h"
#include "LocationCtrip.h"

map<string,int>location::Location::app_statistics_ = map<string,int>();
int location::Location::app_total_ = 0;

location::Location::Location(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

location::Location::~Location()
{
	
}

void location::Location::Record(const string &app)
{
/*
	if (app_total_ > 9999)
	{
		WriteAppStatistics();
		app_statistics_.clear();
		app_total_ = 0;
	}

	map<string, int>::iterator iter = app_statistics_.find(app);
	if (iter == app_statistics_.end())
	{
		app_statistics_.insert(std::make_pair(app,1));
	}
	else
	{
		iter->second++;
	}

	app_total_++;
	*/
}


void location::Location::WriteAppStatistics()
{

}

int location::Location::ProcessLocation(HttpSession *http_session ,const char *packet)
{
	int action = -1;

	action = AiLiao::IsAiLiao(http_session);
	if (action)
	{
		AiLiao pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
	/*
	action = BaiHe::IsBaiHe(http_session);
	if (action)
	{
		BaiHe pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
	*/
	//美团
	action = MeiTuan::IsMeiTuan(http_session);
	if (action)
	{
		MeiTuan pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = WangXin::IsWangXin(http_session);
	if (action)
	{
		WangXin pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = WoChaCha::IsWoChaCha(http_session);
	if (action)
	{
		WoChaCha pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = GanJi::IsGanJi(http_session);
	if (action)
	{
		GanJi pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	/*
	action = AiBa::IsAiBa(http_session);
	if (action)
	{
		AiBa pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

  */
	/*
	action = BaiDuTuanGou::IsBaiDuTuanGou(http_session);
	if (action)
	{
		BaiDuTuanGou pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
*/
	action = QiuShiBaiKe::IsQiuShiBaiKe(http_session);
	if (action)
	{
		QiuShiBaiKe pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = KuaiLeBeiYun::IsKuaiLeBeiYun(http_session);
	if (action)
	{
		KuaiLeBeiYun pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = MaMaBang::IsMaMaBang(http_session);
	if (action)
	{
		MaMaBang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = PeiNiKanDianYing::IsPeiNiKanDianYing(http_session);
	if (action)
	{
		PeiNiKanDianYing pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = QingTingFM::IsQingTingFM(http_session);
	if (action)
	{
		QingTingFM pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = LeDongLi::IsLeDongLi(http_session);
	if (action)
	{
		LeDongLi pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
	/*
	action = TaoBao::IsTaoBao(http_session);
	if (action)
	{
		TaoBao pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
	*/
	action = BaiXingWang::IsBaiXingWang(http_session);
	if (action)
	{
		BaiXingWang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = CheLunChaWeiZhang::IsCheLunChaWeiZhang(http_session);
	if (action)
	{
		CheLunChaWeiZhang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	/*
	action = RenRenWang::IsRenRenWang(http_session);
	if (action)
	{
		RenRenWang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
*/
	/*
	action = WeiCheWeiZhang::IsWeiCheWeiZhang(http_session);
	if (action)
	{
		WeiCheWeiZhang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
*/
	action = WeiDaiJia::IsWeiDaiJia(http_session);
	if (action)
	{
		WeiDaiJia pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = WIFIBanLv::IsWIFIBanLv(http_session);
	if (action)
	{
		WIFIBanLv pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = WIFIWanNengYaoChi::IsWIFIWanNengYaoChi(http_session);
	if (action)
	{
		WIFIWanNengYaoChi pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
/*
	action = AiChengShi::IsAiChengShi(http_session);
	if (action)
	{
		AiChengShi pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
*/
	action = DingDingYouHui::IsDingDingYouHui(http_session);
	if (action)
	{
		DingDingYouHui pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = MoXiuZhuoMian::IsMoXiuZhuoMian(http_session);
	if (action)
	{
		MoXiuZhuoMian pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = SouHuGouFangZhuShou::IsSouHuGouFangZhuShou(http_session);
	if (action)
	{
		SouHuGouFangZhuShou pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = SuiEXingWLAN::IsSuiEXingWLAN(http_session);
	if (action)
	{
		SuiEXingWLAN pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = RiLi365::IsRiLi365(http_session);
	if (action)
	{
		RiLi365 pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = Blued::IsBlued(http_session);
	if (action)
	{
		Blued pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = GOZhuoMian::IsGOZhuoMian(http_session);
	if (action)
	{
		GOZhuoMian pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = BaiDuWangYeDingWei::IsBaiDuWangYeDingWei(http_session);
	if (action)
	{
		BaiDuWangYeDingWei pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = ChangWuXian::IsChangWuXian(http_session);
	if (action)
	{
		ChangWuXian pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = JingDianTeJia::IsJingDianTeJia(http_session);
	if (action)
	{
		JingDianTeJia pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
/*
	action = KaiXinWang::IsKaiXinWang(http_session);
	if (action)
	{
		KaiXinWang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
*/
	action = KuGou::IsKuGou(http_session);
	if (action)
	{
		KuGou pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = LaMaBang::IsLaMaBang(http_session);
	if (action)
	{
		LaMaBang pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = MiLiao::IsMiLiao(http_session);
	if (action)
	{
		MiLiao pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = MianBaoLvXing::IsMianBaoLvXing(http_session);
	if (action)
	{
		MianBaoLvXing pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = NuoMi::IsNuoMi(http_session);
	if (action)
	{
		NuoMi pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = QianChengWuYou::IsQianChengWuYou(http_session);
	if (action)
	{
		QianChengWuYou pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = QuNaLvXing::IsQuNaLvXing(http_session);
	if (action)
	{
		QuNaLvXing pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	action = RenMaiTong::IsRenMaiTong(http_session);
	if (action)
	{
		RenMaiTong pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}

	/*
	action = ShengHuoRiLi::IsShengHuoRiLi(http_session);
	if (action)
	{
		ShengHuoRiLi pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
*/
	action = MoJiTianQi::IsMoJiTianQi(http_session);
	if (action)
	{
		MoJiTianQi mjtq(http_session,"PHONE_LOCATION");
		return mjtq.Process(packet,action);
	}

	action = LingShengDuoDuo::IsLingShengDuoDuo(http_session);
	if (action)
	{
		LingShengDuoDuo lsdd(http_session,"PHONE_LOCATION");
		return lsdd.Process(packet,action);
	}

	action = MoMo::IsMoMo(http_session);
	if (action)
	{
		MoMo momo(http_session,"PHONE_LOCATION");
		return momo.Process(packet,action);
	}

	action = DaZhongDianPing::IsDaZhongDianPing(http_session);
	if (action)
	{
		DaZhongDianPing pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
    //58同城
	action = TongCheng58::IsTongCheng58(http_session);
	if (action)
	{
		TongCheng58 pl(http_session,"PHONE_LOCATION");
		return pl.Process(packet,action);
	}
	//驴妈妈旅游
	action = LvMaMaLvYou::IsLvMaMaLvYou(http_session);
	if (action)
	{
		LvMaMaLvYou lvmamalvyou(http_session,"PHONE_LOCATION");
		return lvmamalvyou.Process(packet,action);
	}
	//网易Lofter
	action = Lofter::IsLofter(http_session);
	if (action)
	{
		Lofter lofter(http_session,"PHONE_LOCATION");
		return lofter.Process(packet,action);
	}
	//Zaker
	action = Zaker::IsZaker(http_session);
	if (action)
	{
		Zaker zaker(http_session,"PHONE_LOCATION");
		return zaker.Process(packet,action);
	}
	//携程
	action = LocationCtrip::IsLocationCtrip(http_session);
	if (action)
	{
		LocationCtrip locationctrip(http_session,"PHONE_LOCATION");
		return locationctrip.Process(packet,action);
	}
	return 0;
}

string location::Location::GetIpAgent()
{
	string sip = http_session_->m_sip.GetBuffer(0);
	string dip = http_session_->m_dip.GetBuffer(0);
	string agent(http_session_->m_Requestline.m_useragent.GetBuffer(0));
	return sip + "_" + dip + "_" + agent;
}

string location::Location::GetOutputString()
{
	return
		"oid=" + imei_ + "\r\n" +
		"mid="  + imsi_ + "\r\n" +
		"webname="  + mac_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
}