
#include "StdAfx.h"
#include "StringManipulation.h"


TransformEncoding::TransformEncoding()
{

}

TransformEncoding::~TransformEncoding()
{

}

StringManipulation::StringManipulation()
{

}

StringManipulation::~StringManipulation()
{

}

void StringManipulation::SplitStringToMap(const string &src_string ,char sep_item,char sep_value,const string &need_trim)
{
	temp_map_.clear();

	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_string.find(sep_item,item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos) 
			{
				break;
			}
			else
			{
				item = src_string.substr(item_beg_pos); 
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = src_string.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + sizeof(sep_item);
		}
		
		string::size_type value_pos = item.find(sep_value);
		if (value_pos == string::npos)
		{
			continue;
		}
		string key(item.substr(0,value_pos));
		Trim(key);
		string val(item.substr(value_pos + sizeof(sep_value)));
		Trim(key);
		if (!need_trim.empty())
		{
			Trim(key,need_trim);
			Trim(val,need_trim);
		}
		temp_map_.insert(std::make_pair(key,val));
	}
}
//字符串
void StringManipulation::SplitStringToMap(const string &src_string ,const string &sep_item,const string &sep_value,const string &need_trim)
{
	temp_map_.clear();
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = src_string.find(sep_item,item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos) 
			{
				break;
			}
			else
			{
				item = src_string.substr(item_beg_pos); 
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = src_string.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + sep_item.length();
		}
		
		string::size_type value_pos = item.find(sep_value);
		if (value_pos == string::npos)
		{
			continue;
		}
		string key(item.substr(0,value_pos));
		Trim(key);
		string val(item.substr(value_pos + sep_value.length()));
		Trim(key);
		if (!need_trim.empty())
		{
			Trim(key,need_trim);
			Trim(val,need_trim);
		}
		temp_map_.insert(std::make_pair(key,val));
	}
}

string StringManipulation::GetValueFromMapByKey(const string &key_string, bool trans_code)
{	
	map<string, string>::iterator iter = temp_map_.find(key_string);
	if (iter != temp_map_.end())
	{
		if (trans_code)
		{
			return UTF8ToGBK(UrlDecode(iter->second));
		}
		else
		{
			return iter->second;
		}	
	}
	else
	{
		return "";
	}
}

string StringManipulation::GetValueBetweenBeginEnd(const string &src_string,
												   const string &begin_string,
												   const string &end_string,
												   bool need_decode)
{
	string::size_type begin_pos = src_string.find(begin_string);
	string::size_type end_pos = string::npos;
	if (begin_pos != string::npos)
	{
		end_pos = src_string.find(end_string,begin_pos + begin_string.length());
		if (end_pos != string::npos)
		{
			string result = src_string.substr(begin_pos + begin_string.length(),
				end_pos - begin_pos - begin_string.length());
			if (need_decode)
			{
				return UTF8ToGBK(UrlDecode(result));
			}
			else
			{
				return result;
			}
		}
	}
	return "";
}


//urI 解码
string StringManipulation::UrlDecode(const std::string& url)
{
    string result;
    int hex = 0;
    for (string::size_type i = 0; i < url.size(); ++i)
    {
        switch (url[i])
        {
        case '+':
            result += ' ';
            break;
        case '%':
            if (isxdigit(url[i + 1]) && isxdigit(url[i + 2]))  
            {
                string temp = url.substr(i + 1, 2);            
                hex = strtol(temp.c_str(), 0, 16);
                if (!((hex >= '0' && hex <= '9') || (hex >= 'a' && hex <= 'z') || (hex >= 'A' && hex <= 'Z') /*
				(hex == '-') || (hex == '_') || (hex == '.') || (hex == '*') */))
                {
                    result += char(hex);
                    i += 2;
                }
                else 
				{
					result += '%';
				}
            }
			else 
			{
                result += '%';
            }
            break;
        default:
            result += url[i];
            break;
        }
    }
    return result;
}

string StringManipulation::UTF8ToGBK(const string &utf8)  
{
	int length1 = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.c_str(), utf8.length(), NULL, 0);
	
	if(length1 > 0)
	{		
		WCHAR *wbuf = new WCHAR[length1 * 2 + 2];
		memset(wbuf,0,length1 * 2 + 2);
		int length2 = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.length(), wbuf, length1);
		if(length2 > 0)
		{
			length1 = WideCharToMultiByte(CP_ACP, 0, wbuf, length2, NULL, 0, NULL, NULL);
			if(length1 > 0)
			{
				char *buf = new char[length1 + 1];
				memset(buf,0,length1 + 1);
				length2 = WideCharToMultiByte(CP_ACP, 0, wbuf, length2, buf, length1, NULL, NULL);
				if(length2 > 0)
				{
					string gbk(buf);
					delete [] wbuf;
					delete [] buf;
					return gbk;
				}
				else
				{
					delete [] wbuf;
					delete [] buf;
					return utf8;
				}	
			}
		}
		else
		{
			delete [] wbuf;
			return utf8;
		}	
	}
	return utf8;
}

string StringManipulation::UnicodeToGBKPrefix_u(const string &src_string)
{
	string result;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;
	while(1)
	{
		item_end_pos = src_string.find("\\u",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = src_string.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = src_string.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("\\u");
		}
		
		Replace(item,"\\u","");
		string tail;
		string unicode;
		if (item.size() == 4)
		{
			unicode = item;
		}
		else if (item.size() > 4)
		{
			tail = item.substr(4);
			unicode = item.substr(0, 4);
		}
		result = result + UnicodeToGBK(unicode) + tail;
	}
	return result;
}

//%u300A%u5C01%u795E%u300B 替换 %u 你好
string StringManipulation::UnicodeToGBK(const string &unicode)
{
	const char *src = unicode.c_str();
	
	int wlength = unicode.size() / 4;
	wchar_t *wbuf = new wchar_t[wlength + 1];

	for (int i = 0; i < wlength; i++)
	{
		char tmp[5] = {0};
		memcpy(tmp, &src[i * 4], 4);
		
		long val = strtol(tmp, NULL, 16);
		memcpy(&wbuf[i], &val, sizeof(long));
	}
	
	wbuf[wlength] = 0;
	
	int length = WideCharToMultiByte(936,0,wbuf,-1,NULL,0,NULL,NULL);

	char *buf = new char[length + 1];

	WideCharToMultiByte(936,0,wbuf,-1,buf,length,NULL,NULL);
	buf[length] = 0;

	string gbk(buf);
	delete [] wbuf;
	delete [] buf;
	return gbk;
}

string StringManipulation::UnicodeToZhongWen(const string &src_string)
{  
	string result;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;
	int hex = 0;
	while(1)
	{
		item_end_pos = src_string.find(";",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = src_string.substr(item_beg_pos);
				item_beg_pos = string::npos;
				result = result + item;
				break;
			}
		}
		else
		{
			item = src_string.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen(";");
		}
		string::size_type value_pos = item.find("&#");
		if (value_pos == string::npos)
		{
			result = result + item + ";";
			continue;
		}
		string val(item.substr(value_pos + strlen("&#")));
		hex  = atoi(val.c_str());
		char str[32];
		itoa(hex,str,16);
		string  tmp(str);
		result = result + UnicodeToGBK(tmp);
	}
	return result;
}

string StringManipulation::HexstringToAsciistring(const string &hexstring)
{
	string::size_type pos = 0;
	string result;
	
	while (1)
	{
		if (pos > hexstring.size())
		{
			break;
		}
		else
		{
			string tmpstr = hexstring.substr(pos,2);
			if (tmpstr.size() != 2)
			{
				break;
			}
			char tmpchar[2] = {0};
			tmpchar[0] = strtol(tmpstr.c_str(),0,16);
			string tmpres(tmpchar);
			result += tmpres;
			pos += 2;
		}
		
	}
	return result;
	
}
string StringManipulation::Base64Decode(const string &base64,int& out_byte)
{
	const char *data = base64.c_str();
	int data_byte = base64.size();
	const char decode_table[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			62, // '+'
			0, 0, 0,
			63, // '/'
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
			0, 0, 0, 0, 0, 0, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
			0, 0, 0, 0, 0, 0,
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	
	string decoded;
	int value;
	int i = 0;
	while (i < data_byte)
	{
		if (*data != '\r' && *data!='\n')
		{
			value = decode_table[*data++] << 18;
			value += decode_table[*data++] << 12;
			decoded += (value & 0x00FF0000) >> 16;
			out_byte++;
			if (*data != '=')
			{
				value += decode_table[*data++] << 6;
				decoded += (value & 0x0000FF00) >> 8;
				out_byte++;
				if (*data != '=')
				{
					value += decode_table[*data++];
					decoded += value & 0x000000FF;
					out_byte++;
				}
			}
			i += 4;
		}
		else
		{
			data++;
			i++;
		}
	}
	return decoded;
}

int StringManipulation::ToInt(const string &src_string)
{
	return atoi(src_string.c_str());
}

string StringManipulation::ToLower(const string &src_string)
{
	string result = src_string;
	for (int i = 0; i < src_string.size(); i++)
	{
		if ( ('A' <= src_string[i] ) && (src_string[i] <= 'Z'))
		{
			result[i] = result[i] + 32;
		}
	}
	return result;
}

string StringManipulation::Trim(string &src_string, const string &need_trim)
{
	if (!src_string.empty())
	{
		string::size_type index = src_string.find_first_not_of(need_trim);
		if (index != string::npos)
		{
			src_string = src_string.substr(index);
		}

		index = src_string.find_last_not_of(need_trim) + 1;
		if (index != string::npos)
		{
			src_string = src_string.substr(0,index);
		}
	}
	return src_string;
}

void StringManipulation::Replace(string &src_string,const string &old_string ,const string &new_string)
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

//收件人 格式化 2016-08-26
string StringManipulation::SplitToFormat( string src_string , string sep_begin,  string sep_end)
{
	//"2584855711@qq.com" <2584855711@qq.com>,"yuleili2013@126.com" <yuleili2013@126.com> 
    //2584855711@qq.com;yuleili2013@126.com;
	string split(";");
	string strEnd;
	string::size_type iPos;
	string::size_type iTos;
	while((iPos = src_string.find(sep_begin),0) != string::npos && (iTos = src_string.find(sep_end),iPos) != string::npos)
	{
		string strFirst = src_string.substr(iPos + sep_begin.length(),iTos -iPos - sep_begin.length());
		strEnd += strFirst;
		strEnd += split;
		src_string = src_string.substr(iTos + sep_end.length(),src_string.length() - iTos + sep_end.length());
	}
    return strEnd;
}
