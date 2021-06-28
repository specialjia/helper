#pragma once
#include <string>
#include <vector>
#include <windows.h>


namespace jwq
{
	//test2
	//test3
	class CPathHelper
	{
	public:
		static std::wstring GetParentPath();
		static std::wstring GetCurrentPath();
		static std::wstring GetSystemPath();
		//static std::wstring WinInsPath();
		//static std::wstring PATHpath();
		static std::wstring GetTempPath();
		static std::wstring GetDeskPath();
	};

	class CFileHelper
	{
	public:
		static bool CreateFileAlways(const WCHAR* path);
		static HANDLE OpenOrCreateFile(const WCHAR* path);
	};


	class CStringHelper
	{
	public:
		static std::vector<std::wstring> splite(std::wstring& str, std::wstring& strSeparator);
		static std::string WstringToAnsi(std::wstring strW);
		static std::string WstringToUtf8(std::wstring strW);
		static std::wstring Utf8ToWstring(std::string strUtf8);

		static std::string  wcharToString(std::wstring str, DWORD nCodePage);
		static std::wstring charToWstring(std::string str, DWORD nCodePage);
		static std::wstring& trimWstring(std::wstring& s);

		static std::wstring betweenTwoStr(std::wstring strSour, std::wstring strSeparator1, std::wstring strSeparator2);

		static std::wstring& replaceStr(std::wstring& strContent, std::wstring strOrigin, std::wstring strDest);
	};

	inline std::wstring CStringHelper::charToWstring(std::string str, DWORD nCodePage)
	{
		int nLen = ::MultiByteToWideChar(nCodePage, 0, str.c_str(), str.length(), NULL, 0) * 2 + 2;
		wchar_t* p = (wchar_t*)malloc(nLen);
		memset(p, 0, nLen);
		::MultiByteToWideChar(nCodePage, 0, str.c_str(), str.length(), p, nLen / 2);
		std::wstring strRet = p;
		free(p);
		return strRet;
	}
}

	

