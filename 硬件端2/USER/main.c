#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

#include "hc05.h"
#include "usart2.h"			 	 
#include "string.h"	 
#include "bsp_exti.h"
#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"

//ALIENTEKminiSTM32开发板扩展实验 
//ATK-HC05蓝牙串口模块实验-库函数版本  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
//ALIENTEK战舰STM32开发板实验13
//TFTLCD显示实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 	
extern int Move_gewei;
extern int Move_shiwei;
extern int Move_baiwei;
extern int Move_qianwei;
extern int Move_wanwei; 


char str[100];

//心率
uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
uint32_t un_min, un_max, un_prev_data;  
int i;
int32_t n_brightness;
float f_temp;
u8 temp_num=0;
u8 temp[6];
u8 dis_hr=0,dis_spo2=0;
int time;	
uint8_t hc05_role=0; 

#define MAX_BRIGHTNESS 255
//函数声明
void heart(void);


//显示ATK-HC05模块的主从状态
void HC05_Role_Show(void)
{
	
	if(HC05_Get_Role()==1)printf("ROLE:Master \n");	//主机
	else printf("ROLE:Slave \n");			 		//从机
	/*4. 设置蓝牙的名称*/
	if(HC05_Set_Cmd("AT+NAME=HC-05"))printf("4 蓝牙名称设置失败!\r\n");
	else printf("4 蓝牙名称设置为 HC-05 \r\n");
	/*5. 设置蓝牙配对密码*/
	if(HC05_Set_Cmd("AT+PSWD=1234"))printf("5 蓝牙配对密码设置失败!\r\n"); //密码必须是4位
	else printf("5 蓝牙配对密码设置为 1234 \r\n");
}


int main(void)
 {	 
	u8 t;
	u8 key;
	u8 sendmask=0;
	u8 sendcnt=0;
	u8 sendbuf[20];	  
	u8 reclen=0;  
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	//USART2_Init(9600);	//初始化串口2为:9600,波特率.
	 
	EXTI_Key_Config ();
	 
	max30102_init();
  printf("\r\n MAX30102  init  \r\n");	 
  //delay_ms(1500);
	//POINT_COLOR=RED;
	printf("ALIENTEK STM32 ^_^\r\n");	
	printf("HC05 BLUETOOTH COM TEST\r\n");	
	printf("ATOM@ALIENTEK\r\n");
	 
	delay_ms(1000);			//等待蓝牙模块上电稳定
	/*1. 初始化HC05串口蓝牙*/
	printf("1 蓝牙正在初始化.........\r\n"); 
	while(HC05_Init()) 		//初始化ATK-HC05模块  
	{
		printf("ATK-HC05 Error!\n"); 
		delay_ms(500);
		printf("Please Check!!!\n"); 
		delay_ms(100);
	}	 										   	   

	HC05_Role_Show();
	delay_ms(100);
	USART3_RX_STA=0; 
	  
	 
 	while(1) 
	{	
			
			 
			delay_ms(2000);
	  
		 										     				   
			
	}											    
}
 
void heart(void)
{
	 i=0;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
         
			if(dis_hr == 0 && dis_spo2 == 0)  //**dis_hr == 0 && dis_spo2 == 0
			{
				sprintf((char *)str,"HR:--- SpO2:--- ");//**HR:--- SpO2:---
				printf("HR:--- SpO2:--- Move:%d%d%d%d%d \r\n ",Move_wanwei,Move_qianwei,Move_baiwei,Move_shiwei,Move_gewei);		
			
			}
			else{
				sprintf((char *)str,"HR:%3d SpO2:%3d ",dis_hr,dis_spo2);//**HR:%3d SpO2:%3d 
				printf("HR:%3d SpO2:%3d Move:%d%d%d%d%d \r\n ",dis_hr,dis_spo2,Move_wanwei,Move_qianwei,Move_baiwei,Move_shiwei,Move_gewei);
				sprintf(str,"%d-%d-%d%d%d%d%d ",dis_hr,dis_spo2,Move_wanwei,Move_qianwei,Move_baiwei,Move_shiwei,Move_gewei);
				u3_printf(str);
			
			}
       maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
			
			
				

}
