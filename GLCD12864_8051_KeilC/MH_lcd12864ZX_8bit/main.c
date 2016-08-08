/*******************************************************************
  * @file 		main.c
  * @author  	VoAnhKha
  * @version 	V1.0.0
  * @date   
  * @brief  	File chuong trinh chinh
	
	
	//Program to interface Serial EEPROM AT24C02 with 8051 microcontroller (AT89C51) 
// link at http://www.engineersgarage.com/microcontroller/8051projects/interface-serial-eeprom-24c02-AT89C51-circuit
********************************************************************/

#include <main.h>
#include <intrins.h>   //For using [_nop_()]

unsigned char i, j, y, butflag, image_addr=0x00, milestone=0x00 ;
unsigned char kha[16];

sbit sda=P3^5;
sbit scl=P3^4;	
bit ack;
sbit led1=P1^1;
sbit but1=P0^5;
sbit but2=P0^6;
sfr lcd_data_pin=0xA0;//p2 port
sfr output=0x80;//p0 port

unsigned char reead,write,write2,i,j,k, m, n;

unsigned int temp;

void delay(unsigned int count)
{
	int i,j;
	for(i=0;i<count;i++)
		for(j=0;j<1275;j++);
}


void aknowledge()	  //acknowledge condition
{
	scl=1;
	_nop_();
	_nop_();
	scl=0;
} 

void noack()	  //no acknowledge condition
{
	sda = 1;
	scl=1;
	_nop_();
	_nop_();
	scl=0;
	sda = 0;
} 
void start()		//start condition
{
	sda=1;
	scl=1;
	_nop_(); // START SETUP TIME    
	_nop_();  
	_nop_();     
	_nop_(); 	
	sda=0;
	_nop_(); // START HOLD TIME    
	_nop_();
		_nop_();
		_nop_();
	scl=0;
}
void stop()			//stop condition
{
	sda=0;
	scl=1;
	_nop_();
	_nop_(); // STOP SETUP TIME3
	_nop_();
	_nop_();
	sda=1;
	scl=0;
}
void send_byte(unsigned char value)	//send byte serially
{ 
	unsigned int i;
	unsigned char send;
	send=value;
	for(i=0;i<8;i++)
	{
		sda=send/128;			//extracting MSB
		send=send<<1;			//shiftng left
		scl=1;
		_nop_();
		_nop_();
		scl=0;
	}
   ack=sda;					//reading acknowledge
   sda=0;
}
unsigned char read_byte()			//reading from EEPROM serially
{
	unsigned int i;
	sda=1;
	reead=0;
	for(i=0;i<8;i++)
	{
		reead=reead<<1;
		scl=1;
		_nop_();
		_nop_();
		if(sda==1)
			reead++;
		scl=0;
	}
	sda=0;
	return reead;				//Returns 8 bit data here
}		


void Read16bytes(unsigned char high_addr, unsigned char low_addr)
{
	start();
	send_byte(0xA0);
	aknowledge();
	send_byte(high_addr);
	aknowledge();
	send_byte(low_addr);
	aknowledge();
	start();
	send_byte(0xA1);					 //device address
	aknowledge();
	
	for(y=0;y<15;y++)	
		{
		kha[y] = read_byte();
		aknowledge();
		}
		kha[15] = read_byte();
		noack();
	  stop();
}

unsigned char checkbut()
{ //check button 1, return 1 if pressed then released
	if(but1==0)
	{ delay_ms(30);
		if(but1==0 & but2!=0)
		{while(but1==0);
		 return 1;
	  }
		else if (but1==0 & but2==0)
		{delay_ms(3000);
			if(but1==0 & but2==0)
			{while(but1==0 & but2==0);
				return 3;
			}
		}
	}

	//check button 2, return 2 if pressed then released
		if(but2==0)
	{ delay_ms(30);
		if(but2==0 & but1!=0)
		{while(but2==0);
		 return 2;
	  }
		else if (but2==0 & but1==0)
		{delay_ms(3000);
			if(but2==0 & but1==0)
			{while(but2==0 & but1==0);
				return 3;
		  }
		}
	}
	else
	return 0;
   
}

void lcd_display_image(unsigned char highaddr)
{
	unsigned char y, z, high = highaddr, low=0x00;
	unsigned int tmp=0;
	write_command_lcd(0x34,1); //Graphic display OFF	
	
	for(z=0;z<32;z++)  //z: line number on GLCD
	{	
		Read16bytes(high, low);
		write_command_lcd((0x80 + z),1); //set vertical address x
		write_command_lcd((0x80),1);	// set horizontal address i
		write_command_lcd(0x30,1);		// basic instruction set
		for(y=0;y<16;y++)	
		{
				write_data_lcd(~kha[y]);
		}
		
		Read16bytes(high+0x02, low);
				for(y=0;y<16;y++)	
		{
				write_data_lcd(~kha[y]);
		}
		
		write_command_lcd(0x36,1); //Graphic display ON	
		
		if(low!=0xF0)
		low = low+16;
		else
		{low = 0x00;
			high = high + 0x01;
		}
	}
}
void main()
{
  delay_ms(400);				// LUU Y SU CAN THIET CUA KHOANG THOI GIAN DELAY NAY (>40ms)
	lcd_init();
	delay_ms(5);
	LCD_RST=1;
	lcd_clear();
	image_addr = 0x00;
	milestone = 0x00;
	temp=0;
	
  //Read();
	
	//	lcd_puts(1,1,p);
	//	lcd_puts(1,2,q);
	//	lcd_puts(0,3,"Happy New Year");
	//	lcd_puts(1,4,"2015");
	
	lcd_display_image(image_addr);

	while(1)
	{
		butflag = checkbut();
		if(butflag==1) 
		{if (image_addr == 0x7C) image_addr = 0x00;
		else image_addr = image_addr + 0x04;
		lcd_display_image(image_addr);
	
		}
		
		if(butflag==2) 
		{if (image_addr == 0x00) image_addr = 0x7C;
		else image_addr = image_addr - 0x04;
		lcd_display_image(image_addr);
		}
		
				if(butflag==3) 
		{if (milestone == 0x70) milestone = 0x00;
		else milestone = milestone + 0x10;
		image_addr = milestone;
		lcd_display_image(image_addr);
		}
		
		
	}
}

/* -----------------END OF FILE--------------------------- */
