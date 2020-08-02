 #include <stdio.h> 
#include <errno.h>
#define Buff_Size 256 
int main (int argc, char *argv[])
{
    FILE *InFile ,*OutFile ;
    char rec[Buff_Size];
    size_t bytes_in,bytes_out;

    if (argc !=3)
    {
        printf("Usage: filecopy.exe <file1> <file2>");
        return 1;
    }
    
    InFile = fopen(argv[1],"rb");
    if (InFile == NULL)
    {
        perror(argv[1]);
        return 2;
    }
     OutFile = fopen(argv[2],"wb");
     if (OutFile == NULL)
     {
         perror(argv[2]);
         return 3;
     }

     while(bytes_in = fread(rec,1,Buff_Size,InFile)>0)
     {
         bytes_out =fwrite(rec,1,Buff_Size,OutFile);
         if (bytes_out != bytes_in)
         {
           /*perror("Fatal Writing error!!!"); */
             return 4;
         }
     }
     fclose(InFile);
     fclose(OutFile);
     return 0;
}