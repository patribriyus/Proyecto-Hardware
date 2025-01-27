#include <string.h>
#include "..\inc\44b.h"
#include "..\inc\44blib.h"
#include "..\inc\def.h"
#include "..\inc\lcd.h"
#include "..\inc\lcdlib.h"

//SCR_XSIZE,SCR_YSIZE,LCD_XSIZE,LCD_YSIZE are defined in lcd.h

/*#define M5D(n) ((n) & 0x1fffff)

#define ARRAY_SIZE_MONO 	(SCR_XSIZE/8*SCR_YSIZE)
#define ARRAY_SIZE_G4   	(SCR_XSIZE/4*SCR_YSIZE)
#define ARRAY_SIZE_G16  	(SCR_XSIZE/2*SCR_YSIZE)
#define ARRAY_SIZE_COLOR 	(SCR_XSIZE/1*SCR_YSIZE)

#define HOZVAL			(LCD_XSIZE/4-1)
#define HOZVAL_COLOR		(LCD_XSIZE*3/8-1)
#define LINEVAL			(LCD_YSIZE-1)
#define MVAL			(13)
//#define CLKVAL_MONO 		(19)  //60Mhz, CLKVAL=19 ->78.6Hz
//#define CLKVAL_G4 		(19)  //60Mhz, CLKVAL=19 ->78.6Hz
//#define CLKVAL_G16 		(19)  //60Mhz, CLKVAL=19 ->78.6Hz
//#define CLKVAL_COLOR 		(19)  //60Mhz, CLKVAL=19 ->78.6Hz

#define CLKVAL_MONO 		(13)  //60Mhz, CLKVAL=19 ->78.6Hz
#define CLKVAL_G4 		(13)  //60Mhz, CLKVAL=19 ->78.6Hz
#define CLKVAL_G16 		(13)  //60Mhz, CLKVAL=19 ->78.6Hz
#define CLKVAL_COLOR 		(10)  //60Mhz, CLKVAL=19 ->78.6Hz*/



void LcdInit(int color);

unsigned int (*frameBuffer1)[SCR_XSIZE/32];
unsigned int (*frameBuffer4)[SCR_XSIZE/16];
unsigned int (*frameBuffer16)[SCR_XSIZE/8];
unsigned int (*frameBuffer256)[SCR_XSIZE/4];


#define MVAL_USED 0

void Lcd_Init(int depth)
{
    //320x240 1bit/1pixel LCD
    
    switch(depth)
    {
    case 1:    
	if((U32)frameBuffer1==0)
	{
	    frameBuffer1=(unsigned int (*)[SCR_XSIZE/32])malloc(ARRAY_SIZE_MONO); 
	}

	rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_MONO<<12);
	    // disable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON2=(LINEVAL)|(HOZVAL<<10)|(10<<21);  
	    //LINEBLANK=10 (without any calculation) 
	rLCDSADDR1= (0x0<<27) | ( ((U32)frameBuffer1>>22)<<21 ) | M5D((U32)frameBuffer1>>1);
	    // monochrome, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D( (((U32)frameBuffer1+(SCR_XSIZE*LCD_YSIZE/8))>>1) ) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/16) | ( ((SCR_XSIZE-LCD_XSIZE)/16)<<9 );

	rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_MONO<<12);
	    // enable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	break;

    case 4:
	if((U32)frameBuffer4==0)
	{
	    frameBuffer4=(unsigned int (*)[SCR_XSIZE/16])malloc(ARRAY_SIZE_G4); 
	}

	rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_G4<<12);
	    // disable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON2=(LINEVAL)|(HOZVAL<<10)|(10<<21);  
	    //LINEBLANK=10 (without any calculation) 
	rLCDSADDR1= (0x1<<27) | ( ((U32)frameBuffer4>>22)<<21 ) | M5D((U32)frameBuffer4>>1);
	    // 4-gray, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D((((U32)frameBuffer4+(SCR_XSIZE*LCD_YSIZE/4))>>1)) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/8) | ( ((SCR_XSIZE-LCD_XSIZE)/8)<<9 );

	//The following value has to be changed for better display.
	//Select 4 levels among 16 gray levels.

	rBLUELUT=0xfa40; 
	rDITHMODE=0x0;
	rDP1_2 =0xa5a5;      
	rDP4_7 =0xba5da65;
	rDP3_5 =0xa5a5f;
	rDP2_3 =0xd6b;
	rDP5_7 =0xeb7b5ed;
	rDP3_4 =0x7dbe;
	rDP4_5 =0x7ebdf;
	rDP6_7 =0x7fdfbfe;

	rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_G4<<12);
	    // enable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	break;

    case 16:
	if((U32)frameBuffer16==0)
	{
	    frameBuffer16=(unsigned int (*)[SCR_XSIZE/8])malloc(ARRAY_SIZE_G16); 
	}

	rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_G16<<12);
	    // disable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON2=(LINEVAL)|(HOZVAL<<10)|(10<<21);  
	    //LINEBLANK=10 (without any calculation) 
	rLCDSADDR1= (0x2<<27) | ( ((U32)frameBuffer16>>22)<<21 ) | M5D((U32)frameBuffer16>>1);
	    // 16-gray, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D((((U32)frameBuffer16+(SCR_XSIZE*LCD_YSIZE/2))>>1)) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/4) | ( ((SCR_XSIZE-LCD_XSIZE)/4)<<9 );

	//The following value has to be changed for better display.
	rDITHMODE=0x0;
	rDP1_2 =0xa5a5;      
	rDP4_7 =0xba5da65;
	rDP3_5 =0xa5a5f;
	rDP2_3 =0xd6b;
	rDP5_7 =0xeb7b5ed;
	rDP3_4 =0x7dbe;
	rDP4_5 =0x7ebdf;
	rDP6_7 =0x7fdfbfe;

	rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_G16<<12);
	    // enable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	break;

    case 256:
	if((U32)frameBuffer256==0)
	{
	    frameBuffer256=(unsigned int (*)[SCR_XSIZE/4])malloc(ARRAY_SIZE_COLOR); 
	}

	rLCDCON1=(0)|(2<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	    // disable,8B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON2=(LINEVAL)|(HOZVAL_COLOR<<10)|(10<<21);  
	    //LINEBLANK=10 (without any calculation) 
	rLCDSADDR1= (0x3<<27) | ( ((U32)frameBuffer256>>22)<<21 ) | M5D((U32)frameBuffer256>>1);
	    // 256-color, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D((((U32)frameBuffer256+(SCR_XSIZE*LCD_YSIZE))>>1)) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/2) | ( ((SCR_XSIZE-LCD_XSIZE)/2)<<9 );

	//The following value has to be changed for better display.

	rREDLUT  =0xfdb96420;
	rGREENLUT=0xfdb96420;
	rBLUELUT =0xfb40;

	rDITHMODE=0x0;
	rDP1_2 =0xa5a5;      
	rDP4_7 =0xba5da65;
	rDP3_5 =0xa5a5f;
	rDP2_3 =0xd6b;
	rDP5_7 =0xeb7b5ed;
	rDP3_4 =0x7dbe;
	rDP4_5 =0x7ebdf;
	rDP6_7 =0x7fdfbfe;

	rLCDCON1=(1)|(2<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	    // enable,8B_SNGL_SCAN,WDLY=8clk,WLH=8clk,

	break;

    default:
	break;
    }	
}



void Lcd_MoveViewPort(int vx,int vy,int depth)
{
    U32 addr;
    switch(depth)
    {
    case 1:
    	// LCDBASEU,LCDBASEL register has to be changed before 12 words before the end of VLINE.
    	// In mono mode, x=320 is 10 words, So, We can't change LCDBASEU,LCDBASEL 
    	// during LINECNT=1~0 at mono mode. 
    #if (LCD_XSIZE<512)
    	while((rLCDCON1>>22)<=1); // if x<512
    #else	
    	while((rLCDCON1>>22)==0); // if x>512 ((12+4)*32) 
    #endif
        addr=(U32)frameBuffer1+(vx/8)+vy*(SCR_XSIZE/8);
	rLCDSADDR1= (0x0<<27) | ( (addr>>22)<<21 ) | M5D(addr>>1);
	    // monochrome, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D( ((addr+(SCR_XSIZE*LCD_YSIZE/8))>>1) ) | (MVAL<<21);
       	break;

    case 4:
    #if (LCD_XSIZE<256)
    	while((rLCDCON1>>22)<=1); // if x<256
    #else	
    	while((rLCDCON1>>22)==0); // if x>256
    #endif
        addr=(U32)frameBuffer4+(vx/4)+vy*(SCR_XSIZE/4);
	rLCDSADDR1= (0x1<<27) | ( (addr>>22)<<21 ) | M5D(addr>>1);
	    // 4-gray, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D(((addr+(SCR_XSIZE*LCD_YSIZE/4))>>1)) | (MVAL<<21);

    	break;

    case 16:
    #if (LCD_XSIZE<128)
    	while((rLCDCON1>>22)<=1); // if x<128
    #else	
    	while((rLCDCON1>>22)==0); // if x>128
    #endif
        addr=(U32)frameBuffer16+(vx/2)+vy*(SCR_XSIZE/2);
	rLCDSADDR1= (0x2<<27) | ( (addr>>22)<<21 ) | M5D(addr>>1);
	    // 16-gray, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D(((addr+(SCR_XSIZE*LCD_YSIZE/2))>>1)) | (MVAL<<21);

    	break;

    case 256:
    #if (LCD_XSIZE<64)
    	while((rLCDCON1>>22)<=1); // if x<64
    #else	
    	while((rLCDCON1>>22)==0); // if x>64
    #endif
        addr=(U32)frameBuffer256+(vx/1)+vy*(SCR_XSIZE/1);
	rLCDSADDR1= (0x3<<27) | ( (addr>>22)<<21 ) | M5D(addr>>1);
	    // 256-color, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D(((addr+(SCR_XSIZE*LCD_YSIZE))>>1)) | (MVAL<<21);
    	break;
    }
}    
    
