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
 * \brief ���������Ϣ������
 */
class sDebug
{
	public:
		sDebug();
		void Update();
		void CheckUpdate(const char *desc, const char *chkflg, int &dbgswitch);
	
	public:
		int	m_bdumpflg;			///< dump switch list
		int m_outuplogin;		///< ���ȫ��
		int m_dbgtrystat;			///< ���ȫ��
		int m_dbgtryall;			///< ���ȫ��
		int m_bkfirstz;			///< ����, ���z������, ��������
		int m_outqqcmd;
		int m_dbgqq;
		int m_dbgqqu;
};

/*!\var ����*/
extern sDebug 	g_dbgSwitch;

#endif
