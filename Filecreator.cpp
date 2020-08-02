#include <stdio.h>
#include <errno.h>
#define Size_Buff 256
int main (int argc,char *argv[])
{
    printf("Usage: Filecreator.exe <output_file_name>");
    char buffer[] = { 'x' , 'y' , 'z' };
    FILE *outfile;
    outfile = fopen(argv[1],"wb");
    fwrite(buffer,1,Size_Buff, outfile );
    fclose(outfile);
    return 0;

}