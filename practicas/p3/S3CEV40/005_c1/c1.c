/*********************************************************************************************
* File��	c1_a.c
* Author:	embest	
* Desc��	
* History:	
*********************************************************************************************/
void delay(int times);

//*----------------------------------------------------------------------------
//* Function Name       : _start
//* Input Parameters    : none
//* Output Parameters   : none
//*----------------------------------------------------------------------------
_start()
{
	int i=5;
	
	for(;;)
	{
		delay(i);
	}
}

//*----------------------------------------------------------------------------
//* Function Name       : delay
//* Input Parameters    : times
//* Output Parameters   : none
//*----------------------------------------------------------------------------
void delay(times)
{
	int i, j=0;
	
	for(i=0; i<times; i++)
	{
		for(j=0; j<10; j++)
		{
		}
	}

}
