
#ifndef HTTP_READING_READINGTIANTIANKUAIBAO_H_H_H
#define HTTP_READING_READINGTIANTIANKUAIBAO_H_H_H
/************************************************************************/
/* 天天快报                                                            */
/************************************************************************/
#include "Reading.h"
namespace reading
{
	class TianTianKuaiBao : public Reading
	{
	public:
		TianTianKuaiBao(HttpSession *http_session, const string &pro_name);
		~TianTianKuaiBao();
		
		static int IsTianTianKuaiBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int AddCommentM(const char *packet);
		enum 
		{
			kAddCommentM = 1, //2014-11-14
		};
	};
}

#endif