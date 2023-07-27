
#include "StdAfx.h"
#include "Taxi.h"

#include "TaxiDiDi.h"
#include "TaxiKuaiDi.h"
#include "TaxiYiDaoYongChe.h"
#include "TaxiYiHaoZhuanChe.h"
#include "TaxiDiDiZhuanChe.h"
#include "TaxiMobike.h"

taxi::Taxi::Taxi(HttpSession *http_session, const string &pro_name) :  HttpPro(http_session, pro_name)
{

}

taxi::Taxi::~Taxi()
{
	
}

taxi::Taxi::ProcessTaxi(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	int n = 0;
	try
	{
		action = DiDi::IsDiDi(http_session);
		if (action)
		{
			n = 1;
			DiDi didi(http_session,"ddtaxi");
			return didi.Process(packet,action);
		}

		action = KuaiDi::IsKuaiDi(http_session);
		if (action)
		{
			n = 2;
			KuaiDi kuaidi(http_session,"kuaidi");
			return kuaidi.Process(packet,action);
		}

		action = YiDaoYongChe::IsYiDaoYongChe(http_session);
		if (action)
		{
			n = 3;
			YiDaoYongChe ydyc(http_session,"taxi_ydyc");
			return ydyc.Process(packet,action);
		}

		action = YiHaoZhuanChe::IsYiHaoZhuanChe(http_session);
		if (action)
		{
			n = 4;
			YiHaoZhuanChe yhzc(http_session,"taxi_yhzc");
			return yhzc.Process(packet,action);
		}

		action = DiDiZhuanChe::IsDiDiZhuanChe(http_session);
		if (action)
		{
			n = 5;
			DiDiZhuanChe ddzc(http_session,"taxi_ddzc");
			return ddzc.Process(packet,action);
		}
		//Ä¦°Ýµ¥³µ
		action = Mobike::IsMobike(http_session);
		if (action)
		{
			n = 6;
			Mobike ddzc(http_session,"TAXI_MOBIKE");
			return ddzc.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessTaxi Err : %d",n);
	}
	return 0;
}
/*
int taxi::Taxi::GetRealFileSize(const char *stream_begin,const char *stream_end)
{
	int len = 0;
	while(1)
	{
		if (memcmp(stream_begin,stream_end,strlen(stream_end)) == 0)
		{
			return len;
		}
		else
		{
			len++;
			stream_begin++;
			if (len > real_file_max_size_)
			{
				return 0;
			}
		}
	}
	return 0;
}
*/
string taxi::Taxi::GetOutputText()
{
	return "";
}