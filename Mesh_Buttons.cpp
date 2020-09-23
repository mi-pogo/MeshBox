//============================================================================
// Name        : Mesh_Buttons.cpp
// Authors     : Mihail Pogorevici, Gabriela Martin
// Version     : 1.0
// Copyright   : Open Source
// Description : Service for rotary encoders and push buttons for MeshBox
//============================================================================

#include "mesh_functions.h"
#include "/usr/include/kodi/xbmcclient.h"
#include <iostream>
using namespace std;

// Kodi Event Server library instance
CXBMCClient eventClient;

// thread and mutex variables
pthread_t thread_but1, thread_but3, thread_but4, thread_but5, thread_enc0, thread_enc2;
pthread_mutex_t mutex_rot;

// queue variable. it stores the events coming from threads
struct myquecon *queEvent;

// thread functions for rotary encoders and buttons
void *button_handler (void *butparam);
void *encoder_handler (void *encparam);


int main() {

    int exState, thread_param, tempval;

    // Set up gpio pointer for direct register access
    setup_io();
    // Switch GPIOs to input mode
    INP_GPIO(backButton);
    INP_GPIO(EncNavOutA);
    INP_GPIO(EncNavOutB);

    // Initialize queue
    queEvent = CreateQueCon();

    // Initialize mutex
    pthread_mutex_init(&mutex_rot, NULL);

    // Initialize encoder threads
    int thderr;
    thread_param = 0;
    thderr = pthread_create(&thread_enc0, NULL, encoder_handler, (void *)thread_param);
    if(thderr != 0) {
      fprintf(stderr, "Failed to create Encoder thread 0\n");
    }
    thread_param = 2;
    thderr = pthread_create(&thread_enc2, NULL, encoder_handler, (void *)thread_param);
    if(thderr != 0) {
      fprintf(stderr, "Failed to create Encoder thread 2\n");
    }
    // Initialize button threads
    thread_param = 1;
    thderr = pthread_create(&thread_but1, NULL, button_handler, (void *)thread_param);
    if(thderr != 0) {
      fprintf(stderr, "Failed to create Button thread 1\n");
    }
    thread_param = 3;
    thderr = pthread_create(&thread_but3, NULL, button_handler, (void *)thread_param);
    if(thderr != 0) {
      fprintf(stderr, "Failed to create Button thread 3\n");
    }
    thread_param = 5;
    thderr = pthread_create(&thread_but5, NULL, button_handler, (void *)thread_param);
    if(thderr != 0) {
      fprintf(stderr, "Failed to create Button thread 5\n");
    }


    while (1){

      pthread_mutex_lock (&mutex_rot); // Enter critical section
      tempval = QuePop(queEvent);  // read event from queue
      pthread_mutex_unlock (&mutex_rot); // Exit critical section

      // test value read from queue and execute action to Kodi according to its value
      switch (tempval) {
        case 0: // rotated left (EncVol)
          if(!readGPIO(powerButton)) // test "power button" position to know which function to execute for the first rotary encoder (EncVol)
              eventClient.SendACTION("Left", ACTION_BUTTON);
          else
              eventClient.SendACTION("VolumeDown", ACTION_BUTTON);
          break;
        case 1: // rotated right (EncVol)
          if(!readGPIO(powerButton)) // test "power button" position to know which function to execute for the first rotary encoder (EncVol)
              eventClient.SendACTION("Right", ACTION_BUTTON);
          else
              eventClient.SendACTION("VolumeUp", ACTION_BUTTON);
          break;
        case 10: // click was pressed (EncVol)
          if(!readGPIO(powerButton)) // test "power button" position to know which function to execute for the first rotary encoder (EncVol)
              eventClient.SendACTION("Select", ACTION_BUTTON);
          else
              eventClient.SendACTION("Mute", ACTION_BUTTON);
          break;
        case 20: // rotated left (EncNav)
          eventClient.SendACTION("Up", ACTION_BUTTON);
          break;
        case 21: // rotated right (EncNav)
          eventClient.SendACTION("Down", ACTION_BUTTON);
          break;
        case 30: // click was pressed (EncNav)
          eventClient.SendACTION("Select", ACTION_BUTTON);
          break;
        case 50: // // "back" was pressed
          eventClient.SendACTION("Back", ACTION_BUTTON);
          break;
        default:
          break;
      }

      usleep (10 * 1000); // wait a bit...

    }

	return (0);
}

// thread function for rotary encoders
void *encoder_handler (void *encparam)
{
    int th_param, outA, outB, bitval, counter = 0;
    uint16_t state=0;

    // read parameter
    th_param = (int)(void *)encparam;

    // determine button id based on input parameter
    if(th_param==2) {
        outA = EncNavOutA;
        outB = EncNavOutB;
    }
    if(th_param==0) {
        outA = EncVolOutA;
        outB = EncVolOutB;
    }


    while(1) {

      usleep(100); // Simulate doing somehing else as well.

      state=(state<<1) | readGPIO(outA) | 0xe000; // read encoder outA current value and debounce...

      if (state==0xf000){

         state=0x0000;

         if(readGPIO(outB))  // test encoder outB current value
             bitval = th_param*10 + 1; // if rotated to left
         else
             bitval = th_param*10;  // if rotated to right

         // cout << th_param << " - " << bitval << endl;
         pthread_mutex_lock (&mutex_rot); // Enter critical section
         QuePush(queEvent, bitval); // send event to the queue
         pthread_mutex_unlock (&mutex_rot); // Exit critical section
      }

    }

}

// thread function for buttons
void *button_handler (void *butparam)
{
    int th_param, but_param, bitval, bittemp;
    uint16_t state=0;

    // read parameter
    th_param = (int)(void *)butparam;

    // determine button id based on input parameter
    if(th_param==1) but_param = EncVolClick;
    if(th_param==3) but_param = EncNavClick;
    if(th_param==4) but_param = powerButton;
    if(th_param==5) but_param = backButton;


    while(1) {

      usleep(100); // Simulate doing somehing else as well.

      bittemp = readGPIO(but_param); // read button current status
      state=(state<<1) | bittemp | 0xe000;  // debounce...

      if (state==0xf000){

         state=0x0000;

         if(!bittemp){ // if button was pressed (input is 0)
             bitval = th_param*10;
             pthread_mutex_lock (&mutex_rot); // Enter critical section
             QuePush(queEvent, bitval);  // send event to the queue
             pthread_mutex_unlock (&mutex_rot); // Exit critical section
         }

      }

    }

}
