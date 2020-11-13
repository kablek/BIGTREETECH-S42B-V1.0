
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 

//static void MX_TIM15_Init(void); //moved to tim.c/tim.h

//OLED Graphical memory handling
uint8_t OLED_GRAM[128][8];
uint8_t OLED_CURRENTLY_WRITTEN_X=0;
uint8_t OLED_CURRENTLY_WRITTEN_Y=0;

//OLED Command handling
uint8_t OLED_COMMAND_PENDING=0;
uint8_t OLED_COMMAND_BUFFER[OLED_COMMAND_BUFFER_SIZE];
uint8_t OLED_COMMAND_BUFFER_READINDEX=0;
uint8_t OLED_COMMAND_BUFFER_WRITEINDEX=0;

//OLED Low-level bitbang handling
uint8_t OLED_CURRENT_BITMASK=OLED_MASK_IDLE;
uint8_t OLED_TRANSFER_BUFFER=0x00;

#ifdef OLED_CORRUPTION_PREVENTION_SYSTEM		//disable system by commenting definition in oled.h
	//list of 8 rolling commands in between frames making sure OLEDs value stay not-corrupted
	static uint8_t OLED_ROLLING_COMMANDS[8]=
	{
		OLED_CMD_DISPLAY_ORIENTATION,
		OLED_CMD_LINE_START,
		OLED_CMD_SEGMENT_REMAP|0x01,
		OLED_CMD_DISPLAY_POWER|0x01,
		OLED_CMD_COM_SCAN_DIRECTION,
		OLED_CMD_LIGHT_FULL_DISPLAY,
		OLED_CMD_SCROLL_DEACTIVATE,
		OLED_CMD_NOP
	};
	uint8_t OLED_CURRENT_ROLLING_COMMAND=0; //variable for keeping track of currently sent command
#endif /* OLED_CORRUPTION_PREVENTION_SYSTEM */

/**
  * @brief Function for writting command bytes
  * @param dat - Command byte to be writen
  * @retval None
  */
void OLED_Write_Command(uint8_t dat)
{
	while(OLED_COMMAND_PENDING>=OLED_COMMAND_BUFFER_SIZE); //dirty way to prevent buffer overflow
	OLED_COMMAND_BUFFER[OLED_COMMAND_BUFFER_WRITEINDEX++]=dat; //Appends command into buffer
	OLED_COMMAND_BUFFER_WRITEINDEX%=OLED_COMMAND_BUFFER_SIZE;//roll back at the end of buffer
	OLED_COMMAND_PENDING++;
}	//TODO:: Implement multi byte commands to make sure multiple bytes get correctly written (isn't issue so far... yet?)

/**
  * @brief Turn display power on sequence
  * @param none
  * @retval None
  */
void OLED_Display_On(void)
{
	OLED_Write_Command(OLED_CMD_CHG_PUMP);  
	OLED_Write_Command(0X14);  //1byte arg
	OLED_Write_Command(OLED_CMD_DISPLAY_POWER|0x01);  
}

/**
  * @brief Turn display power off sequence
  * @param none
  * @retval None
  */    
void OLED_Display_Off(void)
{
	OLED_Write_Command(OLED_CMD_CHG_PUMP);  
	OLED_Write_Command(0X10);  //1byte arg
	OLED_Write_Command(OLED_CMD_DISPLAY_POWER);  
}		   			 

/**
  * @brief Change Display Clock Divide Ratio/ Oscillator Frequency (for compatibility with TrueStep firmware fork)
  * @param val Clock Divide Ratio/ Oscillator Frequency register value
  * @retval None
  */ 
void OLED_SetDisplayClock(uint8_t val)
{
	OLED_Write_Command(OLED_CMD_DISPLAY_POWER);		//power off display
	OLED_Write_Command(OLED_CMD_DISPLAY_CLK);			//Write new clk value
	OLED_Write_Command(val);							//1byte arg
	OLED_Write_Command(OLED_CMD_DISPLAY_POWER|0x01);	//power display back on
}

/**
  * @brief Clear display screen GRAM
  * @param none
  * @retval None
  */  	  
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)
	for(n=0;n<128;n++)
	OLED_GRAM[n][i]=0x00;  //TODO: Maybe implement DMA for this?
}

/**
  * @brief Draw/clear single point in OLEDs GRAM
  * @param x Position of point
  * @param y Position of point
  * @param t Color of point
  * @retval None
  */ 
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;
	pos=7-y/8;		
	bx=y%8;			
	temp=1<<(7-bx);	
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    //TODO: figure out a bit cleaner way of doing this? especiali color definition
}

/**
  * @brief Draw/clear single point in OLEDs GRAM
  * @param x1 Rectangle starting position
  * @param y1 Rectangle starting position
  * @param x2 Rectangle ending position
  * @param y2 Rectangle ending position
  * @param dot Color of rectange
  * @retval None
  */ 	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}									//TODO: Maybe implement DMA on this? Also confirm safety of X Y stuff				    
}


/**
  * @brief Draw single character on screen
  * @param x character position
  * @param y character position
  * @param chr character code
  * @param size Font size
  * @param mode character color NOTE:we are having veeeery shady implementation here.
  * @retval None
  */ 	
void OLED_ShowChar(uint8_t x,uint8_t y,char chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp=0;
	uint8_t t,t1;
	uint8_t y0=y;
	chr=chr-' ';			   
    for(t=0;t<size;t++)
    {
		switch(size)
		{

			case 12:
			break;
				temp=oled_asc2_1206[(uint8_t) chr][t];
			case 16:
			default:
				temp=oled_asc2_1608[(uint8_t) chr][t];	
			break;
		}
                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);			//note that in case of selecting black text background would auto white with this implementation
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 	
}

//TODO:: Comment this function, something to do with number conversion?
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}		

/**
  * @brief Draw number on screen
  * @param x character position
  * @param y character position
  * @param num Number to be drawn
  * @param len som sort of lenght?? shady implementation
  * @param size font size
  * @retval None
  */ 			  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1;  
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 

/**
  * @brief Draw number on screen
  * @param x character position
  * @param y character position
  * @param p string, shady implementation again, no size or color control
  * @retval None
  */ 	
void OLED_ShowString(uint8_t x,uint8_t y,const char *p)
{
#define MAX_CHAR_POSX 120
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   

/**
  * @brief Complete OLED screen initialization. Hardware included.
  * @param none
  * @retval None
  */ 				    
void OLED_Init(void)
{ 	 
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	//Enable GPIO Clocks
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	//Initialize OLEDs GPIOS
	GPIO_InitStruct.Pin = OLED_CS_Pin| OLED_RS_Pin |OLED_SDIN_Pin |OLED_SCLK_Pin; //NOTE: SCLK pin gets later reinitialized with TIM15 PWM output. Here it is initialized just too keep stuff defined
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;			//NOTE: Dropping speed to medium is still in spec with SSD1306 Timing, in case it doesn't work well switch to high
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = OLED_RST_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;			//NOTE: Dropping speed to medium is still in spec with SSD1306 Timing, in case it doesn't work well switch to high, with RST pin it could maybe even be dropped to low?
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(OLED_RST_GPIO_Port, &GPIO_InitStruct);
	
	//OLED Initialization sequence
    
	OLED_RST_L;			//Put OLED into reset state

	//put GPIOs into know state
	OLED_CS_H;
	OLED_RS_L;
	OLED_SCLK_L;
	OLED_SDIN_L;

	LL_mDelay(100);		//Wait for OLED to actually reset
	OLED_RST_H; 		//put OLED out of reset state

	//Load start-up sequence into buffer -> NOTE: since protocol is handled by TIM15 interrupt commands don't get sent out until TIM15 gets initialized
	OLED_Write_Command(OLED_CMD_DISPLAY_POWER);//
	OLED_Write_Command(OLED_CMD_DISPLAY_CLK);//,
	OLED_Write_Command(0x80);  //1byte arg
	OLED_Write_Command(OLED_CMD_MULTIPLEX_RATIO);//
	OLED_Write_Command(0X3F);//(1/64) 1byte arg
	OLED_Write_Command(OLED_CMD_DISPLAY_OFFSET);//
	OLED_Write_Command(0X00);//1 byte arg?

	OLED_Write_Command(OLED_CMD_LINE_START);// [5:0]
												    
	OLED_Write_Command(OLED_CMD_CHG_PUMP);//
	OLED_Write_Command(0x14);//bit2， 1byte arg
	OLED_Write_Command(OLED_CMD_ADDRESS_MODE);//
	OLED_Write_Command(0x02);//[1:0],;;1 1byte arg
	OLED_Write_Command(OLED_CMD_SEGMENT_REMAP|0x01);//,bit0:0,0->0;1,0->127;
	OLED_Write_Command(OLED_CMD_COM_SCAN_DIRECTION);//;bit3[N-;N:
	OLED_Write_Command(OLED_CMD_COM_HW_CONF);//
	OLED_Write_Command(0x12);//[5:4] 1byte arg
		 
	OLED_Write_Command(OLED_CMD_SET_CONTRAST);//
	OLED_Write_Command(0xEF);//1~255; 1byte arg
	OLED_Write_Command(OLED_CMD_PRE_CHARGE);//
	OLED_Write_Command(0xF1);//1byte arg [3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Write_Command(OLED_CMD_VCOMB_LEVEL);// 
	OLED_Write_Command(0x30);//1byte arg [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_Write_Command(OLED_CMD_LIGHT_FULL_DISPLAY);//;bit0:1,;0,
	OLED_Write_Command(OLED_CMD_DISPLAY_ORIENTATION);//;bit0:1,;0,	    						   
	OLED_Write_Command(OLED_CMD_DISPLAY_POWER|0x01);//
	OLED_Clear(); //Clear
	//TODO:: Clean up those comments, as stated above multibyte command writting should be implemented

	MX_TIM15_Init();	//Initialize and start TIM15. LET OLED PARTY BEGIN!!!
	
	LL_TIM_EnableAllOutputs(TIM15);									//Enable TIM15 Master Output Enable (MOE) bit. <-no output without MOE bit set.

	LL_TIM_SetUpdateSource(TIM15,LL_TIM_UPDATESOURCE_COUNTER);		//Only trigger interrupt on Update event (which in setup mode is falling clock edge)
	LL_TIM_CC_EnableChannel(TIM15, LL_TIM_CHANNEL_CH2);				//Enable output channel
	LL_TIM_EnableIT_UPDATE(TIM15);									//Enable interrupt
	LL_TIM_EnableCounter(TIM15);									//Start the timer
}  

#if 0	//following code is removed/deprecated due to new version being moved into tim.c/tim.h files
/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

	/* USER CODE BEGIN TIM15_Init 0 */

	/* USER CODE END TIM15_Init 0 */

	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM15);

	/* TIM15 interrupt Init */
	NVIC_SetPriority(TIM15_IRQn, 0);		//TODO:: Figure out NVIC priorities 
	NVIC_EnableIRQ(TIM15_IRQn);

	/* TIM15 load setup values */
	TIM_InitStruct.Prescaler = 0;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = OLED_DATA_PERIOD_LENGTH;			//NOTE: OLED_SCLK Frequency= HCLK/OLED_DATA_PERIOD_LENGTH
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;		//NOTE:	ex: OLED_SCLK Frequency= 48MHz/48 = 1MHz
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM15, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM15);								//TODO: Figure out this preload stuff and make it work with dynamic TIM15 speeds
	LL_TIM_SetClockSource(TIM15, LL_TIM_CLOCKSOURCE_INTERNAL);		//Internal CLK selection DUUUUH!
	/* TIM15 Channel setup values */
	LL_TIM_OC_DisablePreload(TIM15, LL_TIM_CHANNEL_CH2);			//TODO: Figure out this preload stuff and make it work with dynamic TIM15 speeds
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;					//PWM2 mode -> Output inactive until  Timer value >= Compare value
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = OLED_DATA_PREP_TIME;			//NOTE: This value decides ratio between low and high SCLK, it represents number of TIM15 clock cycles before rising edge
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;			//Default polarity
	TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
	TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
	TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
	LL_TIM_OC_Init(TIM15, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
	LL_TIM_OC_EnableFast(TIM15, LL_TIM_CHANNEL_CH2);			//Not really sure what it does, but I guess fast is good?
	LL_TIM_SetOnePulseMode(TIM15, LL_TIM_ONEPULSEMODE_SINGLE);	//One pulse mode -> Timer gets restarted at the end of interrupt handler function ->NOTE: Might consider disabling one pulse mode in case of optimizations -> will get to it
	LL_TIM_SetTriggerOutput(TIM15, LL_TIM_TRGO_UPDATE);
	LL_TIM_DisableMasterSlaveMode(TIM15);
	TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;			//NOTE: This BDTR stuff is not really needed, we just load values to put everything into known state.
	TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
	TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
	TIM_BDTRInitStruct.DeadTime = 0;
	TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
	TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
	TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
	LL_TIM_BDTR_Init(TIM15, &TIM_BDTRInitStruct);

	/**TIM15 GPIO Configuration
	 PB15 (OLED_SCLK_Pin)  ------> TIM15_CH2
	*/

	//LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);		//NOTE: Don't need it, because it is already done in OLED_Init();	I keep it here just to make sure,

	GPIO_InitStruct.Pin = OLED_SCLK_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(OLED_SCLK_GPIO_Port, &GPIO_InitStruct);



}
#endif

/**
  * @brief This function handles TIM15 global interrupt.
  */
void TIM15_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM15) == 1)		//NOTE: might not be needed, but requires testing first
	{
		LL_TIM_ClearFlag_UPDATE(TIM15);				//Clear interrupt flag
		//Transfer handling ahead:
		if(OLED_CURRENT_BITMASK==OLED_MASK_IDLE) 	//means previous transfer has ended and is now time to load next one
		{
			if(OLED_COMMAND_PENDING!=0) 			//check if there should be some sent commands
			{
				OLED_RS_L;
				OLED_COMMAND_PENDING--;
				//OLED_COMMAND_PENDING%=OLED_COMMAND_BUFFER_SIZE;		//should make sure this variable is safe, but since I am not touching it anywhere else unprotected...
				OLED_TRANSFER_BUFFER=OLED_COMMAND_BUFFER[OLED_COMMAND_BUFFER_READINDEX++]; //Load next command byte
				OLED_COMMAND_BUFFER_READINDEX%=OLED_COMMAND_BUFFER_SIZE;

				#ifdef OLED_VARIABLE_TRANSFER_SPEED						//NOTE: System to make Data and Command bytes different speed (to aid stability?) Disable by commenting definition in oled.h
					LL_TIM_SetAutoReload(TIM15,OLED_CMD_PERIOD_LENGTH);
					LL_TIM_OC_SetCompareCH2(TIM15,OLED_CMD_PREP_TIME);
				#endif /* OLED_VARIABLE_TRANSFER_SPEED */
			}
			else	//Or just load next graphic byte
			{
				OLED_RS_H;
				OLED_TRANSFER_BUFFER=OLED_GRAM[OLED_CURRENTLY_WRITTEN_X][OLED_CURRENTLY_WRITTEN_Y]; //Load next GFX byte

				#ifdef OLED_VARIABLE_TRANSFER_SPEED						//NOTE: System to make Data and Command bytes different speed (to aid stability?) Disable by commenting definition in oled.h
					LL_TIM_SetAutoReload(TIM15,OLED_DATA_PERIOD_LENGTH);
					LL_TIM_OC_SetCompareCH2(TIM15,OLED_DATA_PREP_TIME);
				#endif /* OLED_VARIABLE_TRANSFER_SPEED */
				/* code */
			}
			OLED_CURRENT_BITMASK=0x80;
			OLED_CS_L;
		}
		else if(OLED_CURRENT_BITMASK==OLED_MASK_EOT) //means this transfer will now end
		{
			OLED_CS_H;
			if(LL_GPIO_IsOutputPinSet(OLED_RS_GPIO_Port,OLED_RS_Pin) != 0)
			{
			if(++OLED_CURRENTLY_WRITTEN_X>=128)
				{
					OLED_CURRENTLY_WRITTEN_Y++;
					OLED_CURRENTLY_WRITTEN_Y%=8;
					#ifdef OLED_CORRUPTION_PREVENTION_SYSTEM
						OLED_CURRENT_ROLLING_COMMAND++;
						OLED_CURRENT_ROLLING_COMMAND%=8;
						OLED_Write_Command (OLED_ROLLING_COMMANDS[OLED_CURRENT_ROLLING_COMMAND]);
					#endif /* OLED_CORRUPTION_PREVENTION_SYSTEM */
					OLED_Write_Command (OLED_CMD_PAGE_START_ADDRESS+OLED_CURRENTLY_WRITTEN_Y);  
					OLED_Write_Command (OLED_CMD_COLUMN_START_LO);      
					OLED_Write_Command (OLED_CMD_COLUMN_START_HI); 
				}
				OLED_CURRENTLY_WRITTEN_X%=128;
			}
			OLED_CURRENT_BITMASK=OLED_MASK_IDLE;
			return LL_TIM_EnableCounter(TIM15);
		}	
		if(OLED_CURRENT_BITMASK&OLED_TRANSFER_BUFFER) OLED_SDIN_H;
		else OLED_SDIN_L;
		OLED_CURRENT_BITMASK>>=1;
		LL_TIM_EnableCounter(TIM15);
	}
}

//TODO: consider writing up some sort of keyframe based compresion type system, only refreshing what is needed
//TODO: Do some speed optimizations, we want display to be somewhat lightweight