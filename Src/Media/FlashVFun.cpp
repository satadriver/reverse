
#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "flashvideo.h"
#include "winsock.h"

//以下所有逻辑为 下行逻辑 或 无特征模糊分析 2007- 6- 29 深圳无下行回滚代码
inline void net3bytetolong(UCHAR* p, UINT& ilong)
{
	ilong=0;
	memcpy((UCHAR *)&ilong+1,p,3);
	ilong=Swap4Byte((ULONG)ilong);
}

//分析flash命令
BOOL ParserFlashCommand(UCHAR *p,BOOL bhead)
{
	//三种命令
	if(p[0]>>4==0)
	{
		if (p[7]<0x1f)
			return TRUE;
	}
	else if(p[0]>>4==4)
	{
		if (p[7]<0x1f)
			return TRUE;
	}
	else if(p[0]>>4==8)
	{
		if(bhead)
		{
			if(p[4]==0x52 || p[4]==0x54)
				return TRUE;
			else if( p[4]==0x12 || p[4]==0x22 || p[4]==0x32 )
				return TRUE;
		}
	}
	return FALSE;
}

//分析flash命令
BOOL FlashSession::ParserFlashCommand(UCHAR *p,flashstatus & flashdatastatus,UCHAR suffix)
{
	//三种命令
	if(p[0]>>4==0 && p[1]==0){
		if(p[7]<2 || p[7]>0x1f)
			return FALSE;
		flashdatastatus.commandtype[suffix]=0;
		flashdatastatus.mediatype[suffix]=p[7];
		if(p[7]==9 && suffix==0){
			flashdatastatus.mediatype[1]=8;
		}
		net3bytetolong(p+1,flashdatastatus.timestram[suffix]);
		net3bytetolong(p+4,flashdatastatus.medialength[suffix]);
		if(p[7]!=8 && p[7]>0){

			if(m_bfromserver==0){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-2);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x80;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x80;
			}
			else if(m_bfromserver==1){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-1);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x80;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x80;
			}
			else if(m_bfromserver==3){
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix){
					flashdatastatus.separator[!suffix]=0xC0;
					m_AVseparator[0]=0xC0 + (p[0] & 0x0F-1);//下行
					m_AVseparator[1]=0xC0 + (p[0] & 0x0F-2);//上行
				}
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x80;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x80;
			}

		}
		else if(p[7]==8){//音频
			if(m_bfromserver==0){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-2);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x41;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x41;
			}
			else if(m_bfromserver==1){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-1);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x41;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x41;
			}
			else if(m_bfromserver==3){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix){
					flashdatastatus.separator[!suffix]=0xC0;
					m_AVseparator[0]=0xC0 + (p[0] & 0x0F-1);//下行
					m_AVseparator[1]=0xC0 + (p[0] & 0x0F-2);//上行
				}
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x41;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x41;				
			}
		}
		return TRUE;
	}
	else if(p[0]>>4==4 && p[1]==0 && p[7]>0){
		if(p[7]<2 || p[7]>0x1f)
			return FALSE;
		
		flashdatastatus.commandtype[suffix]=4;
		flashdatastatus.mediatype[suffix]=p[7];
		if(p[7]==9 && suffix==0){
			flashdatastatus.mediatype[1]=8;
		}
		net3bytetolong(p+1,flashdatastatus.timestram[suffix]);
		net3bytetolong(p+4,flashdatastatus.medialength[suffix]);	
		if(p[7]!=8){
			if(m_bfromserver==0){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-2);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x80;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x80;
			}
			else if(m_bfromserver==1){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-1);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x80;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x80;
			}
			else if(m_bfromserver==3){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix){
					flashdatastatus.separator[!suffix]=0xC0;
					m_AVseparator[0]=0xC0 + (p[0] & 0x0F-1);//下行
					m_AVseparator[1]=0xC0 + (p[0] & 0x0F-2);//上行
				}
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x80;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x80;				
			}
		}
		else if(p[7]==8){
			if(m_bfromserver==0){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-2);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x41;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x41;
			}
			else if(m_bfromserver==1){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix)
					flashdatastatus.separator[!suffix]=0xC0 + (p[0] & 0x0F-1);
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x41;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x41;
			}
			else if(m_bfromserver==3){
				//分隔符号
				flashdatastatus.separator[suffix]=0xC0 + (p[0] & 0x0F);
				if(!suffix){
					flashdatastatus.separator[!suffix]=0xC0;
					m_AVseparator[0]=0xC0 + (p[0] & 0x0F-1);//下行
					m_AVseparator[1]=0xC0 + (p[0] & 0x0F-2);//上行
				}
				//余数
				flashdatastatus.residual[suffix]=flashdatastatus.medialength[suffix] % 0x41;
				if(flashdatastatus.residual[suffix]==0)
					flashdatastatus.residual[suffix]=0x41;				
			}
		}		

		return TRUE;
	}
	else if(p[0]>>4==8 ){
		//只能是音视频
		if(flashdatastatus.separator[0]==0 &&
			flashdatastatus.separator[1]==0)
			return FALSE;

		if((flashdatastatus.separator[0] & 0x0f)==(p[0] & 0x0f)){
			flashdatastatus.commandtype[0]=8;
			if(flashdatastatus.mediatype[0]==8){
				flashdatastatus.medialength[0]=0;
				flashdatastatus.residual[0]=0;
			}
				
			net3bytetolong(p+1,flashdatastatus.timestram[0]);
			return TRUE;
		}
		else if((flashdatastatus.separator[1] & 0x0f)==(p[0] & 0x0f)){
			flashdatastatus.commandtype[1]=8;
			if(flashdatastatus.mediatype[1]==8){
				flashdatastatus.medialength[1]=0;
				flashdatastatus.residual[1]=0;
			}
			net3bytetolong(p+1,flashdatastatus.timestram[1]);
			return TRUE;
		}
		else
			return FALSE;
	}
	else 
		return FALSE;
	
}

int SearchFlashHeader(UCHAR * p,int len)
{
	
	UCHAR * Packet=0;
	if(len>=9)
	{
		for (int i=0;i<len-9;i++)
		{
			Packet=p+i;
			if(Packet[0]>>4==4 && Packet[1]==0 && 
				((Packet[7]==0x09  && (Packet[8] ==0x12 || Packet[8] ==0x22 ||Packet[8] ==0x32) )
				|| (Packet[7]==0x08 && Packet[8]==0x52 )))
				return i;
		}
	}
	
	return -1;
}

int WriteFlashStream(FILE * fs,UCHAR * buf, UINT Length,UINT medialength,UCHAR mediatype,UINT timestramp,MediaCommInfo *pheader)
{
	int re=0;
	try
	{

		if (Length < medialength)
			return 0;
		if(mediatype==8)
		{
			if(fs>0)
			{
				while(Length>=0x41 || medialength>0)
				{
					if(medialength>0)
					{
						if(pheader->m_havehead==0)
						{
							fwrite("FLV\x1\x5\x0\x0\x0\x9",1,9,fs);
							fwrite("\x0\x0\x0\x0\x8",1,5,fs);
							fwrite("\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0",1,10,fs);
							//写前长度    4
							UINT tmplength=11;
						
							fwrite(((char *)&tmplength)+3,1,1,fs);
							fwrite(((char *)&tmplength)+2,1,1,fs);
							fwrite(((char *)&tmplength)+1,1,1,fs);
							fwrite(((char *)&tmplength),1,1,fs);
							pheader->m_havehead=1;
						}
						//写音频
						if(medialength>0)
						{
							//09          1
							fwrite("\x8",1,1,fs);
							//写当前长度  3
							fwrite(((char *)&medialength)+2,1,1,fs);
							fwrite(((char *)&medialength)+1,1,1,fs);
							fwrite(((char *)&medialength),1,1,fs);				

							//pheader->timestramp+=timestramp;
							pheader->m_timestramp+=timestramp;
							if(pheader->m_timestramp<=pheader->m_videotimestramp)
								pheader->m_timestramp=pheader->m_videotimestramp+0x05;
							fwrite(((char *)&pheader->m_timestramp)+2,1,1,fs);
							fwrite(((char *)&pheader->m_timestramp)+1,1,1,fs);
							fwrite(((char *)&pheader->m_timestramp)+0,1,1,fs);

							//printf("%06x\n",tmpflahdatahead.timestram);
							//00 00 00 00 4
							fwrite("\x0\x0\x0\x0",1,4,fs);
							fwrite(buf,1,medialength,fs);
							UINT tmplength=medialength+11;
							fwrite(((char *)&tmplength)+3,1,1,fs);
							fwrite(((char *)&tmplength)+2,1,1,fs);
							fwrite(((char *)&tmplength)+1,1,1,fs);
							fwrite(((char *)&tmplength),1,1,fs);	
						}
						buf+=medialength;					
						medialength=0;
						if(Length>=medialength)
						{
							Length-=medialength;
						}
						pheader->m_havehead=1;
					}
					else
					{
						if(pheader->m_havehead==0)
						{
							fwrite("FLV\x1\x5\x0\x0\x0\x9",1,9,fs);
							fwrite("\x0\x0\x0\x0\x8",1,5,fs);
							fwrite("\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0",1,10,fs);
							//写前长度    4
							UINT tmplength=11;
							
							fwrite(((char *)&tmplength)+3,1,1,fs);
							fwrite(((char *)&tmplength)+2,1,1,fs);
							fwrite(((char *)&tmplength)+1,1,1,fs);
							fwrite(((char *)&tmplength),1,1,fs);	
							pheader->m_havehead=1;
						}
						//写视频
						if(Length>0)
						{
							//09          1
							fwrite("\x8",1,1,fs);
							//UINT tmpLength=0x41;
							//写当前长度  3
							fwrite("\x0",1,1,fs);
							fwrite("\x0"+1,1,1,fs);
							fwrite("\x41",1,1,fs);				

							pheader->m_timestramp+=0x20;
							if(pheader->m_timestramp<=pheader->m_videotimestramp)
								pheader->m_timestramp=pheader->m_videotimestramp+0x05;
							fwrite(((char *)&pheader->m_timestramp)+2,1,1,fs);
							fwrite(((char *)&pheader->m_timestramp)+1,1,1,fs);
							fwrite(((char *)&pheader->m_timestramp)+0,1,1,fs);
							//printf("%06x\n",tmpflahdatahead.timestram);
							//00 00 00 00 4
							fwrite("\x0\x0\x0\x0",1,4,fs);
							fwrite(buf,1,0x41,fs);
							UINT tmplength=0x41+11;
							fwrite(((char *)&tmplength)+3,1,1,fs);
							fwrite(((char *)&tmplength)+2,1,1,fs);
							fwrite(((char *)&tmplength)+1,1,1,fs);
							fwrite(((char *)&tmplength),1,1,fs);	

						}				
						Length-=0x41;
						buf+=0x41;
						pheader->m_havehead=1;
					}

				}//end while

			}// end if s>0
		}
		else if(mediatype==9)
		{
			if(fs>0)
			{
				if(pheader->m_havehead==0)
				{
					fwrite("FLV\x1\x5\x0\x0\x0\x9",1,9,fs);
					fwrite("\x0\x0\x0\x0\x8",1,5,fs);
					fwrite("\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0",1,10,fs);
					//写前长度    4
					UINT tmplength=11;
					
					fwrite(((char *)&tmplength)+3,1,1,fs);
					fwrite(((char *)&tmplength)+2,1,1,fs);
					fwrite(((char *)&tmplength)+1,1,1,fs);
					fwrite(((char *)&tmplength),1,1,fs);
					pheader->m_havehead=1;
				}
				//写视频
				if(Length>0)
				{
					//09          1
					fwrite("\x9",1,1,fs);
					//写当前长度  3
					fwrite(((char *)&Length)+2,1,1,fs);
					fwrite(((char *)&Length)+1,1,1,fs);
					fwrite(((char *)&Length),1,1,fs);				
					

					//pheader->timestramp+=0x20;

					if(pheader->m_timestramp>0)
					{
						if(pheader->m_videotimestramp+timestramp>=pheader->m_timestramp)
							pheader->m_videotimestramp+=timestramp ;
						else
							pheader->m_videotimestramp=pheader->m_timestramp+0x1;
					}
					fwrite(((char *)&pheader->m_videotimestramp)+2,1,1,fs);
					fwrite(((char *)&pheader->m_videotimestramp)+1,1,1,fs);
					fwrite(((char *)&pheader->m_videotimestramp)+0,1,1,fs);
					if(pheader->m_timestramp==0){
						pheader->m_videotimestramp+=timestramp ;
					}
	//				pheader->videotimestramp=
					pheader->m_deltavideotimes=timestramp;
	//				pheader->videotimestramp=pheader->timestramp;
					//printf("%06x\n",tmpflahdatahead.timestram);
					//00 00 00 00 4
					fwrite("\x0\x0\x0\x0",1,4,fs);
					fwrite(buf,1,Length,fs);
					UINT tmplength=Length+11;
					fwrite(((char *)&tmplength)+3,1,1,fs);
					fwrite(((char *)&tmplength)+2,1,1,fs);
					fwrite(((char *)&tmplength)+1,1,1,fs);
					fwrite(((char *)&tmplength),1,1,fs);	
					pheader->m_havehead=1;
					//fclose(fs);	
				}//end if(Length>0)
			}//end if s>0
			
		}//if(mediatype==9)

	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "WriteFlashStream() Error!!");
	}
	return re;
}
