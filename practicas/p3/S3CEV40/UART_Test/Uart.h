char Uart_IntGetkey(void);
void Error(void);
void Error(void)                 __attribute__ ((interrupt ("IRQ")));
void Exint2(void)                __attribute__ ((interrupt ("IRQ")));
void Test_BDMA(void);
void Test_Done(void);
void Test_Done(void)             __attribute__ ((interrupt ("IRQ")));
void Test_Uart0(void);
void Test_Uart0Fifo(void);
void Test_Uart1(void);
void Test_Uart1Fifo(void);
void Test_Uart1IrDA_Rx(void);
void Test_Uart1IrDA_Tx(void);
void Test_UartAFC_Rx(void);
void Test_UartAFC_Tx(void);
void U0AFC_RxErrorInt(void);
void U0AFC_RxErrorInt(void)      __attribute__ ((interrupt ("IRQ")));
void U0AFC_RxInt(void);
void U0AFC_RxInt(void)           __attribute__ ((interrupt ("IRQ")));
void U1AFC_TxInt(void);
void U1AFC_TxInt(void)           __attribute__ ((interrupt ("IRQ")));
void U1IrDA_RxErrorInt(void);
void U1IrDA_RxErrorInt(void)     __attribute__ ((interrupt ("IRQ")));
void U1IrDA_RxInt(void);
void U1IrDA_RxInt(void)          __attribute__ ((interrupt ("IRQ")));
void U1IrDA_TxInt(void); 
void U1IrDA_TxInt(void)          __attribute__ ((interrupt ("IRQ")));
void Uart0_RxFifoErrorInt(void);
void Uart0_RxFifoErrorInt(void)  __attribute__ ((interrupt ("IRQ")));
void Uart0_RxFifoInt(void);
void Uart0_RxFifoInt(void)       __attribute__ ((interrupt ("IRQ")));
void Uart0_RxInt(void);
void Uart0_RxInt(void)           __attribute__ ((interrupt ("IRQ")));
void Uart0_TxFifoInt(void);
void Uart0_TxFifoInt(void)       __attribute__ ((interrupt ("IRQ")));
void Uart0_TxInt(void);
void Uart0_TxInt(void)           __attribute__ ((interrupt ("IRQ")));
void Uart1_RxFifoErrorInt(void);
void Uart1_RxFifoErrorInt(void)  __attribute__ ((interrupt ("IRQ")));
void Uart1_RxFifoInt(void);
void Uart1_RxFifoInt(void)       __attribute__ ((interrupt ("IRQ")));
void Uart1_RxInt(void);
void Uart1_RxInt(void)           __attribute__ ((interrupt ("IRQ")));
void Uart1_TxFifoInt(void);
void Uart1_TxFifoInt(void)       __attribute__ ((interrupt ("IRQ")));
void Uart1_TxInt(void);
void Uart1_TxInt(void)           __attribute__ ((interrupt ("IRQ")));
