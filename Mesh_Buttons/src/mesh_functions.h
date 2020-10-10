#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

// define GPIO number for buttons and rotary encoders. Be aware, that these are GPIO input numbers, not pin numbers of Raspberry Pi. Please refer to the official Raspberry Pi pinout diagram.
#define EncVolOutA 6
#define EncVolOutB 12
#define EncVolClick 5
#define EncNavOutA 16
#define EncNavOutB 26
#define EncNavClick 13
#define powerButton 23  // actually this button switches between "Volume Up/Down/Mute" and "Left/Right/Click" functionalities for the first encoder (EncVol). It's not a power button, it just looks like one -> hence the name.
#define backButton 17


//#define BCM2708_PERI_BASE        0x20000000
#define BCM2711_PERI_BASE        0xFE000000
#define GPIO_BASE                (BCM2711_PERI_BASE + 0x200000) /* GPIO controller */

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock


// page and block size for mmap
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// I/O dev/mem access variables and functions
extern volatile unsigned *gpio;
void setup_io();
int readGPIO(int g);


// Queue structures and functions
struct mynode{
	int bitval;
	struct mynode *next;
};

struct myquecon{
  struct mynode *head, *tail;
};

struct myquecon *CreateQueCon();
struct mynode *CreateNode(int val);
void QuePush(struct myquecon *mq, int val);
int QuePop(struct myquecon *mq);




