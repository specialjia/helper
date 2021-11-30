#pragma once
#include <atlstr.h>
#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <windows.h>


namespace std
{
	class thread;
}

namespace jwq
{
class  CDuiString
{
public:
	enum { MAX_LOCAL_STRING_LEN = 63 };

	CDuiString();
	CDuiString(const TCHAR ch);
	CDuiString(const CDuiString& src);
	CDuiString(LPCTSTR lpsz, int nLen = -1);
	~CDuiString();

	void Empty();
	int GetLength() const;
	bool IsEmpty() const;
	TCHAR GetAt(int nIndex) const;
	void Append(LPCTSTR pstr);
	void Assign(LPCTSTR pstr, int nLength = -1);
	LPCTSTR GetData() const;

	void SetAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;

	TCHAR operator[] (int nIndex) const;
	const CDuiString& operator=(const CDuiString& src);
	const CDuiString& operator=(const TCHAR ch);
	const CDuiString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
	const CDuiString& operator=(LPCSTR lpStr);
	const CDuiString& operator+=(LPCSTR lpStr);
#else
	const CDuiString& operator=(LPCWSTR lpwStr);
	const CDuiString& operator+=(LPCWSTR lpwStr);
#endif
	CDuiString operator+(const CDuiString& src) const;
	CDuiString operator+(LPCTSTR pstr) const;
	const CDuiString& operator+=(const CDuiString& src);
	const CDuiString& operator+=(LPCTSTR pstr);
	const CDuiString& operator+=(const TCHAR ch);

	bool operator == (LPCTSTR str) const;
	bool operator != (LPCTSTR str) const;
	bool operator <= (LPCTSTR str) const;
	bool operator <  (LPCTSTR str) const;
	bool operator >= (LPCTSTR str) const;
	bool operator >  (LPCTSTR str) const;

	int Compare(LPCTSTR pstr) const;
	int CompareNoCase(LPCTSTR pstr) const;

	void MakeUpper();
	void MakeLower();

	CDuiString Left(int nLength) const;
	CDuiString Mid(int iPos, int nLength = -1) const;
	CDuiString Right(int nLength) const;

	int Find(TCHAR ch, int iPos = 0) const;
	int Find(LPCTSTR pstr, int iPos = 0) const;
	int ReverseFind(TCHAR ch) const;
	int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

	int __cdecl Format(LPCTSTR pstrFormat, ...);
	int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

protected:
	int __cdecl InnerFormat(LPCTSTR pstrFormat, va_list Args);

protected:
	LPTSTR m_pstr;
	TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
};
	
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
		static CDuiString GetFileExtendName(CDuiString strFileName);
		static CDuiString GetFileNameByFileFullPath(CDuiString strFileFullPathName);
		static CDuiString GetCurrentExeFileNameWithExtendName();
		static CDuiString GetCurrentExeFileNameWithoutExtendName();
		static CDuiString GetCurrentExeFileFullPath();
		static CDuiString GetCurrentExeFileFullPathName();
		static std::wstring GetVersion();
	private:
		static bool CreateFileAlways(const WCHAR* path);
	};


	class CStringHelper
	{
	public:
		static std::vector<std::wstring> splite(std::wstring& str, std::wstring& strSeparator);
		static std::string w2a(std::wstring strW);
		static std::string w2u(std::wstring strW);
		static std::wstring u2w(std::string strUtf8);
		static std::wstring a2w(std::string strAnsi);
		static std::string a2u(std::string strAnsi);
		static  std::string u2a(std::string strUtf8);

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
		static bool DeleteNilDir(std::wstring dir);
		static std::wstring GetPathDir(std::wstring exePath);
		static bool EnumDirFile(std::wstring dir,std::vector<std::wstring> &v);
	};

	class CPrinterHelper {
	public:
		static std::vector<CString>  GetPrinterList();
	};


	#define  THREADPOOL_MAX_NUM 16
	class CThreadHelper
	{
		using Task = std::function<void()>;   //��������
		std::vector<std::thread> m_pool;   //�̳߳�
		std::queue<Task> m_tasks;   //�������
		std::mutex m_lock;			//ͬ��
		std::condition_variable m_task_cv;	//��������
		std::atomic<bool>	m_run{ true }; //�̳߳��Ƿ�ִ��
		std::atomic<int>	m_idlThrNum{ 0 };	//�����߳�����
	public:
		inline CThreadHelper(unsigned short size = 4) { addThread(size); }
		inline ~CThreadHelper()
		{
			m_run = false;
			m_task_cv.notify_all();		//���������߳�ִ��
			for (std::thread& thread: m_pool)
			{
				if(thread.joinable())
				{
					thread.join();  //�ȴ����������ǰ�᣺�߳�һ����ִ����
				}
			}
		}

	public:
		//�ύһ������
		//����.get()��ȡ����ֵ�ȴ�����ִ���꣬��ȡ����ֵ
		//�����ַ�������ʵ�ֵ������Ա��
		//һ����ʹ�� bind: .commit(std::bind(&Dog::sayHello,&dog));
		//һ���� mem_fn:		.commit(std::men_fn(&Dog::sayHello), this)
		template<class F, class... Args>
		auto commit(F& f,Args&&... args) -> std::future<decltype(f(args...))>
		{
			if (!m_run)
				throw std::runtime_error("coomit on THreadPool is stopped");

			using RetType = decltype(f(args...));
			auto task = std::make_shared<std::packaged_task<RetType()>>(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);  //�Ѳ�����ڼ�������������󶨣�
			std::future<RetType> future = task->get_future();
			{
				//������񵽶���
				std::lock_guard<std::mutex> lock{ m_lock }; //�Ե�ǰ��������� lock_guard ��mutext��stack��װ�࣬�����ʱ��lock(),������ʱ��unlock()
				m_tasks.emplace([task]()
				{
					(*task)();
				});
			}
#ifdef THREADPOOL_ATUO_GROW
			if (m_idlThrNum < 1 && m_pool.size() < THREADPOOL_MAX_NUM)
				addThread(1);
#endif
			m_task_cv.notify_one(); //����һ���߳�ִ��

			return future;
		}

		//�����߳�����
		int idlCount() { return m_idlThrNum; }
		int thrCount() { return m_pool.size(); }
#ifndef THREADPOOL_AUTO_GROW
	private:
#endif
		//���ִ���������߳�
		void addThread(unsigned short size)
		{
			//����߳��������������� Ԥ�������� THREADPOOL_MAX_NUM
			for(;m_pool.size() < THREADPOOL_MAX_NUM && size > 0; --size)
			{
				m_pool.emplace_back([this]
				{
					while(m_run)
					{
						Task task;  //��ȡһ����ִ�е� task
						{
							//unniqwue_lock ��� lock_guard �ĺô��� ��������hi unlock() �� lock()
							std::unique_lock<std::mutex> lock{ m_lock };
							m_task_cv.wait(lock, [this]
							{
								return !m_run || !m_tasks.empty();
							});  //wait ֪����task
							if (!m_run && m_tasks.empty())
								return;
							task = move(m_tasks.front());  // ���Ƚ��ȳ��Ӷ���ȥһ��task
							m_tasks.pop();
						}
						m_idlThrNum--;
						task();
						m_idlThrNum++;
					}
				});
				m_idlThrNum++;
			}
		}

	};
	
}

	

