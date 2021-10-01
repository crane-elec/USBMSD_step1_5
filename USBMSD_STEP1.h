#ifndef USBMSD_STEP1_H
#define USBMSD_STEP1_H


#include "mbed.h"
#define USBMSD_STEP1_SECTORS 1004*2
#define USBMSD_STEP1_SECTOR_SIZE 512
#define USBMSD_STEP1_DISK_SIZE    USBMSD_STEP1_SECTOR_SIZE*USBMSD_STEP1_SECTORS
#include <stdlib.h>
#include "USBMSD.h"
#include "RawNAND.h"
#include "bbt_eeprom.h"

class USBMSD_STEP1: public USBMSD
{   public:
        USBMSD_STEP1(PinName ceb, PinName cle,
                     PinName ale, PinName web,
                     PinName reb, PinName wpb,
                     PinName rbb, 
                     PinName io1, PinName io2,
                     PinName io3, PinName io4,
                     PinName io5, PinName io6,
                     PinName io7, PinName io8,
                     PinName ledRd,PinName ledWr,PinName swInit);

        virtual int disk_read(uint8_t* data, uint64_t block, uint8_t count);
        virtual int disk_write(const uint8_t* data, uint64_t block, uint8_t count);
        virtual int disk_initialize();
        virtual uint64_t disk_sectors();
        virtual uint64_t disk_size();
        virtual int disk_status();

        DigitalOut * m_ledRead;
        DigitalOut * m_ledWrite;
        DigitalIn  * m_swInit;
//        DigitalIn  * m_isp;

    private:
        RawNAND    * m_rawNand;
        bbt_eeprom * m_bbt;
        uint8_t      m_buffer[512];
};

#endif
