#ifndef __RK_FD655H__
#define __RK_FD655H__

#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <linux/io.h>

#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/of.h>
//#include <linux/switch.h>
#include <linux/pinctrl/consumer.h>
#include <linux/fb.h>
#include <linux/time.h>



typedef unsigned char   u_int8;
typedef unsigned short  u_int16;	
typedef unsigned long 	u_int32;



#if 0
#define pr_dbg(args...) printk(KERN_ALERT "FD650: " args)
#else
#define pr_dbg(args...)
#endif

#define pr_error(args...) printk(KERN_ALERT "FD650: " args)

#ifndef CONFIG_OF
#define CONFIG_OF
#endif

#define MOD_NAME_CLK       "fd650_clk"
#define MOD_NAME_DAT       "fd650_dat"
#define DEV_NAME           "fd650_dev"


struct fd650_dev{
	int clk_pin;    
	int dat_pin;
	char wbuf[5];
	struct class sysfs;
    int vplay_flag;
    int vpause_flag;
    struct notifier_block notifier;
};

#define KEYBOARD_SCAN_FRE      2
#define KEYBOARD_MARK         0x00


#define POLL_PERIOD_WHEN_KEY_DOWN 10 /* unit msec */
#define POLL_PERIOD_WHEN_KEY_UP   50



#define		DELAY				udelay(10)	///<Delay time sequence macros, control the rate of SCL and SDA, suggested that in more than 5 us

#define LEDMAPNUM 63

/** Character conversion of digital tube display code*/
typedef struct _led_bitmap
{
	u_int8 character;
	u_int8 bitmap;
} led_bitmap;

/** Character conversion of digital tube display code array*/
const led_bitmap BCD_decode_tab[LEDMAPNUM] = 
{	
#if 1
	{'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
	{'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
	{'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
	{'b', 0x7C}, {'B', 0x7C}, {'c', 0x58}, {'C', 0x39},
	{'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
	{'f', 0x71}, {'F', 0x71}, {'I', 0X60}, {'i', 0x60},
	{'L', 0x38}, {'l', 0x38}, {'r', 0x38}, {'R', 0x38},
	{'n', 0x54}, {'N', 0x37}, {'O', 0x3F}, {'o', 0x3f},
	{'p', 0xf3}, {'P', 0x38}, {'S', 0x6D}, {'s', 0x6d},
	{'y', 0x6e}, {'Y', 0x6E}, {'_', 0x08}, {0,   0x3F}, 
	{1,   0x06}, {2,   0x5B}, {3,   0x4F}, {4,   0x66}, 
	{5,   0x6D}, {6,   0x7D}, {7,   0x07}, {8,   0x7F}, 
	{9,   0x6F}, {'!', 0X01}, {'@', 0X02}, {'#', 0X04},
	{'$', 0X08}, {':', 0X10}, {'^', 0X20}, {'&', 0X40},
	{0xC5,0X00}, {0x3b,0x18}, {0xc4,0x08}, {0x3c,0x14},
	{0xc3,0x04}, {0x3d,0x1c}, {0xc2,0x0c},
#else
    {'0', 0x3F}, {'1', 0x30}, {'2', 0x5B}, {'3', 0x79},
	{'4', 0x74}, {'5', 0x6D}, {'6', 0x6F}, {'7', 0x38},
	{'8', 0x7F}, {'9', 0x7D}, {'a', 0x77}, {'A', 0x77},
	{'b', 0x67}, {'B', 0x67}, {'c', 0x58}, {'C', 0x39},
	{'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
	{'f', 0x71}, {'F', 0x71}, {'o', 0x63}, {'O', 0x63}, 
    {'t', 0x47}, {'T', 0x47}, {':', 0X10}, {0,   0x3F}, 
	{1,   0x30}, {2,   0x5B}, {3,   0x79}, {4,   0x74}, 
	{5,   0x6D}, {6,   0x6F}, {7,   0x38}, {8,   0x7F}, 
	{9,   0x7D}

#endif 	
};//BCD����ӳ��


// ����ϵͳ��������

#define SET     	0x48        // ����??2?��y
#define DISP		0x01		// ?a????��???
#define SLEEP		0x04		// ?��????????
#define SEG7		0x08

#define FD650_BIT_INTENS1	0x10		///< 1������
#define FD650_BIT_INTENS2	0x20		///< 2������
#define FD650_BIT_INTENS3	0x30		///< 3������
#define FD650_BIT_INTENS4	0x40		///< 4������
#define FD650_BIT_INTENS5	0x50		///< 5������
#define FD650_BIT_INTENS6	0x60		///< 6������
#define FD650_BIT_INTENS7	0x70		///< 7������
#define FD650_BIT_INTENS8	0x00		///< 8������

#define FD650_SYSON		( DISP)	///< ������ʾ������

#define DIG1		0x68		// ��y??1��??0??��?
#define DIG2		0x6a		// ��y??1��??1??��?
#define DIG3		0x6c		// ��y??1��??2??��?
#define DIG4		0x6e		// ��y??1��??3??��?

#define FD650_7SEG_ON	( FD650_SYSON | SEG7 )	///< �����߶�ģʽ
#define FD650_8SEG_ON	( FD650_SYSON | 0x00 )	///< �����˶�ģʽ
#define FD650_SYSON_1	( FD650_SYSON | FD650_BIT_INTENS1 )	///< ������ʾ�����̡�1������
#define FD650_SYSON_2	( FD650_SYSON | FD650_BIT_INTENS2 )
#define FD650_SYSON_3	( FD650_SYSON | FD650_BIT_INTENS3 )
//�Դ�����
#define FD650_SYSON_4	( FD650_SYSON | FD650_BIT_INTENS4 )	///< ������ʾ�����̡�4������
#define FD650_SYSON_5	( FD650_SYSON | FD650_BIT_INTENS5 )
#define FD650_SYSON_6	( FD650_SYSON | FD650_BIT_INTENS6 )
#define FD650_SYSON_7	( FD650_SYSON | FD650_BIT_INTENS7 )
//�Դ�����
#define FD650_SYSON_8	( FD650_SYSON | FD650_BIT_INTENS8 )	///< ������ʾ�����̡�8������


#define FD650_DOT			0x0080			///< �����С������ʾ

// ��ȡ������������
#define FD650_GET_KEY	0x0700					///< ��ȡ����,���ذ�������


// �ⲿ����
void FD650_command( u_int8 cmd , struct fd650_dev *dev )	;	///< ��FD650������������
void FD650_disp( u_int8 address, u_int8 dat, struct fd650_dev *dev )	;	///< ��FD650������������

extern u_int8 Led_Get_Code(char cTemp);

#endif

