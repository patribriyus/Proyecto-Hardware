/*********************************************************************************************
* File��	main.c
* Author:	embest
* Desc��	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"

/*--- function declare ---*/
int main(void);

/*--- extern function ---*/
extern void Test_Flash(void);

/*--- function code ---*/
/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
int Main(void)
{
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
	/******************/
	/* user interface */
	/******************/
	Uart_Printf("\n\rEmbest 44B0X Evaluation Board(S3CEV40)");
	Uart_Printf("\n\rFlash(SST39VF160-90) Program Test\n");
	Test_Flash();
    
    return;
}

