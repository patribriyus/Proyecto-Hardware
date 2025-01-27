/*********************************************************************************************
* File��	keyboard.c
* Author:	embest
* Desc��	keyboard source code
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "keyboard.h"


/*--- global variables ---*/

/* keyboard control address */
volatile UCHAR *keyboard_base = (UCHAR *)0x06000000;

/*--- function declare ---*/

void init_keyboard();
void close_keyboard();
void KeyboardInt(void) __attribute__ ((interrupt ("IRQ")));

void Eint4567Isr(void) __attribute__ ((interrupt ("IRQ")));

/*--- function code ---*/

/*********************************************************************************************
* name:		Test_Keyboard
* func:		test keyboard
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void Test_Keyboard()
{
	// init keyboard
	init_keyboard();
	Uart_Printf("\nPlease press one key on keyboard and look at LED ...\n");
	// close keyboard
	// close_keyboard();
}

/*********************************************************************************************
* name:		init_keyboard
* func:		init keyboard interrupt
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void init_keyboard()
{
	/* enable interrupt */
	rINTMOD = 0x0;
	rINTCON = 0x1;
		
	/* set EINT1 interrupt handler */
	rINTMSK =~(BIT_GLOBAL|BIT_EINT1|BIT_EINT4567);
    pISR_EINT1 = (int)KeyboardInt;
    pISR_EINT4567 = (int)Eint4567Isr;
    
    /* PORT G */
    rPCONG  = 0xffff;					// EINT7~0
	rPUPG   = 0x0;						// pull up enable	    
	rEXTINT = rEXTINT|0x20;				// EINT1 falling edge mode
	
	rI_ISPC = BIT_EINT1|BIT_EINT4567;	// clear pending bit
	rEXTINTPND = 0xf;					// clear EXTINTPND reg
}

/*********************************************************************************************
* name:		close_keyboard
* func:		close keyboard interrupt
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void close_keyboard()
{
	pISR_EINT1 = NULL;
	pISR_EINT4567 = NULL;
	/* Mask interrupt */
	rINTMSK = rINTMSK | BIT_EINT1 | BIT_EINT4567;
}

/*********************************************************************************************
* name:		KeyboardInt
* func:		keyboard interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void KeyboardInt(void)
{
	int value;
	rI_ISPC = BIT_EINT1;			// clear pending bit
	
	value = key_read();
	if(value > -1)
	 {
		Digit_Led_Symbol(value);
        Uart_Printf("Key is:%x \r",value);
     }

}

/*********************************************************************************************
* name:		key_read
* func:		read key value
* para:		none
* ret:		key value, -1 -- error
* modify:
* comment:		
********************************************************************************************/
inline int key_read()
{
	int value;
	char temp;
	
	/* read line 1 */
	temp = *(keyboard_base+0xfd);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 3;
		else if( (temp&0x2) == 0 )
			value = 2;
		else if( (temp&0x4) == 0 )
			value = 1;
		else if( (temp&0x8) == 0 )
			value = 0;
		return value;
	}
	
	/* read line 2 */
	temp = *(keyboard_base+0xfb);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 7;
		else if( (temp&0x2) == 0 )
			value = 6;
		else if( (temp&0x4) == 0 )
			value = 5;
		else if( (temp&0x8) == 0 )
			value = 4;
		return value;
	}
	
	/* read line 3 */
	temp = *(keyboard_base+0xf7);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xb;
		else if( (temp&0x2) == 0 )
			value = 0xa;
		else if( (temp&0x4) == 0 )
			value = 9;
		else if( (temp&0x8) == 0 )
			value = 8;
		return value;
	}
	
	/* read line 4 */
	temp = *(keyboard_base+0xef);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xf;
		else if( (temp&0x2) == 0 )
			value = 0xe;
		else if( (temp&0x4) == 0 )
			value = 0xd;
		else if( (temp&0x8) == 0 )
			value = 0xc;
		return value;
	}
	
	return -1;
}

/*********************************************************************************************
* name:		Eint4567Isr
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Eint4567Isr(void)
{
	unsigned char which_int;

	Uart_Printf("\n SB2 or SB3 had pressed and light LED.");
    which_int=rEXTINTPND;
    rEXTINTPND=0xf;				//clear EXTINTPND reg.		
    rI_ISPC=BIT_EINT4567;		//clear pending_bit
    
    if(which_int == 4)			// SB2
    {
    	Led_Display(0x0);	
    	Led_Display(0x1);	
    }
    else if(which_int == 8)		// SB3
    {
    	Led_Display(0x0);	
    	Led_Display(0x2);	
    }
}