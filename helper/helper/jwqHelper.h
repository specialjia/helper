#pragma once
#include <string>
#include <vector>
#include <windows.h>


namespace jwq
{
	
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
		
		static bool CreateFileEx(std::wstring path);
		static HANDLE OpenOrCreateFile(const WCHAR* path);
	private:
		static bool CreateFileAlways(const WCHAR* path);
	};


	class CStringHelper
	{
	public:
		static std::vector<std::wstring> splite(std::wstring& str, std::wstring& strSeparator);
		static std::string WstringToAnsi(std::wstring strW);
		static std::string WstringToUtf8(std::wstring strW);
		static std::wstring Utf8ToWstring(std::string strUtf8);
		static std::wstring AnsiToWstring(std::string strAnsi);

		static std::string  wcharToString(std::wstring str, DWORD nCodePage);
		static std::wstring charToWstring(std::string str, DWORD nCodePage);
		static std::wstring& trimWstring(std::wstring& s);

		static std::wstring betweenTwoStr(std::wstring strSour, std::wstring strSeparator1, std::wstring strSeparator2);

		static std::wstring& replaceStr(std::wstring& strContent, std::wstring strOrigin, std::wstring strDest);
	};

	class CCmdHelper
	{
	public:
		static bool cmdRun(std::wstring strCmd, std::wstring& strMsg, std::wstring strFileFullPathNameToSave = L"log.txt",
			DWORD dwMillisecondsTimeOut = INFINITE, bool bWow64FsRedirection = false, bool bTimeOutKillProcess = false);

		static bool ExecuteAndGetStdOutput(std::wstring strCmd, std::wstring& strStdOutput, std::wstring& strMsg,
			DWORD dwMillisecondsTimeOut = INFINITE, bool bWow64FsRedirection = false, bool bTimeOutKillProcess = false);
	};

	class CDirHelper
	{
	public:
		static bool CreateDirRecursively(const std::wstring& directory);
		static bool DeleteDirAndFileAll(std::wstring dir);
		static std::wstring GetPathDir(std::wstring exePath);
	};
	
}

	

