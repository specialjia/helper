#define _CRT_SECURE_NO_WARNINGS
#include "jwqHelper.h"

#include <atlstr.h>
#include <windows.h>
#include <ShlObj.h>

#include "log.h"

#include <tchar.h>
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

jwq::CDuiString::CDuiString() : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = '\0';
}

jwq::CDuiString::CDuiString(const TCHAR ch) : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
}

jwq::CDuiString::CDuiString(LPCTSTR lpsz, int nLen) : m_pstr(m_szBuffer)
{
	ASSERT(!::IsBadStringPtr(lpsz, -1) || lpsz == NULL);
	m_szBuffer[0] = '\0';
	Assign(lpsz, nLen);
}

jwq::CDuiString::CDuiString(const CDuiString& src) : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = '\0';
	Assign(src.m_pstr);
}

jwq::CDuiString::~CDuiString()
{
	if (m_pstr != m_szBuffer) free(m_pstr);
}

int jwq::CDuiString::GetLength() const
{
	return (int)_tcslen(m_pstr);
}

jwq::CDuiString::operator LPCTSTR() const
{
	return m_pstr;
}

void jwq::CDuiString::Append(LPCTSTR pstr)
{
	int nNewLength = GetLength() + (int)_tcslen(pstr);
	if (nNewLength >= MAX_LOCAL_STRING_LEN) {
		if (m_pstr == m_szBuffer) {
			m_pstr = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
			_tcscpy(m_pstr, m_szBuffer);
			_tcscat(m_pstr, pstr);
		}
		else {
			m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (nNewLength + 1) * sizeof(TCHAR)));
			_tcscat(m_pstr, pstr);
		}
	}
	else {
		if (m_pstr != m_szBuffer) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
		_tcscat(m_szBuffer, pstr);
	}
}

void jwq::CDuiString::Assign(LPCTSTR pstr, int cchMax)
{
	if (pstr == NULL) pstr = _T("");
	cchMax = (cchMax < 0 ? (int)_tcslen(pstr) : cchMax);
	if (cchMax < jwq::CDuiString::MAX_LOCAL_STRING_LEN) {
		if (m_pstr != m_szBuffer) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
	}
	else if (cchMax > GetLength() || m_pstr == m_szBuffer) {
		if (m_pstr == m_szBuffer) m_pstr = NULL;
		m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(TCHAR)));
	}
	_tcsncpy(m_pstr, pstr, cchMax);
	m_pstr[cchMax] = '\0';
}

bool jwq::CDuiString::IsEmpty() const
{
	return m_pstr[0] == '\0';
}

void jwq::CDuiString::Empty()
{
	if (m_pstr != m_szBuffer) free(m_pstr);
	m_pstr = m_szBuffer;
	m_szBuffer[0] = '\0';
}

LPCTSTR jwq::CDuiString::GetData() const
{
	return m_pstr;
}

TCHAR jwq::CDuiString::GetAt(int nIndex) const
{
	return m_pstr[nIndex];
}

TCHAR jwq::CDuiString::operator[] (int nIndex) const
{
	return m_pstr[nIndex];
}

const jwq::CDuiString& jwq::CDuiString::operator=(const CDuiString& src)
{
	Assign(src);
	return *this;
}

const jwq::CDuiString& jwq::CDuiString::operator=(LPCTSTR lpStr)
{
	if (lpStr)
	{
		ASSERT(!::IsBadStringPtr(lpStr, -1));
		Assign(lpStr);
	}
	else
	{
		Empty();
	}
	return *this;
}

#ifdef _UNICODE

const jwq::CDuiString& jwq::CDuiString::operator=(LPCSTR lpStr)
{
	if (lpStr)
	{
		ASSERT(!::IsBadStringPtrA(lpStr, -1));
		int cchStr = (int)strlen(lpStr) + 1;
		LPWSTR pwstr = (LPWSTR)_alloca(cchStr);
		if (pwstr != NULL) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr);
		Assign(pwstr);
	}
	else
	{
		Empty();
	}
	return *this;
}

const jwq::CDuiString& jwq::CDuiString::operator+=(LPCSTR lpStr)
{
	if (lpStr)
	{
		ASSERT(!::IsBadStringPtrA(lpStr, -1));
		int cchStr = (int)strlen(lpStr) + 1;
		LPWSTR pwstr = (LPWSTR)_alloca(cchStr);
		if (pwstr != NULL) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr);
		Append(pwstr);
	}

	return *this;
}

#else

const jwq::CDuiString& jwq::CDuiString::operator=(LPCWSTR lpwStr)
{
	if (lpwStr)
	{
		ASSERT(!::IsBadStringPtrW(lpwStr, -1));
		int cchStr = ((int)wcslen(lpwStr) * 2) + 1;
		LPSTR pstr = (LPSTR)_alloca(cchStr);
		if (pstr != NULL) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
		Assign(pstr);
	}
	else
	{
		Empty();
	}

	return *this;
}

const jwq::CDuiString& jwq::CDuiString::operator+=(LPCWSTR lpwStr)
{
	if (lpwStr)
	{
		ASSERT(!::IsBadStringPtrW(lpwStr, -1));
		int cchStr = ((int)wcslen(lpwStr) * 2) + 1;
		LPSTR pstr = (LPSTR)_alloca(cchStr);
		if (pstr != NULL) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
		Append(pstr);
	}

	return *this;
}

#endif // _UNICODE

const jwq::CDuiString& jwq::CDuiString::operator=(const TCHAR ch)
{
	Empty();
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
	return *this;
}

jwq::CDuiString jwq::CDuiString::operator+(const CDuiString& src) const
{
	CDuiString sTemp = *this;
	sTemp.Append(src);
	return sTemp;
}

jwq::CDuiString jwq::CDuiString::operator+(LPCTSTR lpStr) const
{
	if (lpStr)
	{
		ASSERT(!::IsBadStringPtr(lpStr, -1));
		CDuiString sTemp = *this;
		sTemp.Append(lpStr);
		return sTemp;
	}

	return *this;
}

const jwq::CDuiString& jwq::CDuiString::operator+=(const CDuiString& src)
{
	Append(src);
	return *this;
}

const jwq::CDuiString& jwq::CDuiString::operator+=(LPCTSTR lpStr)
{
	if (lpStr)
	{
		ASSERT(!::IsBadStringPtr(lpStr, -1));
		Append(lpStr);
	}

	return *this;
}

const jwq::CDuiString& jwq::CDuiString::operator+=(const TCHAR ch)
{
	TCHAR str[] = { ch, '\0' };
	Append(str);
	return *this;
}

bool jwq::CDuiString::operator == (LPCTSTR str) const { return (Compare(str) == 0); };
bool jwq::CDuiString::operator != (LPCTSTR str) const { return (Compare(str) != 0); };
bool jwq::CDuiString::operator <= (LPCTSTR str) const { return (Compare(str) <= 0); };
bool jwq::CDuiString::operator <  (LPCTSTR str) const { return (Compare(str) < 0); };
bool jwq::CDuiString::operator >= (LPCTSTR str) const { return (Compare(str) >= 0); };
bool jwq::CDuiString::operator >  (LPCTSTR str) const { return (Compare(str) > 0); };

void jwq::CDuiString::SetAt(int nIndex, TCHAR ch)
{
	ASSERT(nIndex >= 0 && nIndex < GetLength());
	m_pstr[nIndex] = ch;
}

int jwq::CDuiString::Compare(LPCTSTR lpsz) const
{
	return _tcscmp(m_pstr, lpsz);
}

int jwq::CDuiString::CompareNoCase(LPCTSTR lpsz) const
{
	return _tcsicmp(m_pstr, lpsz);
}

void jwq::CDuiString::MakeUpper()
{
	_tcsupr(m_pstr);
}

void jwq::CDuiString::MakeLower()
{
	_tcslwr(m_pstr);
}

jwq::CDuiString jwq::CDuiString::Left(int iLength) const
{
	if (iLength < 0) iLength = 0;
	if (iLength > GetLength()) iLength = GetLength();
	return CDuiString(m_pstr, iLength);
}

jwq::CDuiString jwq::CDuiString::Mid(int iPos, int iLength) const
{
	if (iLength < 0) iLength = GetLength() - iPos;
	if (iPos + iLength > GetLength()) iLength = GetLength() - iPos;
	if (iLength <= 0) return CDuiString();
	return CDuiString(m_pstr + iPos, iLength);
}

jwq::CDuiString jwq::CDuiString::Right(int iLength) const
{
	int iPos = GetLength() - iLength;
	if (iPos < 0) {
		iPos = 0;
		iLength = GetLength();
	}
	return CDuiString(m_pstr + iPos, iLength);
}

int jwq::CDuiString::Find(TCHAR ch, int iPos /*= 0*/) const
{
	ASSERT(iPos >= 0 && iPos <= GetLength());
	if (iPos != 0 && (iPos < 0 || iPos >= GetLength())) return -1;
	LPCTSTR p = _tcschr(m_pstr + iPos, ch);
	if (p == NULL) return -1;
	return (int)(p - m_pstr);
}

int jwq::CDuiString::Find(LPCTSTR pstrSub, int iPos /*= 0*/) const
{
	ASSERT(!::IsBadStringPtr(pstrSub, -1));
	ASSERT(iPos >= 0 && iPos <= GetLength());
	if (iPos != 0 && (iPos < 0 || iPos > GetLength())) return -1;
	LPCTSTR p = _tcsstr(m_pstr + iPos, pstrSub);
	if (p == NULL) return -1;
	return (int)(p - m_pstr);
}

int jwq::CDuiString::ReverseFind(TCHAR ch) const
{
	LPCTSTR p = _tcsrchr(m_pstr, ch);
	if (p == NULL) return -1;
	return (int)(p - m_pstr);
}

int jwq::CDuiString::Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo)
{
	CDuiString sTemp;
	int nCount = 0;
	int iPos = Find(pstrFrom);
	if (iPos < 0) return 0;
	int cchFrom = (int)_tcslen(pstrFrom);
	int cchTo = (int)_tcslen(pstrTo);
	while (iPos >= 0) {
		sTemp = Left(iPos);
		sTemp += pstrTo;
		sTemp += Mid(iPos + cchFrom);
		Assign(sTemp);
		iPos = Find(pstrFrom, iPos + cchTo);
		nCount++;
	}
	return nCount;
}

int jwq::CDuiString::Format(LPCTSTR pstrFormat, ...)
{
	int nRet;
	va_list Args;

	va_start(Args, pstrFormat);
	nRet = InnerFormat(pstrFormat, Args);
	va_end(Args);

	return nRet;

}

int jwq::CDuiString::SmallFormat(LPCTSTR pstrFormat, ...)
{
	CDuiString sFormat = pstrFormat;
	TCHAR szBuffer[64] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::_vsntprintf(szBuffer, sizeof(szBuffer), sFormat, argList);
	va_end(argList);
	Assign(szBuffer);
	return iRet;
}

int jwq::CDuiString::InnerFormat(LPCTSTR pstrFormat, va_list Args)
{
#if _MSC_VER <= 1400
	TCHAR* szBuffer = NULL;
	int size = 512, nLen, counts;
	szBuffer = (TCHAR*)malloc(size);
	ZeroMemory(szBuffer, size);
	while (TRUE) {
		counts = size / sizeof(TCHAR);
		nLen = _vsntprintf(szBuffer, counts, pstrFormat, Args);
		if (nLen != -1 && nLen < counts) {
			break;
		}
		if (nLen == -1) {
			size *= 2;
		}
		else {
			size += 1 * sizeof(TCHAR);
		}

		if ((szBuffer = (TCHAR*)realloc(szBuffer, size)) != NULL) {
			ZeroMemory(szBuffer, size);
		}
		else {
			break;
		}
	}

	Assign(szBuffer);
	free(szBuffer);
	return nLen;
#else
	int nLen, totalLen;
	TCHAR* szBuffer;
	nLen = _vsntprintf(NULL, 0, pstrFormat, Args);
	totalLen = (nLen + 1) * sizeof(TCHAR);
	szBuffer = (TCHAR*)malloc(totalLen);
	ZeroMemory(szBuffer, totalLen);
	nLen = _vsntprintf(szBuffer, nLen + 1, pstrFormat, Args);
	Assign(szBuffer);
	free(szBuffer);
	return nLen;

#endif
}
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
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	CloseHandle(hFile);
	return true;
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

jwq::CDuiString jwq::CFileHelper::GetFileExtendName(CDuiString strFileName)
{
	int intCharPosition;

	//Find "\\",Get The File's Name
	intCharPosition = strFileName.ReverseFind(L'\\');
	if (-1 != intCharPosition)
	{
		strFileName = strFileName.Right(strFileName.GetLength() - intCharPosition - 1);
	}
	//AfxMessageBox(strFileName);

	//Find ".",Get the file's extend name
	intCharPosition = strFileName.ReverseFind(L'.');
	if (-1 != intCharPosition)
	{
		strFileName = strFileName.Right(strFileName.GetLength() - intCharPosition - 1);
		return strFileName;
	}
	else
	{
		//No extend name
		return L"";
	}
}

jwq::CDuiString jwq::CFileHelper::GetFileNameByFileFullPath(CDuiString strFileFullPathName)
{
	int intCharPosition;

	//Find "\\",Get The File's Name
	intCharPosition = strFileFullPathName.ReverseFind(L'\\');

	if (-1 != intCharPosition)
	{
		strFileFullPathName = strFileFullPathName.Right(strFileFullPathName.GetLength() - intCharPosition - 1);
	}

	return strFileFullPathName;
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
 

bool jwq::CDirHelper::DeleteDirAndFileAll(std::wstring dir)
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
		if (!_tccmp(ffd.cFileName,L".") || !_tccmp(ffd.cFileName,L".."))
		{
			continue;
		}
		else
		{
			name.Append(dir.c_str());
			name.Append(_T("\\"));
			name.Append(ffd.cFileName);
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				DeleteDirAndFileAll(name.GetBuffer());
				success = RemoveDirectoryW(name.GetBuffer());
			}
			else
			{
				success = DeleteFile(name);
			}
			
			if (!success)
			{				 
				return success;
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	return success;
}

bool jwq::CDirHelper::DeleteNilDir(std::wstring dir)
{
	return RemoveDirectoryW(dir.c_str());
}

std::wstring jwq::CDirHelper::GetPathDir(std::wstring exePath)
{
	if (exePath.find(L'\\') == -1 && exePath.find(L'/') == -1) return L"";

	int pos = exePath.rfind(L'\\');
	if (pos == std::wstring::npos)
	{
		pos = exePath.rfind(L'/');
	}
	if (pos == std::wstring::npos) return L"";
	std::wstring dir = exePath.substr(0, pos);
	return dir;
}

bool jwq::CDirHelper::EnumDirFile(std::wstring dir, std::vector<std::wstring>& v)
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
		if (!_tccmp(ffd.cFileName, L".") || !_tccmp(ffd.cFileName, L".."))
		{
			continue;
		}
		else
		{
			name.Append(dir.c_str());
			name.Append(_T("\\"));
			name.Append(ffd.cFileName);
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				 
			}
			else
			{
				v.push_back(name.GetBuffer());
			}

			if (!success)
			{
				return success;
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	return success;
}
