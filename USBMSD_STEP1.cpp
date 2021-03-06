#include "USBMSD_STEP1.h"
#include "RawNAND.h"

USBMSD_STEP1::USBMSD_STEP1(PinName ceb, PinName cle,
                 PinName ale, PinName web,
                 PinName reb, PinName wpb,
                 PinName rbb, 
                 PinName io1, PinName io2,
                 PinName io3, PinName io4,
                 PinName io5, PinName io6,
                 PinName io7, PinName io8,
                 PinName ledRd,PinName ledWr,PinName swInit) 
                 {
                m_rawNand = new RawNAND(ceb,cle,ale,web,reb,wpb,rbb,io1,io2,io3,io4,io5,io6,io7,io8);
                m_bbt     = new bbt_eeprom(1004);
                
                
                m_ledRead  = new DigitalOut(ledRd,1);
                m_ledWrite = new DigitalOut(ledWr,1);
                m_swInit   = new DigitalIn(swInit);
                
                // if SW is pushed, build BBT table
                if (*m_swInit==0) {
                    uint8_t pagebuff;

                    *m_ledRead=0;
                    *m_ledWrite=0;

                    // search bad blocks in replace area. block 1004-1023
                    for (int r=0;r<20;r++) {
                        m_rawNand->pageRead(&pagebuff,(r+1004),63,2175,1);
                        if( pagebuff!=0xff) {
                            m_bbt->setSlot_EEP(r,1,0,0);
                        }
                        else {
                            // remove broken flag and user flag
                            m_bbt->setSlot_EEP(r,0,0,0);
                        }
                    }
                    
                    // search bad blocks
                    for (int b=0;b<1024-20;b++) {
                        m_rawNand->pageRead(&pagebuff,b,63,2175,1);
                        if (pagebuff!=0xff){
                            m_bbt->setSlot_EEP(m_bbt->getFreeSlot_EEP(), 0, 1,b);
                        }
                    }

                    // set temp
                    m_bbt->put(1,0x00);
                    *m_ledRead=1;
                    *m_ledWrite=1;

                }
                m_bbt->updateTranslateTable();
                
}


int USBMSD_STEP1::disk_read(uint8_t* data, uint64_t block, uint8_t count)
{
    *m_ledRead = 0; 
    m_rawNand->pageRead(data,m_bbt->getTranslatedBlockAddress(block>>1),62+(block&0x1),0,512);
    //m_rawNand->pageRead(data,m_bbt->getTranslatedBlockAddress(block),63,0,512);
    *m_ledRead = 1;
    return 0;
}
int USBMSD_STEP1::disk_write(const uint8_t* data, uint64_t block, uint8_t count)
{
    *m_ledWrite = 0;
    // copy data
    for (uint16_t c=0;c<2;c++){
        if ( (block&0x1) == c) continue;
        m_rawNand->pageRead(m_buffer,m_bbt->getTranslatedBlockAddress(block>>1),62+c,0,512);
    }
    // write data
    m_rawNand->erase(m_bbt->getTranslatedBlockAddress(block>>1));
    for (uint16_t c=0;c<2;c++){
        if ( (block&0x1) == c) {
            m_rawNand->pageProgram(data,m_bbt->getTranslatedBlockAddress(block>>1),62+c,0,512);
        } else {
            m_rawNand->pageProgram(m_buffer,m_bbt->getTranslatedBlockAddress(block>>1),62+c,0,512);
        }
    }

    //m_rawNand->erase(m_bbt->getTranslatedBlockAddress(block));
    //m_rawNand->pageProgram(data,m_bbt->getTranslatedBlockAddress(block),63,0,512);

    *m_ledWrite = 1; 
    return 0;
}
int USBMSD_STEP1::disk_status()
{
    return 0;
}
int USBMSD_STEP1::disk_initialize()
{
    
    return 0;
}
uint64_t USBMSD_STEP1::disk_sectors()
{
    return USBMSD_STEP1_SECTORS;
}
uint64_t USBMSD_STEP1::disk_size()
{
    return USBMSD_STEP1_DISK_SIZE;
}
