// cpulimit.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tlhelp32.h"

#pragma comment(linker, "/MERGE:.rdata=.text") 
#pragma comment(linker, "/MERGE:.data=.text") 
#pragma comment(linker, "/FILEALIGN:512 /SECTION:.text,ERW /IGNORE:4078") 
#pragma comment(linker, "/ENTRY:MyMain") 
#pragma comment(linker, "/NODEFAULTLIB") 
#pragma comment(linker, "/SUBSYSTEM:WINDOWS") 


typedef DWORD (WINAPI *extSetProcessWorkingSetSizex)(HANDLE, int,int);
typedef HANDLE (WINAPI *extOpenThreadx)(DWORD, BOOL, DWORD);
typedef DWORD (WINAPI *extResumeProcessx)(HANDLE);
typedef DWORD (WINAPI *extSuspendProcessx)(HANDLE);



extSetProcessWorkingSetSizex extSetProcessWorkingSetSize;
extOpenThreadx extOpenThread;
extResumeProcessx extResumeProcess;
extSuspendProcessx extSuspendProcess;

/////////////////////////////////////////////////////////
/////////////////////////////some RTL calls substitutions
char *__fastcall mystrstr(char *str1,char *str2)
{
if((!str1)||(!str2)||(!*str1)||(!*str2))return 0;
char *t1=str1;
while(*t1)
 {
 char *z1=t1,*z2=str2;
 while(*z1&&*z2&&(*z1==*z2)){z1++;z2++;}
 if(!*z2)return t1;
 t1++;
 }
return 0;
}
int __fastcall mystricmp(char *s1,char *s2)
{
switch(CompareString(LOCALE_USER_DEFAULT,NORM_IGNORECASE,s1,-1,s2,-1))
 {
 case CSTR_LESS_THAN: return -1;
 case CSTR_GREATER_THAN: return 1;
 case CSTR_EQUAL:return 0;
 }
return 2;
}

////////////////////////////////////////////////////
//////////////////////////////////////////////////go
void __fastcall GetDebugPriv()
{
HANDLE hToken;
TOKEN_PRIVILEGES tkp;
OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&tkp.Privileges[0].Luid);
tkp.PrivilegeCount = 1;
tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
}

HANDLE __fastcall ProcByExe(char *exe, DWORD &pid)
{
HANDLE prc=0;
HANDLE snp=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
if(snp!=INVALID_HANDLE_VALUE)
 {
 PROCESSENTRY32 pe={sizeof(PROCESSENTRY32),0};
 if(Process32First(snp,&pe))
  {
  do
   {
   char *t=pe.szExeFile+strlen(pe.szExeFile);
   while((t>pe.szExeFile)&&(*t!='\\'))t--;
   if(*t=='\\')t++;
   if(!mystricmp(t,exe))
    {
    prc=OpenProcess(PROCESS_ALL_ACCESS,0,pid=pe.th32ProcessID);
    break;
    }
   }while (Process32Next(snp,&pe));
  }
 CloseHandle(snp);
 }
return prc;
}
void __fastcall SuspendResumeIt(DWORD pid, bool suspend)
{
HANDLE snp=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
if(snp!=INVALID_HANDLE_VALUE)
 {
 THREADENTRY32 pe={sizeof(THREADENTRY32),0};
 if(Thread32First(snp,&pe))
  {
  do
   {
   if(pe.th32OwnerProcessID==pid)
    {
    HANDLE trd=extOpenThread(THREAD_SUSPEND_RESUME,0,pe.th32ThreadID);
    if(trd)
     {
     if(suspend)SuspendThread(trd);
           else ResumeThread(trd); 
     CloseHandle(trd);
     }
    } 
   }while (Thread32Next(snp,&pe));
  }
 CloseHandle(snp);
 }
}

void __fastcall CPULimitMain()
{
if(OpenMutex(MUTEX_ALL_ACCESS,0,"CPULimit_Activated_Mutex")){MessageBox(0,"CPULimit.exe already started!","CPULimit",MB_ICONWARNING);ExitProcess(-3);}
HANDLE amtx=CreateMutex(0,true,"CPULimit_Activated_Mutex");

extSetProcessWorkingSetSize=(extSetProcessWorkingSetSizex)GetProcAddress(LoadLibrary("kernel32.dll"),"SetProcessWorkingSetSize");
extOpenThread=(extOpenThreadx)GetProcAddress(LoadLibrary("kernel32.dll"),"OpenThread");
extResumeProcess=(extResumeProcessx)GetProcAddress(LoadLibrary("ntdll.dll"),"NtResumeProcess");
extSuspendProcess=(extSuspendProcessx)GetProcAddress(LoadLibrary("ntdll.dll"),"NtSuspendProcess");


char fpath[1100];
GetModuleFileName(0,fpath,1023);fpath[1023]=0;
char *tmp=fpath+strlen(fpath);
while(*tmp!='\\')tmp--;
tmp++;strcpy(tmp,"cpulimit.ini");

char exename[261];
GetPrivateProfileString("Settings","ExeName","",exename,260,fpath);exename[260]=0;
DWORD timeon=GetPrivateProfileInt("Settings","TimeOn",1000,fpath);
DWORD timeoff=GetPrivateProfileInt("Settings","TimeOff",1000,fpath);
DWORD hipr=GetPrivateProfileInt("Settings","HighPriority",0,fpath);
DWORD CanUseNtDll=GetPrivateProfileInt("Settings","CanUseNtDll",1,fpath);
if(exename[0]&&(timeon)&&(timeoff))
 {
 if(extOpenThread)
  {
  GetDebugPriv();
  if(hipr)SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
  if(extSetProcessWorkingSetSize)extSetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
  HANDLE prc=0;
  DWORD pid;
  while(!OpenMutex(MUTEX_ALL_ACCESS,0,"CPULimit_Deactivate_Mutex"))
   {
   if(prc&&(WaitForSingleObject(prc,0)!=WAIT_TIMEOUT))
    {
    CloseHandle(prc);
    prc=0;
    }
   if(!prc)prc=ProcByExe(exename,pid);
   if(prc)
    {
    if(extSuspendProcess&&CanUseNtDll)extSuspendProcess(prc);
                                 else SuspendResumeIt(pid, 1);
    Sleep(timeoff);
    if(extResumeProcess&&CanUseNtDll)extResumeProcess(prc);
                                else SuspendResumeIt(pid, 0);
    Sleep(timeon);
    }else Sleep(timeoff+timeon);
   }
  }else MessageBox(0,"Your system is not supported. Exiting.","CPU limit",MB_ICONWARNING);
 }else MessageBox(0,"CPUlimit.ini is not correct! Please check configuration file. Exiting.","CPU limit",MB_ICONWARNING);
CloseHandle(amtx);
}

void __fastcall HaltMich()
{
 HANDLE mtx;
 if((mtx=OpenMutex(MUTEX_ALL_ACCESS,0,"CPULimit_Activated_Mutex"))!=NULL)
  {
  CloseHandle(mtx);
  CreateMutex(0,0,"CPULimit_Deactivate_Mutex");
  for(;;)
   {
   mtx=OpenMutex(MUTEX_ALL_ACCESS,0,"CPULimit_Activated_Mutex");
   if(mtx){CloseHandle(mtx);Sleep(10);}else break;
   }
  }
}

/////////////////////////////////////////////////
//////////////////////////////////service support

int SvcStarted;
SERVICE_STATUS SvcStatus;
SERVICE_STATUS_HANDLE SvcHandle;
VOID WINAPI ServiceHandler(DWORD fdwControl)
{
if((fdwControl==SERVICE_CONTROL_STOP)||(fdwControl==SERVICE_CONTROL_SHUTDOWN))
 {
 SvcStatus.dwCurrentState=SERVICE_STOP_PENDING; 
 SetServiceStatus(SvcHandle,&SvcStatus);
 HaltMich(); 
 }
}

VOID WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv)
{
SvcHandle=RegisterServiceCtrlHandler("CPULimit",&ServiceHandler);
ZeroMemory(&SvcStatus,sizeof(SvcStatus));
SvcStatus.dwServiceType=SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
SvcStatus.dwControlsAccepted=SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
SvcStatus.dwCurrentState=SERVICE_RUNNING;SetServiceStatus(SvcHandle,&SvcStatus);
SvcStarted=1;
CPULimitMain();
SvcStatus.dwCurrentState=SERVICE_STOPPED;SetServiceStatus(SvcHandle,&SvcStatus);
}
SERVICE_TABLE_ENTRY ste[2]={{"CPULimit",&ServiceMain},{0,0}};


int WINAPI MyMain(void *)
{
if(mystrstr(GetCommandLine(),"/service"))
 {
 SvcStarted=0;
 StartServiceCtrlDispatcher(ste);
 while(!SvcStarted)Sleep(100);
 }else if(mystrstr(GetCommandLine(),"/q"))
 {
 HaltMich(); 
 }else if(mystrstr(GetCommandLine(),"/install"))
 {
 SC_HANDLE sch=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
 if(sch)
  {
  char fpath[1100];GetModuleFileName(0,fpath+1,1023);fpath[0]='\"';fpath[1023]=0;
  strcat(fpath,"\" /service");
  SC_HANDLE sh=CreateService(sch,"CPULimit","CPULimit",SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,fpath,0,0,0,0,0);
  if(sh)
   {
   CloseServiceHandle(sh);
   MessageBox(0,"CPULimit service installed. Start it using \"net start CPULimit\"","Install",MB_ICONINFORMATION);  
   }else MessageBox(0,"Can't create service","Install failed",MB_ICONERROR);  
  CloseServiceHandle(sch);
  }else MessageBox(0,"Can't open Service Control Manager","Install failed",MB_ICONERROR);
 }else if(mystrstr(GetCommandLine(),"/uninstall"))
 {
 SC_HANDLE sch=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
 if(sch)
  {
  SC_HANDLE sh=OpenService(sch,"CPULimit",SERVICE_ALL_ACCESS);
  if(sh)
   {
   SERVICE_STATUS ss;QueryServiceStatus(sh,&ss);
   SERVICE_STATUS st;st.dwCurrentState=SERVICE_STOP_PENDING; 
   ControlService(sh,SERVICE_CONTROL_STOP,&ss);
   while(st.dwCurrentState==SERVICE_STOP_PENDING){QueryServiceStatus(sh,&st);Sleep(100);}
   if(!DeleteService(sh))MessageBox(0,"Can't delete service","Uninstall failed",MB_ICONERROR);
                    else MessageBox(0,"CPULimit service uninstalled","Uninstall",MB_ICONINFORMATION);     
   }else MessageBox(0,"Can't open CPULimit service","Uninstall failed",MB_ICONERROR);
  }else MessageBox(0,"Can't open Service Control Manager","Uninstall failed",MB_ICONERROR);
 }else 
 {
 CPULimitMain(); 
 }

ExitProcess(0);
return 0;
}
//---------------------------------------------------------------------------

