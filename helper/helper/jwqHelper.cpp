#define _CRT_SECURE_NO_WARNINGS
#include "jwqHelper.h"

#include <atlstr.h>
#include <windows.h>
#include <ShlObj.h>

#include "log.h"

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

bool jwq::CFileHelper::CreateFileEx(std::wstring path)
{
	bool ret = CreateFileAlways(path.c_str());
	if(!ret && GetLastError()==3)
	{
		std::wstring targetDir = jwq::CDirHelper::GetPathDir(path);
		ret = jwq::CDirHelper::CreateDirRecursively(targetDir);
		ret = CreateFileAlways(path.c_str());
	}
	return ret;
	
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

std::wstring jwq::CStringHelper::AnsiToWstring(std::string strAnsi)
{
	return charToWstring(strAnsi, CP_ACP);
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


std::wstring jwq::CStringHelper::charToWstring(std::string str, DWORD nCodePage)
{
	int nLen = ::MultiByteToWideChar(nCodePage, 0, str.c_str(), str.length(), NULL, 0) * 2 + 2;
	wchar_t* p = (wchar_t*)malloc(nLen);
	memset(p, 0, nLen);
	::MultiByteToWideChar(nCodePage, 0, str.c_str(), str.length(), p, nLen / 2);
	std::wstring strRet = p;
	free(p);
	return strRet;
}


bool jwq::CCmdHelper::cmdRun(std::wstring strCmd, std::wstring& strMsg, std::wstring strFileFullPathNameToSave,
	 DWORD dwMillisecondsTimeOut, bool bWow64FsRedirection, bool bTimeOutKillProcess)
{

	DebugLog(L"Enter strCmd(%s), dwMillisecondsTimeOut(%d), bWow64FsRedirection(%s)", strCmd.c_str(), dwMillisecondsTimeOut, bWow64FsRedirection ? L"true" : L"false");

	BOOL bSuccessful = FALSE;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	DWORD dwReturn = 0;

	std::wstring strTemp;

	DWORD dwErrorCode = 0;

	HANDLE hFileOuntput = NULL;

	SECURITY_ATTRIBUTES psa = { sizeof(psa),NULL,TRUE };;
	psa.bInheritHandle = TRUE;

	WCHAR szwcharCmd[MAX_PATH];

	 
	PVOID OldValue = NULL;
	//==================================================================================================

	si.cb = sizeof(STARTUPINFO);

	GetStartupInfo(&si);

	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
#ifdef _DEBUG
	si.wShowWindow = TRUE; //TRUE表示显示创建的进程的窗口
#else
	si.wShowWindow = FALSE; //TRUE表示显示创建的进程的窗口
#endif // DEBUG

	hFileOuntput = CreateFile(strFileFullPathNameToSave.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		&psa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFileOuntput)
	{
		strMsg = L"create file err";
		strMsg += jwq::CStringHelper::AnsiToWstring(_itoa(GetLastError(), NULL, 16));
		return false;
	}
	//--------------------------------------------------------------------------------------------------
	si.hStdOutput = hFileOuntput;
	si.hStdInput = hFileOuntput;
	si.hStdError = hFileOuntput;

	wcscpy(szwcharCmd, strCmd.c_str());

	 
	bSuccessful = CreateProcess(NULL,
		szwcharCmd, //在Unicode版本中此参数不能为常量字符串，因为此参数会被修改    
		NULL,
		NULL,
		TRUE,
		NULL,
		NULL,
		NULL,
		&si,
		&pi);

	dwErrorCode = GetLastError();

	 
	if (!bSuccessful)
	{
		strMsg = L"CreateProcess err";
		strMsg += jwq::CStringHelper::AnsiToWstring(_itoa(dwErrorCode, NULL, 16));
		CloseHandle(hFileOuntput);
		return false;
	}

	//-------------------------------------------------------------------------------------------------------------
	dwReturn = WaitForSingleObject(pi.hProcess, dwMillisecondsTimeOut);
	switch (dwReturn)
	{
	case WAIT_ABANDONED:
	{ 
	}
	break;
	case WAIT_OBJECT_0:
	{
		 
		if (bTimeOutKillProcess)
		{
			bSuccessful = ::TerminateProcess(pi.hProcess, 0);
			dwReturn = ::WaitForSingleObject(pi.hProcess, 800);
			if (!bSuccessful)
			{
				DWORD dwErrorCode = GetLastError();

				int i = 0;
			}
		}
	}
	break;
	case WAIT_TIMEOUT:
	{
		strMsg = L"wait timeout";
		if (bTimeOutKillProcess)
		{
			bSuccessful = ::TerminateProcess(pi.hProcess, 0);
			dwReturn = ::WaitForSingleObject(pi.hProcess, 800);
			if (!bSuccessful)
			{
				DWORD dwErrorCode = GetLastError();

				int i = 0;
			}
		}

		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		::CloseHandle(hFileOuntput);
		return false;
	}
	break;
	case WAIT_FAILED:
	{
		strMsg = L"wait failed";

		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		::CloseHandle(hFileOuntput);
		return false;
	}
	break;
	default:
	{
		 
	}
	}

	//-------------------------------------------------------------------------------------------------------------

	//::DeleteFile(strFileFullPathNameToSave);

	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);
	::CloseHandle(hFileOuntput);

	DebugLog(L"Exit");

	return true;
}

bool jwq::CDirHelper::CreateDirRecursively(const std::wstring& directory)
{
	auto ret = SHCreateDirectoryExW(0, directory.c_str(), NULL);
	return ret == ERROR_SUCCESS || ret == ERROR_ALREADY_EXISTS;
}

bool jwq::CDirHelper::DeleteDir(std::wstring dir)
{
	WIN32_FIND_DATA ffd;
	CString path;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	BOOL   success = TRUE;
	path.Append(dir.c_str());
	path.Append(_T("\\*"));
	hFind = FindFirstFile(path, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return success;
	}
	do {
		CString name;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		else
		{
			name.Append(dir.c_str());
			name.Append(_T("\\"));
			name.Append(ffd.cFileName);
			success = DeleteFile(name);
			if (!success)
			{
				return success;
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	return success;

	 
}

std::wstring jwq::CDirHelper::GetPathDir(std::wstring path)
{
	if (path.find(L'\\') == -1 && path.find(L'/') == -1) return L"";

	int pos = path.rfind(L'\\');
	if (pos == std::wstring::npos)
	{
		pos = path.rfind(L'/');
	}
	if (pos == std::wstring::npos) return L"";
	std::wstring dir = path.substr(0, pos);
	return dir;
}
