/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "math.h"
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_magneto.h"
#include "vl53l0x_proximity.h"
//#include " VL53L0X_def.h"

#include "stm32l475e_iot01.h"
#include "lib_TT4_interface.h"
#include "m24sr.h"
#include "VcardCSL1.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/************************************************ For NFC API ************************************************************************/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[20];
  uint32_t DemoIndex;
}BSP_DemoTypedef;

/******************************************************************** END **************************************************************/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define f_One	 	49
#define f_Two	 	50
#define f_Three	 	51
#define f_Four	 	52
#define f_Five		53
#define f_Six		54
#define f_Seven		55
#define f_Eight		56
#define f_Nine		57

#define f_a			97
#define	f_b			98
#define	f_c			99
#define	f_d			100
#define	f_e			101
#define	f_f			102
#define	f_g			103

#define f_A			65
#define f_B			66
#define f_C			67
#define f_D			68
#define f_E			69
#define f_F			70
#define f_G			71

#define f_escape 	27
#define f_error     72
#define f_size		1
#define f_count		10000

#define BUFFERDATA 	2
#define BUFFERRX 	5

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

float temp_value 	= 0; 				// Measured temperature value
float humi_value 	= 0; 				// Measured humidity value
float pre_value 	= 0;  				// Measured Pressure value
float gyro_value	= 0;				// Measured gyro value
char str_tmp[100] 	= ""; 				// Formatted message to display the temperature value
char str_humi[100] 	= ""; 				// Formatted message to display the HUMIDITY value
char str_pre[100] 	= ""; 				// Formatted message to display the Pressure value
char str_gyro[100] 	= ""; 				// Formatted message to display the GYRO value
char rxBuffer[BUFFERRX];				// This buffer is use for storing input data
char  str_acc1[100]	= {0};				// Accelerometer  x-axis,y-axis,z-axis
int16_t pDataXYZ[3] = {0};				// Accelerometer  Data
float pfData[3]		= {0};				// GYRO Data

uint8_t msg1[] 		= "\033\143 ****** sensors values measurement ******\n\r";
uint8_t msg2[] 		= "Initialize ALL sensors\r\n";
uint8_t msg3[] 		= "\033\143 Please select the option \r\n 1. TEMPERATURE \r\n 2. HUMIDITY \r\n 3. PRESSURE \r\n 4. ACCELERO \r\n 5. GYRO \r\n 6. MAGNETOMETER \r\n 7. PROXIMITY \r\n 8. NFC\r\n ***MENU*** \r\nPress Escape + Enter\r\033[9A";
uint8_t msg4[]		= "\033\143 Please select the option \r\n 1. NFC Detected \r\n 2. NFC Write \r\n 3. NFC Read \r\n 4. Back \r\n ***MENU*** \r\nPress Escape + Enter\r\033[5A";
uint8_t msg5[] 		= "\033\143 Please select the option \r\n 1. NFC EMAIL Write\r\n 2. NFC URL Write\r\n 3. NFC VCARD Write\r\n 4. NFC SMS Write\r\n 5. AAR Write\r\n 6. Back \r\n ***MENU*** \r\nPress Escape + Enter\r\033[7A";
uint8_t msg6[] 		= "\033\143 Please select the option \r\n 1. NFC EMAIL Read\r\n 2. NFC URL Read\r\n 3. NFC VCARD Read\r\n 4. NFC SMS Read\r\n 5. Back \r\n ***MENU*** \r\nPress Escape + Enter\r\033[6A";

uint8_t com_up[]	= "\033[7A";
uint8_t com_dn[]	= "\033[B";
uint8_t N_com_up[]	= "\033[3A";
uint8_t W_com_up[]	= "\033[5A";
uint8_t R_com_up[]	= "\033[4A";
char Invalid[30]	= "\033\143!!..Invalid Input..!!\r\n";

/******************************************************** Switch ********************************************************************/

uint16_t f_lastDebounceTime = 0;  					//last time the output pin was toggled
uint16_t f_debounceDelay = 50;    					//debounce time
uint16_t f_presstime, f_releasetime; 				//in millis at press and millis at release
uint16_t f_timediff; 								//The time in between each press and release of the switch
int f_lastButtonState = 1;   					    //Previous reading from the input Button
bool f_buttonState = 1; 						    //state of the switch
bool f_flag1, f_flag2; 								//just two variables
uint8_t f_tapCounter=0; 							//switch pressed counter
uint16_t f_reading=0;								//current state of Button
uint8_t sw_flag	=0;									//flag for (f_INT_count )  increment
uint8_t f_INT_count =1;								//for cursore move
uint8_t s_case = 0;									//Switch case input

/******************************************************************** END **************************************************************/

uint8_t f_Menu_flag		= 1;
uint8_t f_NFC_flag		= 0;
uint8_t f_NFC_W_flag	= 0;
uint8_t f_NFC_R_flag	= 0;
uint8_t NFC_check  = 0;
/************************************************ checking the timer callback **********************************************************/

uint8_t flag_temp  		= 1;
uint8_t flag_humi  		= 1;
uint8_t flag_pre   		= 1;
uint8_t flag_acce  		= 1;
uint8_t flag_gyro  		= 1;
uint8_t flag_mag   		= 1;
uint8_t flag_pro   		= 1;
uint8_t flag_Email 		= 1;
uint8_t flag_R_Email 	= 1;
uint8_t	flag_Vcard		= 1;
uint8_t flag_R_Vcard	= 1;
uint8_t	flag_aar		= 1;
uint8_t	flag_URL		= 1;
uint8_t flag_R_URL		= 1;
uint8_t flag_sms		= 1;
uint8_t flag_NFC		= 1;
uint8_t flag_R_sms		= 1;
uint8_t flag_error 		= 1;
extern volatile uint8_t    GPO_Low ;
volatile uint16_t count_temp = 0;
/******************************************************************** END **************************************************************/

/********************* UART1 *******************************************/
uint8_t newMsg = 0 , rxData[BUFFERDATA] , rxIndex=0,size=0;

/******************************************************************** END **************************************************************/

uint16_t prox_value = 0;
char str_pro[100] 	= "";

volatile uint16_t count_error	= 0;

/******************************************************** Email Write ******************************************************************/

char EmailAdd[] 	= {"farhanashraf1999@gmail.com\0"};
char Subject[] 		= {"NFC Email Write Demo...\0"};
char EmailMessage[] = {"For Testing email\0"};
char Information[] 	= {"Email\0"};

/******************************************************************** END **************************************************************/

/******************Field to fill for the SMS demonstration *********/

char PhoneNumber[] 	= {"6201133237\0"};
char Message[] 		= {"NFC Message Demo\0"};
char Instruction[] 	= {"This is an example of NFC generated SMS\0"};

/******************************************************************** END **************************************************************/

/************************************************************* Vcard demonstration *****************************************************/

char FirstName[] = {"Farhan Ashraf\0"};
char Title[] = {"Customer support\0"};
char Org[] = {"Einfochips\0"};
char HomeAddress[] = {"\0"};
char WorkAddress[] = {"Einfochips Ratna Ognaj\0"};
char HomeTel[] = {"\0"};
char WorkTel[] = {"\0"};
char CellTel[] = {"62 01 13 32 37\0"};
char HomeEmail[] = {"\0"};
char WorkEmail[] = {"farhan.ashraf@einfochips.com\0"};

/******************************************************************** END **************************************************************/

/***************************************************************************************************************************************/
char str_url[300] 	= "";
char str_email[700] = "";
char str_Vcard[1000] = "";
char str_sms[300] 	= "";

/******************************************************************** END **************************************************************/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */
/************************* Sensor API Initialization *************************************/
void Proximity_Test(void);
void f_Temperature(void);
void f_Humidity(void);
void f_Pressure(void);
void f_ACCELEROMETER(void);
void f_GYRO(void);
void f_MAGNETOMETERR(void);
void f_cur_mov(void);
void f_cur_sel(void);
int f_Menu(void);
int f_Switch(void);
void f_NFC_D(void);
void f_NFC(void);
void f_NFC_W(void);
void f_NFC_R(void);
int f_Invalid(void);
/************************* NFC API Initialization *************************************/

static void EMAILwrite_demo(void);
static void EMAILread_demo(void);

static void URLwrite_demo(void);
static void URLread_demo(void);

static void Vcardwrite_demo2(void);
static void Vcardread_demo2(void);

static void SMSwrite_demo(void);
static void SMSread_demo(void);

static void AARwrite_demo(void);

/******************************************************************** END **************************************************************/
void Error_Handler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/************************* NFC API Calling *************************************/
BSP_DemoTypedef  NFC_examples[]=
	{
		{EMAILwrite_demo, "EMAIL", 0},
		{EMAILread_demo,"R_EMAIL", 1},

		{URLwrite_demo,"URL", 2},
		{URLread_demo,"R_URL", 3},

		{Vcardwrite_demo2,"vCARD2", 4},
		{Vcardread_demo2,"R_vCARD2", 5},

		{SMSwrite_demo,"SMS", 6},
		{SMSread_demo,"R_SMS", 7},

		{AARwrite_demo,"AAR", 8},

	};
/******************************************************************** END **************************************************************/

/*************************************************** NFC Console UI **************************************************************/

void f_NFC(void)										// NFC Operations API Name Console Print
{
	f_Menu_flag		= 0;
	f_NFC_flag		= 1;
	f_NFC_W_flag	= 0;
	f_NFC_R_flag	= 0;
	HAL_UART_Transmit(&huart1,msg4,sizeof(msg4),100);
	f_INT_count=10;
	sw_flag = 1;
	s_case = 0;
}

void f_NFC_D(void)										// NFC Detected API Name Console Print
{
	if(flag_NFC)
	{
		if(!NFC_check)
		{
			HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC Detected\r\n",sizeof("\033\143NFC Detected\r\n"),10);
		}
		else
		{
			HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC NOT Present\r\n",sizeof("\033\143NFC NOT Present\r\n"),10);
		}
		flag_NFC=0;
	}

}

void f_NFC_W(void)										// NFC Write API Name Console Print
{
	f_Menu_flag		= 0;
	f_NFC_flag		= 0;
	f_NFC_W_flag	= 1;
	f_NFC_R_flag	= 0;
	HAL_UART_Transmit(&huart1,msg5,sizeof(msg5),100);
	f_INT_count = 15;
	sw_flag = 1;
	s_case = 0;
}

void f_NFC_R(void)										// NFC Read API Name Console Print
{
	f_Menu_flag		= 0;
	f_NFC_flag		= 0;
	f_NFC_W_flag	= 0;
	f_NFC_R_flag	= 1;
	HAL_UART_Transmit(&huart1,msg6,sizeof(msg6),100);
	f_INT_count = 22;
	sw_flag = 1;
	s_case = 0;
}

/******************************************************************** END **************************************************************/

/************************************* EMAIL API ******************************************/
static void EMAILwrite_demo(void)
{
	if(flag_Email)
	{
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC Email Write Mode\r\n",sizeof("\033\143NFC Email Write Mode\r\n"),10);

		uint16_t status = ERROR;
		sEmailInfo EmailStruct;
		sEmailInfo *pEmailStruct;

		pEmailStruct = &EmailStruct;

		memcpy(pEmailStruct->EmailAdd, EmailAdd, strlen(EmailAdd)+1);
		memcpy(pEmailStruct->Subject, Subject, strlen(Subject)+1);
		memcpy(pEmailStruct->Message, EmailMessage, strlen(EmailMessage)+1);
		memcpy(pEmailStruct->Information, Information, strlen(Information)+1);

		status = TT4_WriteEmail ( pEmailStruct );

		if(status == SUCCESS)
		{
			HAL_UART_Transmit(&huart1,( uint8_t * )"NFC Email Write Done\r\n",sizeof("NFC Email Write Done\r\n"),10);
			snprintf(str_email,700,"Email Addr 	: %s \r\nSubject 	: %s \r\nMessage 	: %s\r\n",pEmailStruct->EmailAdd,pEmailStruct->Subject,pEmailStruct->Message);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_email,sizeof(str_email),100);
		}

		flag_Email=0;
	}
}

static void EMAILread_demo(void)
{
	//if(flag_R_Email)
	{
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC Email Read Mode \r\n",sizeof("\033\143 NFC Email Read Mode \r\n"),10);

		//if(!NFC_check)
		{
			sEmailInfo EmailStruct;
			sEmailInfo *pEmailStruct;
			pEmailStruct = &EmailStruct;

			TT4_ReadEmail ( pEmailStruct );

			HAL_UART_Transmit(&huart1,( uint8_t * )"NFC Email Read Done \r\n",sizeof("NFC Email Read Done \r\n"),10);
			snprintf(str_email,700,"Email Addr 	: %s \r\nSubject 	: %s \r\nMessage 	: %s\r\n",pEmailStruct->EmailAdd,pEmailStruct->Subject,pEmailStruct->Message);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_email,sizeof(str_email),10);
		}

		flag_R_Email=0;
	}
}

/******************************************************************** END **************************************************************/
/************************************* URL API ******************************************/
static void URLwrite_demo(void)
{
	if(flag_URL)
	{
		sURI_Info URL;
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC URL Write Mode\r\n",sizeof("\033\143NFC URL Write Mode\r\n"),10);
		strcpy(URL.protocol,URI_ID_0x01_STRING);
		strcpy(URL.URI_Message,"einfochips.com");
		strcpy(URL.Information,"\0");

		while (TT4_WriteURI(&URL) != SUCCESS);

		if(TT4_WriteURI(&URL) == SUCCESS)
		{
			HAL_UART_Transmit_IT(&huart1,( uint8_t * )"NFC URL Write Done\r\n",sizeof("NFC URL Write Done\r\n"));
			snprintf(str_url,300,"URL : %s \r\n",URL.URI_Message);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_url,sizeof(str_url),10);
		}

		flag_URL = 0;
	}


}

static void URLread_demo(void)
{

	if(flag_R_URL)
	{
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC URL Read Mode\r\n",sizeof("\033\143NFC URL Read Mode\r\n"),10);
		//if(!NFC_check)
		{
			uint16_t status = ERROR;
			sURI_Info URL;
			sURI_Info *pURL;
			pURL=&URL;

			status = TT4_ReadURI(&URL);

			if(status == SUCCESS)
			{
				HAL_UART_Transmit(&huart1,( uint8_t * )"NFC URL Read Done\r\n",sizeof("NFC URL Read Done\r\n"),10);
				snprintf(str_url,300,"URL : %s \r\n",pURL->URI_Message);
				HAL_UART_Transmit(&huart1,( uint8_t * )str_url,sizeof(str_url),10);
			}

			flag_R_URL = 0;
		}
	}

}

/******************************************************************** END **************************************************************/

/************************************* Vcard API ******************************************/

static void Vcardwrite_demo2(void)
{
	if(flag_Vcard)
	{
		uint16_t status = ERROR;
		sVcardInfo VcardStruct;
		sVcardInfo *pVcardStruct;

		pVcardStruct = &VcardStruct;

		memcpy(pVcardStruct->FirstName, FirstName, strlen(FirstName)+1);
		memcpy(pVcardStruct->Title, Title, strlen(Title)+1);
		memcpy(pVcardStruct->Org, Org, strlen(Org)+1);
		memcpy(pVcardStruct->HomeAddress, HomeAddress, strlen(HomeAddress)+1);
		memcpy(pVcardStruct->WorkAddress, WorkAddress, strlen(WorkAddress)+1);
		memcpy(pVcardStruct->HomeTel, HomeTel, strlen(HomeTel)+1);
		memcpy(pVcardStruct->WorkTel, WorkTel, strlen(WorkTel)+1);
		memcpy(pVcardStruct->CellTel, CellTel, strlen(CellTel)+1);
		memcpy(pVcardStruct->HomeEmail, HomeEmail, strlen(HomeEmail)+1);
		memcpy(pVcardStruct->WorkEmail, WorkEmail, strlen(WorkEmail)+1);

		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC VCARD Write Mode\r",sizeof("\033\143 NFC VCARD Write Mode\r"),10);

		status = TT4_WriteVcard ( &VcardStruct );

		if(status != SUCCESS)
		{
			Error_Handler();
		}
		if(status == SUCCESS)
		{
			HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC VCARD Write Done\r",sizeof("\033\143 NFC VCARD Write Done\r"),10);
			snprintf(str_Vcard,1000,"Name		: %s \r\nTitle		: %s \r\nOrg		: %s\r\nHomeAddress 	: %s\r\nHomeTel 	: %s\r\nWorkTel 	: %s\r\nCellTel 	: %s\r\nHomeEmai 	: %s\r\nHomeEmai 	: %s\r\n",pVcardStruct->FirstName,pVcardStruct->Title,pVcardStruct->Org,pVcardStruct->HomeAddress,pVcardStruct->HomeTel,pVcardStruct->WorkTel,pVcardStruct->CellTel,pVcardStruct->HomeEmail,pVcardStruct->WorkEmail);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_Vcard,sizeof(str_Vcard),100);
		}

		flag_Vcard=0;
	}

}


static void Vcardread_demo2(void)
{
	if(flag_R_Vcard)
	{
		//if(!NFC_check)
		{
			uint16_t status = ERROR;
			sVcardInfo VcardStruct;
			sVcardInfo *pVcardStruct;

			pVcardStruct = &VcardStruct;

			HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143NFC Vcard Read Mode \r\n",sizeof("\033\143 NFC Vcard Read Mode \r\n"),10);

			status = TT4_ReadVcard ( &VcardStruct );

			if(status == SUCCESS)
			{
				snprintf(str_Vcard,1000,"Name		: %s \r\nTitle		: %s \r\nOrg		: %s\r\nHomeAddress 	: %s\r\nHomeTel 	: %s\r\nWorkTel 	: %s\r\nCellTel 	: %s\r\nHomeEmai 	: %s\r\nHomeEmai 	: %s\r\n",pVcardStruct->FirstName,pVcardStruct->Title,pVcardStruct->Org,pVcardStruct->HomeAddress,pVcardStruct->HomeTel,pVcardStruct->WorkTel,pVcardStruct->CellTel,pVcardStruct->HomeEmail,pVcardStruct->WorkEmail);
				HAL_UART_Transmit(&huart1,( uint8_t * )str_Vcard,sizeof(str_Vcard),100);
			}

			flag_R_Vcard = 0;
		}
	}

}

/******************************************************************** END **************************************************************/

/************************************* SMS API ******************************************/
static void SMSwrite_demo(void)
{
	if(flag_sms)
	{
		uint16_t status = ERROR;
		sSMSInfo SMSStruct;
		sSMSInfo *pSMSStruct;

		pSMSStruct = &SMSStruct;
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC SMS Write Mode\r",sizeof("\033\143 NFC SMS Read Mode\r"),10);

		memcpy(pSMSStruct->PhoneNumber, PhoneNumber, strlen(PhoneNumber)+1);
		memcpy(pSMSStruct->Message, Message, strlen(Message)+1);
		memcpy(pSMSStruct->Information, Instruction, strlen(Instruction)+1);

		status = TT4_WriteSMS ( pSMSStruct );

		if(status == SUCCESS)
		{
			HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC SMS Read Done\r\n",sizeof("\033\143 NFC SMS Read Done\r\n"),10);
			snprintf(str_sms,1000,"PhoneNumber 	:  %s \r\nMessage 		:  %s \r\nInformation 	: %s\r\n",pSMSStruct->PhoneNumber,pSMSStruct->Message,pSMSStruct->Information);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_sms,sizeof(str_sms),100);
		}

		flag_R_sms = 0;
	}
}

static void SMSread_demo(void)
{
	if(flag_R_sms)
	{
		uint16_t status = ERROR;
		sSMSInfo SMSStruct;
		sSMSInfo *pSMSStruct;

		pSMSStruct = &SMSStruct;
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC SMS Read Mode\r\n",sizeof("\033\143 NFC SMS Read Mode\r\n"),10);
		status = TT4_ReadSMS ( pSMSStruct );

		if(status == SUCCESS)
		{
			HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC SMS Read Done\r\n",sizeof("\033\143 NFC SMS Read Done\r\n"),10);
			snprintf(str_sms,1000,"PhoneNumber 	:  %s \r\nMessage 		:  %s \r\nInformation 	: %s\r\n",pSMSStruct->PhoneNumber,pSMSStruct->Message,pSMSStruct->Information);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_sms,sizeof(str_sms),100);
		}

		flag_R_sms = 0;
	}

}
/******************************************************************** END **************************************************************/
/***************************** Android Application Record *******************************/
static void AARwrite_demo(void)
{
	if(flag_aar)
	{
		uint16_t status = ERROR;
		uint8_t NULL_NDEF[2] = {0,0};
		sAARInfo AAR_struct;
		sAARInfo *pAAR;

		pAAR = &AAR_struct;
		HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC AAR Write Mode\r\n",sizeof("\033\143 NFC AAR Read Mode\r\n"),10);
		TT4_WriteNDEF(NULL_NDEF);
		memcpy(pAAR->PakageName, "com.stm.bluetoothlevalidation", strlen("com.stm.bluetoothlevalidation")+1);

		status = TT4_AddAAR(pAAR);

		if(status != SUCCESS)
		{
			Error_Handler();
		}
		flag_aar = 0;
	}

}

/******************************************************************** END **************************************************************/

/********************************** Test of VL53L0X proximity sensor *********************************************************************/

void Proximity_Test(void)
{
	if(flag_pro)
	{
		prox_value = VL53L0X_PROXIMITY_GetDistance();
		snprintf(str_pro,100," \033\143 Distance = %d mm\r", prox_value);
		HAL_UART_Transmit(&huart1,( uint8_t * )str_pro,sizeof(str_pro),10);
		memset(str_pro, 0, sizeof(str_pro));
		flag_pro=0;
	}

}


/**
  * @brief  VL53L0X proximity sensor Initialization.
  */
static void VL53L0X_PROXIMITY_Init(void)
{
  uint16_t vl53l0x_id = 0;
  VL53L0X_DeviceInfo_t VL53L0X_DeviceInfo;

  /* Initialize IO interface */
  SENSOR_IO_Init();
  VL53L0X_PROXIMITY_MspInit();

  memset(&VL53L0X_DeviceInfo, 0, sizeof(VL53L0X_DeviceInfo_t));

  if (VL53L0X_ERROR_NONE == VL53L0X_GetDeviceInfo(&Dev, &VL53L0X_DeviceInfo))
  {
    if (VL53L0X_ERROR_NONE == VL53L0X_RdWord(&Dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, (uint16_t *) &vl53l0x_id))
    {
      if (vl53l0x_id == VL53L0X_ID)
      {
        if (VL53L0X_ERROR_NONE == VL53L0X_DataInit(&Dev))
        {
          Dev.Present = 1;
          SetupSingleShot(Dev);
        }
        else
        {
          printf("VL53L0X Time of Flight Failed to send its ID!\n");
        }
      }
    }
    else
    {
      printf("VL53L0X Time of Flight Failed to Initialize!\n");
    }
  }
  else
  {
    printf("VL53L0X Time of Flight Failed to get infos!\n");
  }
}

/**
  * @brief  Get distance from VL53L0X proximity sensor.
  * @retval Distance in mm
  */
static uint16_t VL53L0X_PROXIMITY_GetDistance(void)
{
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;

  VL53L0X_PerformSingleRangingMeasurement(&Dev, &RangingMeasurementData);

  return RangingMeasurementData.RangeMilliMeter;
}



/**
  * @brief  VL53L0X proximity sensor Msp Initialization.
  */
static void VL53L0X_PROXIMITY_MspInit(void)
{/*
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  Configure GPIO pin : VL53L0X_XSHUT_Pin
  GPIO_InitStruct.Pin = VL53L0X_XSHUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(VL53L0X_XSHUT_GPIO_Port, &GPIO_InitStruct);
*/

  HAL_GPIO_WritePin(VL53L0X_XSHUT_GPIO_Port, VL53L0X_XSHUT_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);

}
/******************************************************************** END **************************************************************/
/************************************** This function use for extracting Temperature data **********************************************/
void f_Temperature(void)
{

	if(flag_temp)
	{

		if(BSP_TSENSOR_Init())
		{
			snprintf(str_tmp,100,"\033\143 Not Taking TEMPERATURE Data\r");
			HAL_UART_Transmit(&huart1,( uint8_t * )str_tmp,sizeof(str_tmp),10);
			memset(str_tmp, 0, sizeof(str_tmp));
		}
		else
		{
			temp_value = BSP_TSENSOR_ReadTemp();
			snprintf(str_tmp,100," \033\143 TEMPERATURE = %.2f \r", temp_value);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_tmp,sizeof(str_tmp),10);
			memset(str_tmp, 0, sizeof(str_tmp));
			flag_temp=0;
		}
	}

}
/*********************************************************** END **************************************************************************/

/*This function use for extracting Humidity data */
void f_Humidity(void)
{
	if(flag_humi)
	{
		if(BSP_HSENSOR_Init())
		{
			snprintf(str_humi,100,"\033\143 Not Taking HUMIDITY Data \r");
			HAL_UART_Transmit(&huart1,( uint8_t * )str_humi,sizeof(str_humi),10);
			memset(str_humi, 0, sizeof(str_humi));
		}
		else
		{
			humi_value = BSP_HSENSOR_ReadHumidity();
			snprintf(str_humi,100,"\033\143 HUMIDITY = %.2f \r", humi_value);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_humi,sizeof(str_humi),10);
			memset(str_humi, 0, sizeof(str_humi));
			flag_humi=0;
		}
	}
}
/*********************************************************** END **************************************************************************/

/************************************* This function use for extracting Pressure data *****************************************************/
void f_Pressure(void)
{
	if(flag_pre)
	{
		if(BSP_PSENSOR_Init())
		{
			snprintf(str_pre,100,"\033\143 Not Taking PRESSURE Data \r");
			HAL_UART_Transmit(&huart1,( uint8_t * )str_pre,sizeof(str_pre),10);
			memset(str_pre, 0, sizeof(str_pre));
		}
		else
		{
			pre_value = BSP_PSENSOR_ReadPressure();
			snprintf(str_pre,100,"\033\143 PRESSURE = %.2f \r", pre_value);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_pre,sizeof(str_pre),10);
			memset(str_pre, 0, sizeof(str_pre));
			flag_pre=0;
		}
	}
}
/*********************************************************** END **************************************************************************/

/********************************************** This function use for extracting ACCELEROMETER data ***************************************/
void f_ACCELEROMETER(void)
{
	if(flag_acce)
	{
		if(BSP_ACCELERO_Init())
		{
			BSP_ACCELERO_AccGetXYZ(pDataXYZ);

			snprintf(str_acc1,100,"\033\143 X-axis Error	Y-axis Error	Z-axis Error \r");
			HAL_UART_Transmit(&huart1,( uint8_t * )str_acc1,sizeof(str_acc1),10);
			memset(str_acc1, 0, sizeof(str_acc1));
		}
		else
		{
			BSP_ACCELERO_AccGetXYZ(pDataXYZ);
			snprintf(str_acc1,100,"\033\143 X-axis = %d      Y-axis = %d      Z-axis = %d \r", pDataXYZ[0],pDataXYZ[1],pDataXYZ[2]);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_acc1,sizeof(str_acc1),10);
			memset(str_acc1, 0, sizeof(str_acc1));
			flag_acce = 0;
		}
	}
}
/*********************************************************** END **************************************************************************/

/************************************************* This function use for extracting GYRO data *********************************************/
void f_GYRO(void)
{
	if(flag_gyro)
	{
		if(BSP_GYRO_Init())
		{
			BSP_GYRO_GetXYZ(pfData);

			snprintf(str_gyro,100,"\033\143 X-axis Error	Y-axis Error	Z-axis Error \r");
			HAL_UART_Transmit(&huart1,( uint8_t * )str_gyro,sizeof(str_gyro),10);
			memset(str_gyro, 0, sizeof(str_gyro));
		}
		else
		{
			BSP_GYRO_GetXYZ(pfData);
			snprintf(str_gyro,100,"\033\143 X-axis = %.2f      Y-axis = %.2f      Z-axis = %.2f \r", pfData[0],pfData[1],pfData[2]);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_gyro,sizeof(str_gyro),10);
			memset(str_gyro, 0, sizeof(str_gyro));
			flag_gyro = 0;
		}
	}
}
/*********************************************************** END **************************************************************************/

/******************************************This function use for extracting MAGNETOMETER data *********************************************/
void f_MAGNETOMETERR(void)
{
	if(flag_mag)
	{
		if(BSP_MAGNETO_Init())
		{
			BSP_MAGNETO_GetXYZ(pDataXYZ);

			snprintf(str_acc1,100,"\033\143 X-axis Error	Y-axis Error	Z-axis Error \r");
			HAL_UART_Transmit(&huart1,( uint8_t * )str_acc1,sizeof(str_acc1),10);
			memset(str_acc1, 0, sizeof(str_acc1));

		}
		else
		{
			BSP_MAGNETO_GetXYZ(pDataXYZ);

			snprintf(str_acc1,100,"\033\143 X-axis = %d      Y-axis = %d      Z-axis = %d \r", pDataXYZ[0],pDataXYZ[1],pDataXYZ[2]);
			HAL_UART_Transmit(&huart1,( uint8_t * )str_acc1,sizeof(str_acc1),10);
			memset(str_acc1, 0, sizeof(str_acc1));
			flag_mag = 0;
		}
	}
}
/*********************************************************** END **************************************************************************/


/*********************************************************** This function use for Cursor Move ********************************************/
void f_cur_mov(void)
{
	f_INT_count++;

	if(f_INT_count <= 8)											//Main Menu
	{
		HAL_UART_Transmit(&huart1,com_dn,sizeof(com_dn),10);
	}
	else if(f_INT_count == 9)
	{
		HAL_UART_Transmit(&huart1,com_up,sizeof(com_up),10);
		f_INT_count=1;
	}
	else if(f_INT_count <= 13)										//NFC Menu
	{
		HAL_UART_Transmit(&huart1,com_dn,sizeof(com_dn),10);
	}
	else if(f_INT_count == 14)
	{
		HAL_UART_Transmit(&huart1,N_com_up,sizeof(N_com_up),10);
		f_INT_count=10;
	}
	else if(f_INT_count <= 20)										//NFC Write Menu
	{
		HAL_UART_Transmit(&huart1,com_dn,sizeof(com_dn),10);
	}
	else if(f_INT_count == 21)
	{
		HAL_UART_Transmit(&huart1,W_com_up,sizeof(W_com_up),10);
		f_INT_count=15;
	}
	else if(f_INT_count <= 26)										//NFC Read Menu
	{
		HAL_UART_Transmit(&huart1,com_dn,sizeof(com_dn),10);
	}
	else if(f_INT_count == 27)
	{
		HAL_UART_Transmit(&huart1,R_com_up,sizeof(R_com_up),10);
		f_INT_count=22;
	}




}
/*********************************************************** END **************************************************************************/

/*********************************************** This function use for Cursor Select ******************************************************/
void f_cur_sel(void)
{
	switch(f_INT_count)
	{
		case 1:
		{
			s_case = f_One;
			break;
		}
		case 2:
		{
			s_case =  f_Two;
			break;
		}
		case 3:
		{
			s_case = f_Three;
			break;
		}
		case 4:
		{
			s_case = f_Four;
			break;
		}
		case 5:
		{
			s_case = f_Five;
			break;
		}
		case 6:
		{
			s_case = f_Six;
			break;
		}
		case 7:
		{
			s_case = f_Seven;
			break;
		}
		case 8:
		{
			s_case	= f_Eight;
			break;
		}
		case 10:
		{
			s_case	= f_One;
			break;
		}
		case 11:
		{
			s_case =  f_Two;
			break;
		}
		case 12:
		{
			s_case = f_Three;
			break;
		}
		case 13:
		{
			s_case = f_Four;
			break;
		}
		case 15:
		{
			s_case	= f_One;
			break;
		}
		case 16:
		{
			s_case =  f_Two;
			break;
		}
		case 17:
		{
			s_case = f_Three;
			break;
		}
		case 18:
		{
			s_case = f_Four;
			break;
		}
		case 19:
		{
			s_case = f_Five;
			break;
		}
		case 20:
		{
			s_case = f_Six;
			break;
		}
		case 21:
		{
			s_case = f_Seven;
			break;
		}
		case 22:
		{
			s_case	= f_One;
			break;
		}
		case 23:
		{
			s_case =  f_Two;
			break;
		}
		case 24:
		{
			s_case = f_Three;
			break;
		}
		case 25:
		{
			s_case = f_Four;
			break;
		}
		case 26:
		{
			s_case = f_Five;
			break;
		}
		case 27:
		{
			s_case = f_Six;
			break;
		}
	}
}
/*********************************************************** END **************************************************************************/
/****************************************** This function use for Printing MENU ***********************************************************/
int f_Menu(void)
{
	HAL_UART_Transmit(&huart1,msg3,sizeof(msg3),100);
	f_INT_count=1;
	sw_flag = 1;
	s_case = 0;

	f_Menu_flag		= 1;
	f_NFC_flag		= 0;
	f_NFC_W_flag	= 0;
	f_NFC_R_flag	= 0;


	return 0;
}
/*********************************************************** END **************************************************************************/

/************************************************ This function use for Switch  ***********************************************************/
int f_Switch(void)
{

	f_reading = HAL_GPIO_ReadPin (GPIOC, f_sw13_Pin);

	if (f_reading != f_lastButtonState)
	{
		// reset the debouncing timer
		f_lastDebounceTime = (__HAL_TIM_GET_COUNTER(&htim17)/10);//millis
	}
	if (((__HAL_TIM_GET_COUNTER(&htim17)/10) - f_lastDebounceTime) > f_debounceDelay)
	{
	  // if the button state has changed:
	  if (f_reading != f_buttonState)
	  {
		f_buttonState = f_reading;
	  }
	}
	//printf("button stage : %d\n",f_buttonState)  //when switch is pressed


	if ((f_buttonState == 0) && (f_flag2 == 0))
	{
	  f_presstime = (__HAL_TIM_GET_COUNTER(&htim17)/10); //press time in millis
	  f_flag1 = 0;
	  f_flag2 = 1;
	  f_tapCounter++; //tap counter will increase by 1
	  //printf("Button Press : %d\n",f_tapCounter);
	}


	//when button is released
	if ((f_buttonState == 1) && (f_flag1 == 0))
	{
	  f_releasetime = (__HAL_TIM_GET_COUNTER(&htim17)/10); //release time in millis
	  f_flag1 = 1;
	  f_flag2 = 0;
	  f_timediff = f_releasetime - f_presstime; //the time gap between press and release
	  //printf(" Time gap between Button press and release : %d\n",f_timediff);
	}


  //wait for some time and if sw is in release position
	if (((__HAL_TIM_GET_COUNTER(&htim17)/10)- f_presstime) > 400 && f_buttonState == 1)
	{
		  if (f_tapCounter == 1) //if tap counter is 1
		  {
			if (f_timediff >= 400) //if time diff is larger than 400 then its a hold
			{
				printf("Long press\n");
				f_cur_sel();
			}
			else //if timediff is less than 400 then its a single tap
			{
				printf("single tap\n");
				if(sw_flag)
				{
					f_cur_mov();
				}
			}
		  }
		  else if (f_tapCounter == 2 ) //if tapcounter is 2
		  {
				printf("double tap\n");
				s_case = f_escape;
		  }

	  f_tapCounter = 0;
	}

	f_lastButtonState = f_reading;

	return 0;
}
/*********************************************************** END **************************************************************************/


/******************************************** This function use for Printing Invalid ******************************************************/
int f_Invalid(void)
{

	if(flag_error)
	{
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)Invalid,strlen(Invalid));
		flag_error = 0;
	}

return 0;
}
/*********************************************************** END **************************************************************************/


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	/*This function use for extracting Temperature data */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */

	// Start timer
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start(&htim17);

	BSP_TSENSOR_Init();
	BSP_HSENSOR_Init();
	BSP_PSENSOR_Init();
	BSP_ACCELERO_Init();
	BSP_GYRO_Init();
	BSP_MAGNETO_Init();
	VL53L0X_PROXIMITY_Init();

	while (TT4_Init() != SUCCESS);

	HAL_UART_Transmit(&huart1,msg1,sizeof(msg1),1000);
	HAL_UART_Transmit(&huart1,msg2,sizeof(msg2),1000);

	s_case = f_escape;

	HAL_UART_Receive_IT(&huart1,rxData,1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  NFC_check = HAL_GPIO_ReadPin (GPIOE, M24SR64_Y_GPO_Pin);
	  f_Switch();

	  /*  This if condition is use for UART1 (Interrupt based) */
		if(newMsg)
		{

			size = strlen(rxBuffer);
			printf("size demo : %d\n",size );

			if(size == f_size)
			{
				s_case= rxBuffer[0];
				printf("%d\n",s_case);
			}
			else
			{
				s_case = f_error;
			}

			memset(rxBuffer, 0, sizeof(rxBuffer));
			newMsg=0;
		}

		/* This switch case is using for calling Sensor functions */
		if(f_Menu_flag)												// For main Menu
		{
			switch(s_case)
			{
				case 0:
				{
					break;
				}
				case f_One:
				{
					sw_flag=0;
					f_Temperature();
					break;
				}
				case f_Two:
				{
					sw_flag=0;
					f_Humidity();
					break;
				}
				case f_Three:
				{

					sw_flag=0;
					f_Pressure();
					break;
				}
				case f_Four:
				{
					sw_flag=0;
					f_ACCELEROMETER();
					break;
				}
				case f_Five:
				{
					sw_flag=0;
					f_GYRO();
					break;
				}
				case f_Six:
				{
					sw_flag=0;
					f_MAGNETOMETERR();
					break;
				}
				case f_Seven:
				{
					sw_flag=0;
					Proximity_Test();
					break;
				}
				case f_Eight:
				{
					sw_flag=0;
					f_NFC_flag=1;
					f_NFC();
					s_case=0;
					break;
				}

				case f_escape:
				{
					sw_flag=0;
					f_Menu();
					break;
				}
				default :
				{
					f_Invalid();
				}
			}
		}

	/****************************************************/

		if(f_NFC_flag)												// For main NFC Menu
		{
			switch(s_case)
			{
				case 0:
				{
					break;
				}
				case f_One:
				{
					f_NFC_D();
					//s_case=0;
					break;
				}
				case f_Two:
				{
					f_NFC_W();
					s_case=0;
					break;
				}
				case f_Three:
				{
					f_NFC_R();
					s_case=0;
					break;
				}
				case f_Four:
				{
					sw_flag=0;
					f_Menu();
					break;
				}
				case f_escape:
				{
					sw_flag=0;
					f_Menu();
					break;
				}
				default :
				{
					f_Invalid();
				}
			}
		}

		if(f_NFC_W_flag)											// For main NFC Write Menu
		{
			switch(s_case)
			{
				case 0:
				{
					break;
				}
				case f_One:
				{
					s_case=0;
					NFC_examples[0].DemoFunc();
					break;
				}
				case f_Two:
				{
					s_case=0;
					NFC_examples[2].DemoFunc();
					break;
				}
				case f_Three:
				{
					s_case=0;
					NFC_examples[4].DemoFunc();
					break;
				}
				case f_Four:
				{
					s_case=0;
					NFC_examples[6].DemoFunc();
					break;
				}
				case f_Five:
				{
					s_case=0;
					NFC_examples[8].DemoFunc();
					break;
				}
				case f_Six:
				{
					sw_flag=0;
					f_NFC_flag=1;
					f_NFC();
					s_case=0;
					break;
				}
				case f_escape:
				{
					sw_flag=0;
					f_Menu();
					break;
				}
				default :
				{
					f_Invalid();
				}

			}
		}
		if(f_NFC_R_flag)
		{
			switch(s_case)
			{
				case 0:
				{
					break;
				}
				case f_One:
				{
					s_case=0;
					NFC_examples[1].DemoFunc();
					break;
				}
				case f_Two:
				{
					s_case=0;
					NFC_examples[3].DemoFunc();
					break;
				}
				case f_Three:
				{
					s_case=0;
					NFC_examples[5].DemoFunc();
					break;
				}
				case f_Four:
				{
					s_case=0;
					NFC_examples[7].DemoFunc();
					break;
				}
				case f_Five:
				{
					sw_flag=0;
					f_NFC_flag=1;
					f_NFC();
					s_case=0;
					break;
				}
				case f_escape:
				{
					sw_flag=0;
					f_Menu();
					break;
				}
				default :
				{
					f_Invalid();
				}
			}
		}
  }


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10909CEC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 8000-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = f_count;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 8000-1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 65535;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, M24SR64_Y_RF_DISABLE_Pin|ISM43362_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ARD_D10_Pin|SPBTLE_RF_RST_Pin|ARD_D9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ARD_D8_Pin|ISM43362_BOOT0_Pin|ISM43362_WAKEUP_Pin|LED14_Pin
                          |SPSGRF_915_SDN_Pin|ARD_D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USB_OTG_FS_PWR_EN_Pin|PMOD_RESET_Pin|STSAFE_A100_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPBTLE_RF_SPI3_CSN_GPIO_Port, SPBTLE_RF_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, VL53L0X_XSHUT_Pin|LED3_WIFI__LED4_BLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPSGRF_915_SPI3_CSN_GPIO_Port, SPSGRF_915_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : M24SR64_Y_RF_DISABLE_Pin ISM43362_RST_Pin ISM43362_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = M24SR64_Y_RF_DISABLE_Pin|ISM43362_RST_Pin|ISM43362_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_OVRCR_EXTI3_Pin SPSGRF_915_GPIO3_EXTI5_Pin SPBTLE_RF_IRQ_EXTI6_Pin ISM43362_DRDY_EXTI1_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_OVRCR_EXTI3_Pin|SPSGRF_915_GPIO3_EXTI5_Pin|SPBTLE_RF_IRQ_EXTI6_Pin|ISM43362_DRDY_EXTI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : M24SR64_Y_GPO_Pin */
  GPIO_InitStruct.Pin = M24SR64_Y_GPO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(M24SR64_Y_GPO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : f_sw13_Pin */
  GPIO_InitStruct.Pin = f_sw13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(f_sw13_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_A5_Pin ARD_A4_Pin ARD_A3_Pin ARD_A2_Pin
                           ARD_A1_Pin ARD_A0_Pin */
  GPIO_InitStruct.Pin = ARD_A5_Pin|ARD_A4_Pin|ARD_A3_Pin|ARD_A2_Pin
                          |ARD_A1_Pin|ARD_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D1_Pin ARD_D0_Pin */
  GPIO_InitStruct.Pin = ARD_D1_Pin|ARD_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D10_Pin SPBTLE_RF_RST_Pin ARD_D9_Pin */
  GPIO_InitStruct.Pin = ARD_D10_Pin|SPBTLE_RF_RST_Pin|ARD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D4_Pin */
  GPIO_InitStruct.Pin = ARD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(ARD_D4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D7_Pin */
  GPIO_InitStruct.Pin = ARD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D7_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D13_Pin ARD_D12_Pin ARD_D11_Pin */
  GPIO_InitStruct.Pin = ARD_D13_Pin|ARD_D12_Pin|ARD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D3_Pin */
  GPIO_InitStruct.Pin = ARD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D6_Pin */
  GPIO_InitStruct.Pin = ARD_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D8_Pin ISM43362_BOOT0_Pin ISM43362_WAKEUP_Pin LED14_Pin
                           SPSGRF_915_SDN_Pin ARD_D5_Pin SPSGRF_915_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = ARD_D8_Pin|ISM43362_BOOT0_Pin|ISM43362_WAKEUP_Pin|LED14_Pin
                          |SPSGRF_915_SDN_Pin|ARD_D5_Pin|SPSGRF_915_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DFSDM1_DATIN2_Pin DFSDM1_CKOUT_Pin */
  GPIO_InitStruct.Pin = DFSDM1_DATIN2_Pin|DFSDM1_CKOUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : QUADSPI_CLK_Pin QUADSPI_NCS_Pin OQUADSPI_BK1_IO0_Pin QUADSPI_BK1_IO1_Pin
                           QUAD_SPI_BK1_IO2_Pin QUAD_SPI_BK1_IO3_Pin */
  GPIO_InitStruct.Pin = QUADSPI_CLK_Pin|QUADSPI_NCS_Pin|OQUADSPI_BK1_IO0_Pin|QUADSPI_BK1_IO1_Pin
                          |QUAD_SPI_BK1_IO2_Pin|QUAD_SPI_BK1_IO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : INTERNAL_UART3_TX_Pin INTERNAL_UART3_RX_Pin */
  GPIO_InitStruct.Pin = INTERNAL_UART3_TX_Pin|INTERNAL_UART3_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LPS22HB_INT_DRDY_EXTI0_Pin LSM6DSL_INT1_EXTI11_Pin ARD_D2_Pin HTS221_DRDY_EXTI15_Pin
                           PMOD_IRQ_EXTI12_Pin */
  GPIO_InitStruct.Pin = LPS22HB_INT_DRDY_EXTI0_Pin|LSM6DSL_INT1_EXTI11_Pin|ARD_D2_Pin|HTS221_DRDY_EXTI15_Pin
                          |PMOD_IRQ_EXTI12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_PWR_EN_Pin SPBTLE_RF_SPI3_CSN_Pin PMOD_RESET_Pin STSAFE_A100_RESET_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin|SPBTLE_RF_SPI3_CSN_Pin|PMOD_RESET_Pin|STSAFE_A100_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : VL53L0X_XSHUT_Pin */
  GPIO_InitStruct.Pin = VL53L0X_XSHUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(VL53L0X_XSHUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : VL53L0X_GPIO1_EXTI7_Pin LSM3MDL_DRDY_EXTI8_Pin */
  GPIO_InitStruct.Pin = VL53L0X_GPIO1_EXTI7_Pin|LSM3MDL_DRDY_EXTI8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED3_WIFI__LED4_BLE_Pin */
  GPIO_InitStruct.Pin = LED3_WIFI__LED4_BLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED3_WIFI__LED4_BLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OTG_FS_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_ID_Pin USB_OTG_FS_DM_Pin USB_OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_ID_Pin|USB_OTG_FS_DM_Pin|USB_OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INTERNAL_SPI3_SCK_Pin INTERNAL_SPI3_MISO_Pin INTERNAL_SPI3_MOSI_Pin */
  GPIO_InitStruct.Pin = INTERNAL_SPI3_SCK_Pin|INTERNAL_SPI3_MISO_Pin|INTERNAL_SPI3_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PMOD_SPI2_SCK_Pin */
  GPIO_InitStruct.Pin = PMOD_SPI2_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PMOD_SPI2_SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PMOD_UART2_CTS_Pin PMOD_UART2_RTS_Pin PMOD_UART2_TX_Pin PMOD_UART2_RX_Pin */
  GPIO_InitStruct.Pin = PMOD_UART2_CTS_Pin|PMOD_UART2_RTS_Pin|PMOD_UART2_TX_Pin|PMOD_UART2_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D15_Pin ARD_D14_Pin */
  GPIO_InitStruct.Pin = ARD_D15_Pin|ARD_D14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart -> Instance == USART1)
    {
    	if(rxData[0]!='\r')
        {
            rxBuffer[rxIndex]=rxData[0];
            rxIndex++;
        }
        else
        {
            newMsg=1;
            rxIndex=0;
        }
        HAL_UART_Receive_IT(&huart1 ,rxData,1);
    }

}
/*This Callback function use for generating Timer(timer 16) delay  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check the timer triggered this callback
	if(htim -> Instance == TIM16)
	{
		flag_temp  		= 1;							//checking the timer callback
		flag_humi  		= 1;
		flag_pre   		= 1;
		flag_acce  		= 1;
		flag_gyro  		= 1;
		flag_mag   		= 1;
		flag_pro   		= 1;
		flag_Email 		= 1;
		flag_R_Email 	= 1;
		flag_Vcard		= 1;
		flag_R_Vcard	= 1;
		flag_URL		= 1;
		flag_R_URL		= 1;
		flag_aar		= 1;
		flag_sms		= 1;
		flag_NFC		= 1;
		flag_R_sms		= 1;
		flag_error 		= 1;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


  if(GPIO_Pin == NFC_GPIO_GPO_PIN)
  {
	  printf("in HAL_GPIO_EXTI_Callback\n");
	  M24SR_GPO_Callback();
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  //s_case = f_escape;
	  //f_Switch();
  }
  //HAL_UART_Transmit(&huart1,( uint8_t * )"\033\143 NFC Error\r\n",sizeof("\033\143 NFC Error\r\n"),10);
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
