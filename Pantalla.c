
#include <built_in.h>

unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_DataPort_B0 at GPIOA_ODR.B9;
sbit TFT_DataPort_B1 at GPIOC_ODR.B7;
sbit TFT_DataPort_B2 at GPIOA_ODR.B10;
sbit TFT_DataPort_B3 at GPIOB_ODR.B3;
sbit TFT_DataPort_B4 at GPIOB_ODR.B5;
sbit TFT_DataPort_B5 at GPIOB_ODR.B4;
sbit TFT_DataPort_B6 at GPIOB_ODR.B10;
sbit TFT_DataPort_B7 at GPIOA_ODR.B8;

sbit TFT_RST at GPIOC_ODR.B1;
sbit TFT_CS at GPIOB_ODR.B0;
sbit TFT_RS at GPIOA_ODR.B4;
sbit TFT_WR at GPIOA_ODR.B1;
sbit TFT_RD at GPIOA_ODR.B0;

void TFT_Set_Index_Custom(unsigned short index) {
unsigned int temp;
  TFT_RS = 0;

  // Write to port
  //temp = TFT_DataPort;
  //temp &= 0xFF00;
//  TFT_DataPort = index | temp;
  temp = index;
  TFT_DataPort_B0 = ((temp & 0x01) >> 0 );
  TFT_DataPort_B1 = ((temp & 0x02) >> 1 );
  TFT_DataPort_B2 = ((temp & 0x04) >> 2 );
  TFT_DataPort_B3 = ((temp & 0x08) >> 3 );
  TFT_DataPort_B4 = ((temp & 0x10) >> 4 );
  TFT_DataPort_B5 = ((temp & 0x20) >> 5 );
  TFT_DataPort_B6 = ((temp & 0x40) >> 6 );
  TFT_DataPort_B7 = ((temp & 0x80) >> 7 );
  // Strobe
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;
}

void TFT_Write_Command_Custom(unsigned short cmd) {
unsigned int temp;
  TFT_RS = 1;

  // Write to port
  //temp = TFT_DataPort;
  //temp &= 0xFF00;
  //TFT_DataPort = cmd | temp;
  temp = cmd;
  TFT_DataPort_B0 = ((temp & 0x01) >> 0 );
  TFT_DataPort_B1 = ((temp & 0x02) >> 1 );
  TFT_DataPort_B2 = ((temp & 0x04) >> 2 );
  TFT_DataPort_B3 = ((temp & 0x08) >> 3 );
  TFT_DataPort_B4 = ((temp & 0x10) >> 4 );
  TFT_DataPort_B5 = ((temp & 0x20) >> 5 );
  TFT_DataPort_B6 = ((temp & 0x40) >> 6 );
  TFT_DataPort_B7 = ((temp & 0x80) >> 7 );
  // Strobe
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;
}

void TFT_Write_Data_Custom(unsigned int _data) {
unsigned int temp;
  TFT_RS = 1;

  // Write to port
  //temp = TFT_DataPort;
  //temp &= 0xFF00;
  //TFT_DataPort = Hi(_data) | temp;
  temp = Hi(_data);
  TFT_DataPort_B0 = ((temp & 0x01) >> 0 );
  TFT_DataPort_B1 = ((temp & 0x02) >> 1 );
  TFT_DataPort_B2 = ((temp & 0x04) >> 2 );
  TFT_DataPort_B3 = ((temp & 0x08) >> 3 );
  TFT_DataPort_B4 = ((temp & 0x10) >> 4 );
  TFT_DataPort_B5 = ((temp & 0x20) >> 5 );
  TFT_DataPort_B6 = ((temp & 0x40) >> 6 );
  TFT_DataPort_B7 = ((temp & 0x80) >> 7 );
  // Strobe
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;

  // Write to port
  //temp = TFT_DataPort;
  //temp &= 0xFF00;
  //TFT_DataPort = Lo(_data) | temp;

  temp = Lo(_data);
  TFT_DataPort_B0 = ((temp & 0x01) >> 0 );
  TFT_DataPort_B1 = ((temp & 0x02) >> 1 );
  TFT_DataPort_B2 = ((temp & 0x04) >> 2 );
  TFT_DataPort_B3 = ((temp & 0x08) >> 3 );
  TFT_DataPort_B4 = ((temp & 0x10) >> 4 );
  TFT_DataPort_B5 = ((temp & 0x20) >> 5 );
  TFT_DataPort_B6 = ((temp & 0x40) >> 6 );
  TFT_DataPort_B7 = ((temp & 0x80) >> 7 );
  // Strobe
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;
}

void InitPantalla(){
   GPIO_Config(&GPIOA_BASE, _GPIO_PINMASK_9| _GPIO_PINMASK_10| _GPIO_PINMASK_8,_GPIO_CFG_MODE_OUTPUT | _GPIO_CFG_SPEED_2MHZ | _GPIO_CFG_OTYPE_PP);
   GPIO_Config(&GPIOB_BASE, _GPIO_PINMASK_0 |_GPIO_PINMASK_3 | _GPIO_PINMASK_5 | _GPIO_PINMASK_4 | _GPIO_PINMASK_10 ,_GPIO_CFG_MODE_OUTPUT | _GPIO_CFG_SPEED_2MHZ | _GPIO_CFG_OTYPE_PP);
   GPIO_Config(&GPIOC_BASE,_GPIO_PINMASK_1 | _GPIO_PINMASK_7  ,_GPIO_CFG_MODE_OUTPUT | _GPIO_CFG_SPEED_2MHZ | _GPIO_CFG_OTYPE_PP);
   GPIO_Digital_Output(&GPIOA_BASE,_GPIO_PINMASK_0 |_GPIO_PINMASK_1 |_GPIO_PINMASK_4 | _GPIO_PINMASK_9 | _GPIO_PINMASK_10 | _GPIO_PINMASK_8 );
   GPIO_Digital_Output(&GPIOB_BASE,_GPIO_PINMASK_3 | _GPIO_PINMASK_4 | _GPIO_PINMASK_5 | _GPIO_PINMASK_10);
   GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_7);
   TFT_Set_Active(&TFT_Set_Index_Custom, &TFT_Write_Command_Custom, &TFT_Write_Data_Custom);
   TFT_Init_ILI9341_8bit_Custom(240, 320);
}