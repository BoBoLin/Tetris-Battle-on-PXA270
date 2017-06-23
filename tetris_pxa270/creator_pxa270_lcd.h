#ifndef _CREATOR_S3C2410_LCD_H_
#define _CREATOR_S3C2410_LCD_H_

//#include <linux/config.h>
#if defined(__linux__)
  #include <asm/ioctl.h> /* For _IO* macros */
  #define LCD_IOCTL_NR(n) _IOC_NR(n)
#elif defined(__FreeBSD__)
  #include <sys/ioccom.h>
  #define LCD_IOCTL_NR(n) ((n) & 0xff)
#endif

#define LCD_MAJOR_NUM 120
#define LCD_IOCTL_MAGIC LCD_MAJOR_NUM
#define LCD_IO(nr) _IO(LCD_IOCTL_MAGIC,nr)
#define LCD_IOR(nr,size) _IOR(LCD_IOCTL_MAGIC,nr,size)
#define LCD_IOW(nr,size) _IOW(LCD_IOCTL_MAGIC,nr,size)
#define LCD_IOWR(nr,size) _IOWR(LCD_IOCTL_MAGIC,nr,size)




// function headers

typedef struct lcd_write_info { /* for LCD */
  unsigned char Msg[512]
 ; /* ���J���r���s���ϡA�̤j512 */
  unsigned short Count; /* ���ڿ��J���r������ */
  int CursorX, CursorY;/* Ū�� cursor �]�w���Ф�X, Y */
} lcd_write_info_t;

typedef struct lcd_full_image_info {
  unsigned short data[0x800]; /* LCD���Ӹ��� */
} lcd_full_image_info_t;

// ���ЦU�ؾާ@�Ҧ�
#define WHITE_BLINK_CURSOR 0 // �զ��{�{����
#define BLACK_BLINK_CURSOR 1 // �¦��{�{����
#define REVERSE_CURSOR 2 // �¥ե洫������
#define REVERSE_BLINK_CURSOR 3 // �¥հ{�{�洫������

#define TABS 4 // �h�֪ťզr�N��TAB

/* Key Pad scan code define */
#define VK_S2 1 /* ASCII = '1' */
#define VK_S3 2 /* ASCII = '2' */
#define VK_S4 3 /* ASCII = '3' */
#define VK_S5 10 /* ASCII = 'A' */
#define VK_S6 4 /* ASCII = '4' */
#define VK_S7 5 /* ASCII = '5' */
#define VK_S8 6 /* ASCII = '6' */
#define VK_S9 11 /* ASCII = 'B' */
#define VK_S10 7 /* ASCII = '7' */
#define VK_S11 8 /* ASCII = '8' */
#define VK_S12 9 /* ASCII = '9' */
#define VK_S13 12 /* ASCII = 'C' */
#define VK_S14 14 /* ASCII = '*' */
#define VK_S15 0 /* ASCII = '0' */
#define VK_S16 15 /* ASCII = '#' */
#define VK_S17 13 /* ASCII = 'D' */

enum {
  VK_CHAR_0 = 0, VK_CHAR_1, VK_CHAR_2, VK_CHAR_3,
  VK_CHAR_4, VK_CHAR_5, VK_CHAR_6, VK_CHAR_7,
  VK_CHAR_8, VK_CHAR_9, VK_CHAR_A, VK_CHAR_B,
  VK_CHAR_C, VK_CHAR_D, VK_CHAR_STAR, VK_CHAR_POND
};

/* LED define */
#define LED_ALL_ON 0xFF /* �I�GLED Lamp */
#define LED_ALL_OFF 0x00 /* ����LED Lamp */
#define LED_D9_INDEX 0 /* LED �s��D9 (1) */
#define LED_D10_INDEX 1 /* LED �s��D10(2) */
#define LED_D11_INDEX 2 /* LED �s��D11(3) */
#define LED_D12_INDEX 3 /* LED �s��D12(4) */
#define LED_D13_INDEX 4 /* LED �s��D13(5) */
#define LED_D14_INDEX 5 /* LED �s��D14(6) */
#define LED_D15_INDEX 6 /* LED �s��D15(7) */
#define LED_D16_INDEX 7 /* LED �s��D16(8) */

/* 4 Digits 7 Segment LED */
#define _7SEG_D5_INDEX 8 /* Segment �s��D5 (1) */
#define _7SEG_D6_INDEX 4 /* Segment �s��D6 (2) */
#define _7SEG_D7_INDEX 2 /* Segment �s��D7 (3) */
#define _7SEG_D8_INDEX 1 /* Segment �s��D8 (4) */
#define _7SEG_ALL (_7SEG_D5_INDEX|_7SEG_D6_INDEX|_7SEG_D7_INDEX|_7SEG_D8_INDEX)

#define _7SEG_MODE_PATTERN 0 /* �ۤv���w�Ȩ�����,1 : �G, 0 : ���G */
/* �C��Segment����1��byte, �@4��bytes */
/* Segment A : bit 0 */
/* Segment H : bit 7 */
/* D5(1)�OHigh byte, D8(4)�Olow byte */
/* D5(1)�OHigh byte, D8(4)�Olow byte */
#define _7SEG_MODE_HEX_VALUE 1 /* �ϥ�Hexadecimal, �������J���� */
/* �C�Ӧ���1��nibble, �@2��bytes */
/* D5(1)�Ohigh nibble of the 3th byte */
/* D8(4)�Olow nibble of the low byte */
typedef struct _7Seg_Info {
  unsigned char Mode; /* �]�w���J�����ƼҦ� */
  unsigned char Which
 ; /* ���w���ƱN�]�w����Segment, */
  /* �ϥ�or���Ȩӫ��wSegment */
  /* �p�G���O�����h���� */
  /* �̥k�䪺Segment�����Ʀb�̧C��byte */
  /* or nibble */
  /* �Ҧp : D5, D8����58 */
  /* Mode = _7SEG_MODE_HEX_VALUE */
  /* Which = _7SEG_D5_INDEX | _7SEG_D8_INDEX */
  /* Value = 0x58 */

  unsigned long Value; /* �U��Mode�����J�� */
} _7seg_info_t;

/* LCD specific ioctls */
/* �M��LCD�W�����ƨåB���Ц^�쥪�W�� */
#define LCD_IOCTL_CLEAR LCD_IO( 0x0)
/* �g�r����LCD �W*/
#define LCD_IOCTL_WRITE LCD_IOW( 0x01, lcd_write_info_t)
#define LCD_IOCTL_CUR_ON LCD_IO( 0x02)
#define LCD_IOCTL_CUR_OFF LCD_IO( 0x03)
#define LCD_IOCTL_CUR_GET LCD_IOR( 0x04, lcd_write_info_t)
#define LCD_IOCTL_CUR_SET LCD_IOW( 0x05, lcd_write_info_t)
#define LCD_IOCTL_DRAW_FULL_IMAGE LCD_IOW( 0x06, lcd_full_image_info_t)

/* LED specific ioctls */
/* �]�w8��LED Lamps, Low byte �Ȭ����� */
#define LED_IOCTL_SET LCD_IOW( 0x40, unsigned short)
/* �I�G���@��LED lamp */
#define LED_IOCTL_BIT_SET LCD_IOW( 0x41, unsigned short)
/* �������@��LED lamp*/
#define LED_IOCTL_BIT_CLEAR LCD_IOW( 0x42, unsigned short)

/* DIP specific ioctls */
/* ��Switch�ը�ON�ɩҶǦ^�Ȭ� */
/* Ū��DIP SW�����A,bit 0�O1 bit 7�O8 */
#define DIPSW_IOCTL_GET LCD_IOR( 0x50, unsigned short)

/* Keypad specific ioctls */
/* �Ǧ^���䪺�� */
#define KEY_IOCTL_GET_CHAR LCD_IOR( 0x60, unsigned short)
/* ���ݨ즳����, �~�Ǧ^��*/
#define KEY_IOCTL_WAIT_CHAR LCD_IOR( 0x61, unsigned short)
/* �ˬd�O�_�����J���� */
#define KEY_IOCTL_CHECK_EMTPY LCD_IOR( 0x62, unsigned short)
/* �M��keybuffer �Ҧ�����*/
#define KEY_IOCTL_CLEAR LCD_IO( 0x63)
/* �������ݿ��J���� */
#define KEY_IOCTL_CANCEL_WAIT_CHAR LCD_IO( 0x64)

/* 7SEG LED specific ioctls */
/* �Ұ� 7 Segment LED */
#define _7SEG_IOCTL_ON LCD_IO( 0x70)
/* ���� 7 Segment LED */
#define _7SEG_IOCTL_OFF LCD_IO( 0x71)
/* �]�w 7 Segment LED*/
#define _7SEG_IOCTL_SET LCD_IOW( 0x72, _7seg_info_t)

typedef struct _Timer_Info {
  unsigned char tick_sec;
  unsigned char tick_min;
  unsigned char tick_hour;

} _Timer_info_t;

// get creator timer
#define _TIME_IOCTL_GET LCD_IOR(0x73, _Timer_info_t)

// set creator timer
#define _TIME_IOCTL_SET LCD_IOW(0x74, _Timer_info_t)

#endif // _CREATOR_S3C2410_LCD_H_
