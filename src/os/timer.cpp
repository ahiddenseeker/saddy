#include <os/timer.h>
#include <log/log.h>

#ifdef WIN32

 
void os::put_last_error() {
	DWORD errCode = GetLastError();
    char *err;
    if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL,
                       errCode,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                       (LPTSTR) &err,
                       0,
                       NULL))
	{
		SL_CRITICAL("Can't format error message!");
	}

	SL_CRITICAL(err);
    LocalFree(err);
}

#endif

os::timer::timer()
{
#ifdef WIN32

	typedef LONG (__stdcall  *PTR) (ULONG,BOOLEAN,PULONG);

	HMODULE hDll = LoadLibrary("ntdll.dll");

	PTR lpfnDLLProc = (PTR)GetProcAddress (hDll, "NtSetTimerResolution");
	ULONG DesiredResolution = 50000; // 1ms
	BOOLEAN SetResolution = TRUE;
	ULONG CurrentResolution = 0 ;
	LONG ret = lpfnDLLProc(DesiredResolution, SetResolution,&CurrentResolution);

	if(!QueryPerformanceFrequency(&m_frequency))
	{
		os::put_last_error();
	}
#else
		clock_getres(SADDY_USED_CLOCK_TYPE, &m_frequency);
#endif
		start();
}


void os::timer::start()
{
#ifdef WIN32
	BOOL result = QueryPerformanceCounter(&m_start); 
	if (!result)
	{
		os::put_last_error();
	}
#else
		clock_gettime(SADDY_USED_CLOCK_TYPE, &m_start);
#endif
}

void os::timer::stop()
{
#ifdef WIN32
	BOOL result = QueryPerformanceCounter(&m_end); 
	if (!result)
	{
		os::put_last_error();
	}
#else
		clock_gettime(SADDY_USED_CLOCK_TYPE, &m_end);
#endif
}



double os::timer::elapsed() const
{
#ifdef WIN32
	QueryPerformanceFrequency(&(const_cast<os::timer*>(this)->m_frequency));
	double millisecondsofhpc = (m_end.QuadPart - m_start.QuadPart) * 1000.0 / m_frequency.QuadPart;
	return millisecondsofhpc;
#else
	double starttime = m_start.tv_sec * 1.0E+3 + m_start.tv_nsec * 1.0E-6;
	double endtime = m_end.tv_sec * 1.0E+3 + m_end.tv_nsec * 1.0E-6;
	return endtime - starttime;
#endif
}