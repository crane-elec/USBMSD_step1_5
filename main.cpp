#include "mbed.h"

#include "USBMSD_STEP1.h"

#define ND_CEB P0_9
#define ND_CLE P0_8
#define ND_ALE P1_29
#define ND_WEB P0_12
#define ND_REB P1_21
#define ND_WPB P0_11
#define ND_RBB P1_31
#define ND_IO1 P0_18
#define ND_IO2 P0_19
#define ND_IO3 P1_25
#define ND_IO4 P0_20
#define ND_IO5 P0_2
#define ND_IO6 P1_26
#define ND_IO7 P1_27
#define ND_IO8 P1_23

#define LED_RD  P0_7
#define LED_WR  P1_13
#define SW_INIT P1_20


int main() {
    USBMSD_STEP1 *myusb;

    myusb = new USBMSD_STEP1(ND_CEB,ND_CLE,ND_ALE,ND_WEB,ND_REB,ND_WPB,ND_RBB,
                  ND_IO1,ND_IO2,ND_IO3,ND_IO4,ND_IO5,ND_IO6,ND_IO7,ND_IO8,
                  LED_RD,LED_WR,SW_INIT);
    myusb->connect();
    while(1) {
        __NOP();
    }
}
