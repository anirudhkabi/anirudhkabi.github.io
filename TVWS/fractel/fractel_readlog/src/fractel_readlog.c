#include<stdio.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	FILE *fdproc, *fduser;	
	char buff[255];
	int bytesread=0;
	int status = 0;

	status = system("route add -host 182.155.121.11 gw 192.168.0.2 ath0");
	printf("status is %d\n", status);
	
	if (argc==2)
	{
		if ((fduser=fopen(argv[1], "w")) == NULL) {
			printf("Could not open output file\n");
		}

	}
	else
	{
		if ((fduser=stdout))//open("log_fractel.log", O_WRONLY | O_CREAT | O_TRUNC ))==-1) {
		{
			printf("Could not open output file\n");
		}
	}
	while (1)
	{
		if ((fdproc=fopen("/proc/net/madwifi/ath0/fractel_log", "r"))==NULL) {
			printf("Could not open input file\n");
			break;
		}
		if ((bytesread=fread(buff, 1, 255, fdproc))!=0)
		{
			//printf("%s\n",buff);			
			if (fwrite(buff, 1, bytesread, fduser)==0)
			{
				printf("Problem while writing\n");
				break;
			}
		}
		else
		{	
			sleep(1);
			//printf("no data : %s\n",buff);			
			//break;
		}
		fclose(fdproc);
	}
	
	fclose(fduser);
	return 0;
}
