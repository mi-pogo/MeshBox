
#include "mesh_functions.h"

// Queue functions

// function to create an empty list
struct myquecon *CreateQueCon()
{
	struct myquecon *qtemp;
	
	qtemp = (struct myquecon *) malloc (sizeof(struct myquecon));
	
	qtemp->head = qtemp->tail = NULL;
	
	return qtemp;
}

// function to create a node
struct mynode *CreateNode(int val)
{
	struct mynode *qtemp;
	
	qtemp = (struct mynode *) malloc (sizeof (struct mynode));
	qtemp->bitval = val;
	qtemp->next = NULL;
	
	return (qtemp);
}

// pushes one element into the queue
void QuePush(struct myquecon *mq, int val)
{
	struct mynode *qtemp;

	qtemp = CreateNode(val);

	if (mq->tail == NULL){
	  mq->head = mq->tail = qtemp;
	  return;
    }

    mq->tail->next = qtemp;
    mq->tail = qtemp;
}

// takes out one element from the queue
int QuePop(struct myquecon *mq)
{
	struct mynode *qtemp;
	int tempval;
	
	if (mq->head == NULL)
	  return (-1);
	  
	qtemp =  mq->head;
	mq->head = mq->head->next;
	tempval = qtemp->bitval;
	
	if (mq->head == NULL)
	  mq->tail = NULL;
	
	free (qtemp);
	
    return (tempval);
}


// I/O dev/mem access variables and functions
volatile unsigned *gpio;

// Set up a memory region to access GPIO
void setup_io()
{
   int  mem_fd;
   void *gpio_map;

   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map); //errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io

// Read GPIO input
int readGPIO(int g)
{
  if (GET_GPIO(g)) // !=0 <-> bit is 1 <- port is HIGH=3.3V
    return 1;
  else // port is LOW=0V
    return 0;
}
