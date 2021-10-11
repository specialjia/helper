
/*
* Copyright (c) 2017 rxi
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "log.h"

#include <string>


#include "../../../jwqHelper/helper/helper/jwqHelper.h"


static struct {
	void *udata;
	log_LockFn lock;
	FILE *fp;
	int level;
	int quiet;
} LogM;


void  DUI__Trace(LPCTSTR pstrFormat, ...)
{
#ifdef _DEBUG
	TCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	wcscat_s(szBuffer, _T("\n"));
	va_end(args);
	::OutputDebugString(szBuffer);
#endif
}

void logInit()
{
	std::wstring strPathW = jwq::CPathHelper::GetDeskPath();
	strPathW.append(L"\\jwq.log");
	std::string strPathA = jwq::CStringHelper::WstringToAnsi(strPathW);
	
	FILE* fp = fopen(strPathA.c_str(), "at+");
	if (fp == nullptr)
	{
		MessageBox(NULL, _TEXT("create log file failed !"), _TEXT("tip"), 0);
	}
	log_set_fp(fp);
	log_set_quiet(true);
	

}

static const char *level_names[] = {
	("TRACE"), ("DEBUG"), ("INFO"), ("WARN"), ("ERROR"), ("FATAL")
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
	"\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


static void lock(void)   {
	if (LogM.lock) {
		LogM.lock(LogM.udata, 1);
	}
}


static void unlock(void) {
	if (LogM.lock) {
		LogM.lock(LogM.udata, 0);
	}
}


void log_set_udata(void *udata) {
	LogM.udata = udata;
}


void log_set_lock(log_LockFn fn) {
	LogM.lock = fn;
}


void log_set_fp(FILE *fp) {
	LogM.fp = fp;
}


void log_set_level(int level) {
	LogM.level = level;
}


void log_set_quiet(int enable) {
	LogM.quiet = enable ? 1 : 0;
}



void log_log(int level, const char* file, int line, const char* fmt, ...) {
	//logInit();
	if (level < LogM.level) {
		return;
	}

	/* Acquire lock */
	lock();

	/* Get current time */
	time_t t = time(NULL);
	struct tm* lt = localtime(&t);

	/* Log to stderr */
	if (!LogM.quiet) {
		va_list args;
		char buf[16];
		buf[strftime(buf, sizeof(buf), ("%H:%M:%S"), lt)] = '\0';
#ifdef LOG_USE_COLOR
		fprintf(
			stderr, ("%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m "),
			buf, level_colors[level], level_names[level], file, line);
#else
		_ftprintf(stderr, _T("%s %-5s %s:%d: "), buf, level_names[level], file, line);
#endif
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
		fprintf(stderr, ("\n"));
		fflush(stderr);
	}

	/* Log to file */
	if (LogM.fp) {
		va_list args;
		char buf[32];
		buf[strftime(buf, sizeof(buf), ("%Y-%m-%d %H:%M:%S"), lt)] = '\0';
		fprintf(LogM.fp, ("%s %-5s %s:%d: "), buf, level_names[level], file, line);
		va_start(args, fmt);
		vfprintf(LogM.fp, fmt, args); //_vftprintf_s 如果fmt中有 %1 这种字符串,会崩溃.
		va_end(args);
		fprintf(LogM.fp, ("\n"));
		fflush(LogM.fp);
	}

	/* Release lock */
	unlock();
}