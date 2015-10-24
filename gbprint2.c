#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "font8x8/font8x8_basic.h"
	
	int fd;
	char *p_buff;
	
	
	char buff[640];


	
	void setTile(int n ,char tile[])
	{
		int i;
		for(i=0;i<16;i++)
		{
			buff[i+n*16]=tile[i];
		}
	}
	
	char reverse (char v)
	{
		    // input bits to be reversed
		unsigned char r = v; // r will be reversed bits of v; first get LSB of v
		int s = sizeof(v) * 8 - 1; // extra shift needed at end

		for (v >>= 1; v; v >>= 1)
		{   
			r <<= 1;
			r |= v & 1;
			s--;
		}
		r <<= s; // shift when v's highest bits are zero
		
		return r;
	}
	
	char  *getTile (char c1)
	{
		int c = c1;
		static char result[16];
		
		
			result[0]   = reverse(font8x8_basic[c][0]);
			result[1]   = reverse(font8x8_basic[c][0]);
			result[2]   = reverse(font8x8_basic[c][1]);
			result[3]   = reverse(font8x8_basic[c][1]);
			result[4]   = reverse(font8x8_basic[c][2]);
			result[5]   = reverse(font8x8_basic[c][2]);
			result[6]   = reverse(font8x8_basic[c][3]);
			result[7]   = reverse(font8x8_basic[c][3]);
			
			result[8]   = reverse(font8x8_basic[c][4]);
			result[9]   = reverse(font8x8_basic[c][4]);
			result[10]   = reverse(font8x8_basic[c][5]);
			result[11]   = reverse(font8x8_basic[c][5]);
			result[12]   = reverse(font8x8_basic[c][6]);
			result[13]   = reverse(font8x8_basic[c][6]);
			result[14]   = reverse(font8x8_basic[c][7]);
			result[15]   = reverse(font8x8_basic[c][7]);
			
		return result;
	}
	
	void waitforChar(char c)
	{
		char cmd;
		while(cmd!=c)
		{
			cmd=serialGetchar(fd);	
		}
	}
	
	void sendExposure()
	{
		serialPutchar(fd,0x01);
		serialPutchar(fd,0x00);
		serialPutchar(fd,0xE4);
		serialPutchar(fd,0x40);
	}
	
	void print(char buf[640])
	{
		serialPutchar(fd,'!');
		printf("ISSUED ! Command \n");
		waitforChar('a');
		printf("ack 1 \n");

		int i,j;
		for(i=0;i<10;i++)
		{
			printf("sending block %d \n",i+1);
			for(j=0;j<64;j++)
			{
				serialPutchar(fd,buf[i*64+j]);
				printf("%c",buf[i*64+j]);
			}
			
			if(i<9)
			{
				waitforChar('a');
				printf("block %d accepted \n",i+1);
			}
		}
		printf("blocks sent, waiting for 1 \n");
		waitforChar('1');
		sendExposure();
		printf("Exposure sent waiting for P \n");
		
		waitforChar('P');
		printf("Waiting for O \n");
		waitforChar('O');
		printf("donePrinting");
	}
	
	
	void printString(char *str, int mode)
		{
			int i;
			printf("prtStr \n");
			
			
			for(i=0;i< 40 ;i++)			
			{
				printf("sTile ");
				setTile(i,getTile(str[i]));
			}
	    	
			
			char buffInv[640];
			char cx;
			for(i=0;i<640;i++)
			{
				cx = buff[i];
				buffInv[i]= ~cx;
			}
			
			if(mode==0)
			{
			print(buff);
			}
			else
			print(buffInv);
		}
		
	int main(int argc, char **args)
	{
		
		
		
		
		if((fd=serialOpen ("/dev/ttyAMA0",57600))<0)
		{
			printf("ERR_1");
			return 1;
		}	
		
		if(wiringPiSetup() == -1)
		{
			printf("ERR_2");
		}
		
		
		serialPutchar(fd,'?');
		printf("seeking GB Printer! \n");
		waitforChar('1');
		printf("found gbPrinter! \n");
		
		
		//print(buff);
		//printString("HALLO WELT                                          ");
		if(strcmp(args[2],"NEG") ==0)
		{
		 printString(args[1],1);
		} else
		
		printString(args[1],0);
		
		return 0;
	}
	
