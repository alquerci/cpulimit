// cpulimit.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"


int main(int argc, TCHAR* argv[])
{
	if(mystrstr(GetCommandLine(), L"/service"))
	{
		ServiceStart();
	}
	else if(mystrstr(GetCommandLine(), L"/q"))
	{
		HaltMich(); 
	}
	else if(mystrstr(GetCommandLine(), L"/install"))
	{
		SC_HANDLE sch = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
		if(sch)
		{
			wchar_t fpath[1100] = {0};
			rsize_t fpathsize = 1100;			

			GetModuleFileName(0, fpath+1, 1023);
			fpath[0] = '\"';
			fpath[1023] = 0;
			
			wcscat_s(fpath, fpathsize,L"\" /service");
			SC_HANDLE sh = CreateService(sch, L"CPULimit", L"CPULimit", SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, fpath, 0, 0, 0, 0, 0);
			if(sh)
			{
				CloseServiceHandle(sh);
				MessageBox(0, L"CPULimit service installed. Start it using \"net start CPULimit\"", L"Install", MB_ICONINFORMATION);  
			}
			else
			{
				MessageBox(0, L"Can't create service", L"Install failed", MB_ICONERROR);  
			}
			CloseServiceHandle(sch);
		}
		else
		{
			MessageBox(0, L"Can't open Service Control Manager", L"Install failed", MB_ICONERROR);
		}
	}
	else if(mystrstr(GetCommandLine(), L"/uninstall"))
	{
		SC_HANDLE sch=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
		if(sch)
		{
			SC_HANDLE sh = OpenService(sch, L"CPULimit", SERVICE_ALL_ACCESS);
			if(sh)
			{
				SERVICE_STATUS ss;
				QueryServiceStatus(sh, &ss);
				SERVICE_STATUS st;
				st.dwCurrentState = SERVICE_STOP_PENDING; 
				ControlService(sh, SERVICE_CONTROL_STOP, &ss);
				while(st.dwCurrentState == SERVICE_STOP_PENDING)
				{
					QueryServiceStatus(sh, &st);
					Sleep(100);
				}

				if(!DeleteService(sh))
				{
					MessageBox(0, L"Can't delete service", L"Uninstall failed", MB_ICONERROR);
				}
				else
				{
					MessageBox(0, L"CPULimit service uninstalled", L"Uninstall", MB_ICONINFORMATION);     
				}
			}
			else
			{
				MessageBox(0, L"Can't open CPULimit service", L"Uninstall failed", MB_ICONERROR);
			}
		}
		else
		{
			MessageBox(0, L"Can't open Service Control Manager", L"Uninstall failed", MB_ICONERROR);
		}
	}
	else 
	{
		CPULimitMain(); 
	}

	ExitProcess(0);
}


