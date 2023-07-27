//
// C++ Interface: dbgswitch
//
// Description: 
//
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBGSWITCH_H
#define DBGSWITCH_H

/**
 */
/*!\class sDebug
 * \brief 输出调试信息开关类
 */
class sDebug
{
	public:
		sDebug();
		void Update();
		void CheckUpdate(const char *desc, const char *chkflg, int &dbgswitch);
	
	public:
		int	m_bdumpflg;			///< dump switch list
		int m_outuplogin;		///< 输出全文
		int m_dbgtrystat;			///< 输出全文
		int m_dbgtryall;			///< 输出全文
		int m_bkfirstz;			///< 启动, 如果z有数据, 备份下来
		int m_outqqcmd;
		int m_dbgqq;
		int m_dbgqqu;
};

/*!\var 开关*/
extern sDebug 	g_dbgSwitch;

#endif
