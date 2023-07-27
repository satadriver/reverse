// mysql.h: interface for the mysql class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_H__F5D7416F_3E14_458D_8125_9E5E31120AC3__INCLUDED_)
#define AFX_MYSQL_H__F5D7416F_3E14_458D_8125_9E5E31120AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
�ͻ��˽�����ȷ��͹�ȥ
�ͻ��˸��ݷ�����������scramble���ܣ��������scramble_buff�з���������
���͵�¼����:
00000000 3A 00 00 01 85 A6 03 00 00 00 00 01 08 00 00 00
00000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00000020 00 00 00 00 72 6F 6F 74 00 14 00 00 00 00 00 00
00000030 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

Bytes Name
----- ----
4 client_flags
4 max_packet_size
1 charset_number
23 (filler) always 0x00...
n (Null-Terminated String) user
n (Length Coded Binary) scramble_buff (1 + x bytes)
1 (filler) always 0x00
n (Null-Terminated String) databasename

databasename�ֶο��п��ޡ�
scramble_buff�ֶε��������������ģ� 
scramble(end, mysql->scramble, passwd);


 					    42 00  00 01 85 a6 03 00 00 00   ......B. ........
0040  00 01 08 00 00 00 00 00  00 00 00 00 00 00 00 00   ........ ........
0050  00 00 00 00 00 00 00 00  00 00 73 64 7a 77 32 30   ........ ..sdzw20
0060  30 38 69 6e 65 74 00 14  11 80 3b f0 82 1e 53 20   08inet.. ..;...S 
0070  5d a0 e4 00 de de df b7  e7 3a aa 9e               ]....... .:..    
  */

#include "..\\SocketSession.h"

inline int IsMySqlPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type ==TCPPacket)
	{
		WORD len=*(WORD*)(pPacket);
		if(len > 0x24  && len==(packh.m_datalen-4))
		{
			if(memcmp(pPacket+2,"\x00\x01\x85\xA6\x03\x00\x00\x00\x00\x01\x08",11)==0
				&& memcmp(pPacket+0x20, "\x00\x00\x00\x00",4)==0)
			{
				return 1;
			}			
		}	
	}
	
	return 0;
}





class MysqlSession : public SocketSession
{
public:
	MysqlSession(DATEHEADER& packh, int aT);
	virtual ~MysqlSession();

	//*************************************************
	//	��������:		BfSession::InsertPacket
	//	����:			����һ���Ự��һ����
	//	�������:		packh �ǰ�ͷ�ṹ
	//					pPacket �ǰ�����
	//	�������:		
	//	����ֵ:			
	//	����:			
	//**************************************************
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

protected:
	void WriteMySqlIndexFile();
protected:

 

};








#endif // !defined(AFX_MYSQL_H__F5D7416F_3E14_458D_8125_9E5E31120AC3__INCLUDED_)
