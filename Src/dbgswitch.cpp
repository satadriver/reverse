//
// C++ Implementation: dbgswitch
//
// Description: 
//
//
//
//
//
#include "StdAfx.h"
#include "dbgswitch.h"
// #include "PublicFun.h"

/*!\var 开关*/
sDebug 	g_dbgSwitch;

/*!\fn 调试开关类构造函数
 */
sDebug::sDebug()
{
	m_bdumpflg = true;		/// 首次Update时Dump所有标志文件列表
	
	m_outuplogin = false;
	m_bkfirstz = false;
	m_outqqcmd = false;
	m_dbgqq = false;
	m_dbgqqu = false;
	m_dbgtrystat = false;
	m_dbgtryall = false;
	CreateDir("e:\\netspy\\flg\\dce");
}

/*!\fn 调试开关, 更新一项
 */
void sDebug::CheckUpdate(const char *desc, const char *chkflg, int &dbgswitch)
{
	if(access(chkflg, 0) == 0)
	{
		if(false == dbgswitch)
		{
			if(false == m_bdumpflg)
				WriteLog("dce", "%s 开关变化, 修改后为 打开", desc);
			dbgswitch = true;
		}
	}
	else
	{
		if(dbgswitch)
		{
			if(false == m_bdumpflg)
				WriteLog("dce", "%s 开关变化, 修改后为 关闭", desc);
			dbgswitch = false;
		}
	}
	if(m_bdumpflg)
	{
		WriteLog("dce", "%-30s, state:%s, %s", chkflg, dbgswitch?"打开":"关闭", desc);
	}
}

/*!\fn 调试开关更新
 */
void sDebug::Update()
{
	CheckUpdate("输出单条增加webfigure项",
				"e:\\netspy\\flg\\dce\\outuplogin.flg",
				m_outuplogin);

	CheckUpdate("启动即备份Z盘dde目录",
				"e:\\netspy\\flg\\dce\\bkzdisk.flg",
				m_bkfirstz);
	
	CheckUpdate("显示QQ包命令号",
				"e:\\netspy\\flg\\dce\\outqqcmd.flg",
				m_outqqcmd);

	CheckUpdate("获得QQsessionkey后解密并存储出来",
				"e:\\netspy\\flg\\dce\\dbgqq.flg",
				m_dbgqq);

	CheckUpdate("获得QQsessionkey后解密并存储出来(单文件)",
				"e:\\netspy\\flg\\dce\\dbgqqu.flg",
				m_dbgqqu);

	CheckUpdate("输出尝试解密QQ统计信息",
		"e:\\netspy\\flg\\dce\\trystat.flg",
		m_dbgtrystat);

	CheckUpdate("尝试使用QQ全解模式",
		"e:\\netspy\\flg\\dce\\tryall.flg",
		m_dbgtryall);

	/// 首次Dump所有标志文件列表
	m_bdumpflg = false;	/// 关闭
}
