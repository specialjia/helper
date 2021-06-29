
#include "jwqHelper.h"

#include <windows.h>
#include <ShlObj.h>

namespace jwq
{
	std::wstring CPathHelper::GetCurrentPath()
	{
		WCHAR path[MAX_PATH] = { 0 };
		GetCurrentDirectoryW(MAX_PATH,path);
		return path;
	}

	std::wstring CPathHelper::GetSystemPath()
	{
		WCHAR path[MAX_PATH] = { 0 };
		GetSystemDirectoryW(path, MAX_PATH);
		return path;
	}

	std::wstring CPathHelper::GetTempPath()
	{
		WCHAR szPath[MAX_PATH] = { 0 };
		::GetTempPathW(MAX_PATH, szPath);

		return szPath;
	}

	std::wstring CPathHelper::GetDeskPath()
	{
		WCHAR path[255];
		SHGetSpecialFolderPathW(0, path, CSIDL_DESKTOPDIRECTORY, 0);
		return path;
	}
}


bool jwq::CFileHelper::CreateFileAlways(const WCHAR *path)
{
	
	HANDLE hFile = CreateFileW(path, GENERIC_WRITE || GENERIC_READ, 
		0,NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		return true;
	}
	CloseHandle(hFile);
	return false;
}

HANDLE jwq::CFileHelper::OpenOrCreateFile(const WCHAR* path)
{
	HANDLE hFile = CreateFileW(path, GENERIC_WRITE || GENERIC_READ,
		0, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	return hFile;
}

std::vector<std::wstring> jwq::CStringHelper::splite(std::wstring& str, std::wstring& strSeparator)
{
	std::vector<std::wstring> v;
	std::wstring strBk = str;
	int pos;
	std::wstring strElement;
	while(true)
	{
		pos = strBk.find(strSeparator);
		if(-1 == pos)
		{
			if(strBk.length() > 0 )
			{
				v.push_back(strBk);
			}
			break;
		}
		else
		{
			strElement = strBk.substr(0, pos);
			if(0 != strElement.length())
			{
				v.push_back(strElement);
			}
			strBk = strBk.substr(pos + strSeparator.length());
		}
	}
	return v;
}

std::string jwq::CStringHelper::WstringToAnsi(std::wstring strW)
{
	return wcharToString(strW, CP_ACP);
}

std::string jwq::CStringHelper::WstringToUtf8(std::wstring strW)
{
	return wcharToString(strW, CP_UTF8);
}

std::wstring jwq::CStringHelper::Utf8ToWstring(std::string strUtf8)
{
	return charToWstring(strUtf8,CP_UTF8);
}

std::string jwq::CStringHelper::wcharToString(std::wstring str, DWORD nCodePage)
{
	int nLen = ::WideCharToMultiByte(nCodePage, 0, str.c_str(), str.length(), NULL, 0,NULL,NULL) + 2;
	char* p = (char*)malloc(nLen);
	memset(p, 0, nLen);
	::WideCharToMultiByte(nCodePage, 0, str.c_str(), str.length(), p, nLen, NULL, NULL);
	std::string strR = p;
	free(p);
	return strR;
}

std::wstring& jwq::CStringHelper::trimWstring(std::wstring& s)
{
	if (s.empty()) return s;
	s.erase(0, s.find_first_not_of(L" "));
	s.erase(s.find_last_not_of(L" ")+1);
	return s;
}

std::wstring jwq::CStringHelper::betweenTwoStr(std::wstring strSour, std::wstring strSeparator1, std::wstring strSeparator2)
{
	if (strSour.empty() || strSeparator1.empty() || strSeparator2.empty()) return L"";
	trimWstring(strSour);
	size_t pos1 = strSour.find(strSeparator1);
	if (std::wstring::npos == pos1) return L"";
	
	size_t pos2 = strSour.find(strSeparator2,pos1+ strSeparator1.length()); //越过它自己
	if (std::wstring::npos == pos2) return L"";

	return strSour.substr(pos1 + strSeparator1.length(), pos2 - (pos1 + strSeparator1.length()));
	
}

std::wstring& jwq::CStringHelper::replaceStr(std::wstring &strContent, std::wstring strOrigin, std::wstring strDest)
{
	if (strContent.empty() || strOrigin.empty()) return strContent;

	for(std::wstring::size_type pos(0); pos != std::wstring::npos;  pos += strDest.length())
	{
		pos = strContent.find(strOrigin, pos);
		if (std::wstring::npos != pos)
		{
			strContent.replace(pos, strOrigin.length(), strDest);
		}
		else
			break;
	}
	return strContent;
}
