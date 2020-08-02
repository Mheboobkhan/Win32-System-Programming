#include <windows.h>
#include <stdio.h>

BOOL Attrb_change(HANDLE F_name,DWORD F_Switch,  DWORD C_Day, DWORD C_Month, DWORD C_Year )
{
    SYSTEMTIME Sys_Time;
    GetSystemTime(&Sys_Time);
    Sys_Time.wDay = C_Day;
    Sys_Time.wMonth = C_Month;
    Sys_Time.wYear = C_Year;

    FILETIME F_Time;
    SystemTimeToFileTime(&Sys_Time,&F_Time);

    if (F_Switch == 0) return SetFileTime(F_name,&F_Time,NULL,NULL);
    if (F_Switch == 1) return SetFileTime(F_name,NULL,&F_Time,NULL);
    if (F_Switch == 2) return SetFileTime(F_name,NULL,NULL,&F_Time);
    else return 0; 
    
    
}

int main(int argc, LPCTSTR argv[])
{
    if (argc !=6)
    {
    printf ("Usage : <File Name> <Switch> <Day> <Month> <Year> \n 0 :: Change Created Date \n 1 :: Change Modified Date \n 2 :: change Last Access Date \n Date Formate :: DD<space>MM<space>YYYY\n");
    return 1;
    }
    HANDLE hFile = CreateFile (argv[1],GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,NULL);
    if (Attrb_change(hFile,atoi (argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]))) printf ("Changes are Implemented!!! :)");
    if (!Attrb_change(hFile,atoi (argv[2]),atoi(argv[3]),atoi(argv[4]), atoi(argv[5]))) printf ("Fatal Error !!! :(%d \n",GetLastError());
    CloseHandle (hFile);
    return 0;
   

}