
#ifndef __OLED_H
#define __OLED_H 

#include "main.h"

#define OLED_COMMAND_BUFFER_SIZE 64      

#define OLED_MASK_EOT 0x00                      //Macro for end of transfer
#define OLED_MASK_IDLE 0xFF                     //Macro for idling transfer

#define OLED_CORRUPTION_PREVENTION_SYSTEM       //System sending rolling set off config bytes to OLED to aid stability (refershing values)

#define OLED_DATA_PERIOD_LENGTH     48          //NOTE: OLED_SCLK Frequency= HCLK/OLED_DATA_PERIOD_LENGTH   ; HCLK=48MHz
#define OLED_DATA_PREP_TIME         28          //NOTE: This value decides ratio between low and high SCLK, it represents number of TIM15 clock cycles before rising edge

#define OLED_VARIABLE_TRANSFER_SPEED            //System making DATA and CMD bytes to OLED different speed (for stability?) Comment to disable

#ifdef OLED_VARIABLE_TRANSFER_SPEED
    #define OLED_CMD_PERIOD_LENGTH     60       //NOTE: OLED_SCLK Frequency= HCLK/OLED_DATA_PERIOD_LENGTH   ; HCLK=48MHz
    #define OLED_CMD_PREP_TIME         48       //NOTE: This value decides ratio between low and high SCLK, it represents number of TIM15 clock cycles before rising edge
#endif


//Function prototypes
void OLED_Write_Command(uint8_t dat);

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);					   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);					

void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);	
void OLED_ShowChar(uint8_t x,uint8_t y,char chr,uint8_t size,uint8_t mode);	
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const char *p);	 		
  
uint32_t oled_pow(uint8_t m,uint8_t n);
//void OLED_Showword(uint8_t x,uint8_t y,uint8_t *num,uint8_t mode);

//OLED command macros:
#define OLED_CMD_SET_CONTRAST           0x81
#define OLED_CMD_LIGHT_FULL_DISPLAY     0xA4 
#define OLED_CMD_DISPLAY_ORIENTATION    0xA6
#define OLED_CMD_DISPLAY_POWER          0xAE

#define OLED_CMD_SCROLL_X_CONTINIOUS    0x26
#define OLED_CMD_SCROLL_XY_CONTINIOUS   0x29
#define OLED_CMD_SCROLL_DEACTIVATE      0x2E
#define OLED_CMD_SCROLL_ACTIVATE        0x2F
#define OLED_CMD_SCROLL_Y_AREA          0xA3

#define OLED_CMD_COLUMN_START_LO        0x00
#define OLED_CMD_COLUMN_START_HI        0x10
#define OLED_CMD_ADDRESS_MODE           0x20
#define OLED_CMD_COLUMN_ADDRESS         0x21
#define OLED_CMD_PAGE_ADDRESS           0x22
#define OLED_CMD_PAGE_START_ADDRESS     0xB0

#define OLED_CMD_LINE_START             0x40
#define OLED_CMD_SEGMENT_REMAP          0xA0
#define OLED_CMD_MULTIPLEX_RATIO        0xA8
#define OLED_CMD_COM_SCAN_DIRECTION     0xC0
#define OLED_CMD_DISPLAY_OFFSET         0xD3
#define OLED_CMD_COM_HW_CONF            0xDA

#define OLED_CMD_DISPLAY_CLK            0xD5
#define OLED_CMD_PRE_CHARGE             0xD9
#define OLED_CMD_VCOMB_LEVEL            0xDB
#define OLED_CMD_NOP                    0xE3

#define OLED_CMD_FADE_BLINK             0x23
#define OLED_CMD_ZOOM                   0xD6

#define OLED_CMD_CHG_PUMP               0x8D

// OLED GPIO pins definitions
#define OLED_CS_Pin             LL_GPIO_PIN_12		//
#define OLED_CS_GPIO_Port       GPIOB
#define OLED_RST_Pin            LL_GPIO_PIN_13 		//
#define OLED_RST_GPIO_Port      GPIOC
#define OLED_RS_Pin             LL_GPIO_PIN_13		//
#define OLED_RS_GPIO_Port       GPIOB

#define OLED_SCLK_Pin           LL_GPIO_PIN_15		//
#define OLED_SCLK_GPIO_Port     GPIOB
#define OLED_SDIN_Pin           LL_GPIO_PIN_14	    //
#define OLED_SDIN_GPIO_Port     GPIOB


/*******************OLED output Defined****************************/
#define OLED_CS_H       LL_GPIO_SetOutputPin(OLED_CS_GPIO_Port, OLED_CS_Pin)  		//
#define OLED_CS_L       LL_GPIO_ResetOutputPin(OLED_CS_GPIO_Port, OLED_CS_Pin)

#define OLED_RST_H      LL_GPIO_SetOutputPin(OLED_RST_GPIO_Port,OLED_RST_Pin) 		//
#define OLED_RST_L      LL_GPIO_ResetOutputPin(OLED_RST_GPIO_Port,OLED_RST_Pin) 	//

#define OLED_RS_H       LL_GPIO_SetOutputPin(OLED_RS_GPIO_Port,OLED_RS_Pin)		//
#define OLED_RS_L       LL_GPIO_ResetOutputPin(OLED_RS_GPIO_Port,OLED_RS_Pin)		//

#define OLED_SCLK_H     LL_GPIO_SetOutputPin(OLED_SCLK_GPIO_Port,OLED_SCLK_Pin)		//
#define OLED_SCLK_L     LL_GPIO_ResetOutputPin(OLED_SCLK_GPIO_Port,OLED_SCLK_Pin)		//

#define OLED_SDIN_H     LL_GPIO_SetOutputPin(OLED_SDIN_GPIO_Port,OLED_SDIN_Pin)	    //
#define OLED_SDIN_L     LL_GPIO_ResetOutputPin(OLED_SDIN_GPIO_Port,OLED_SDIN_Pin)		//



























#endif


























