#include <SimpleModbusMaster.h>
/* serial stuff */
#include <SoftwareSerial.h>
#define baud 9600

#define TxEnablePin 2
#define timeout 1000
#define polling 200 // the scan rate
#define retry_count 10
#define LED 13  // our led pin
#define slave 1 // id of slave to send data to
#define master_but 6
#define remote_but 5

SoftwareSerial rs(3,4);
/********************************************************************************
 * we have local buttons and LED, and remote(s) buttons and led(s)
 * We keep state of changes from each slave
 *
 *
 */
int dev_buttons=[master_but,remote_but]; // ID's of buttons in each unit (as they're the same)

// our packet data
enum {
  PACKET1,              // Read data from Slave
  PACKET2,              // Send data to slave
  TOTAL_NO_OF_PACKETS   // leave this last entry
};

Packet packets[TOTAL_NO_OF_PACKETS];
packetPointer read_pkt = &packets[PACKET1];
packetPointer write_pkt = &packets[PACKET2];
unsigned int readRegs[1];
unsigned int writeRegs[1];


void setup() {
    /* modbus setup */
    modbus_construct(read_pkt, slave, READ_HOLDING_REGISTERS, 0, 1, readRegs);
    modbus_construct(write_pkt, slave, PRESET_MULTIPLE_REGISTERS, 1, 1, writeRegs);
    modbus_configure(&Serial, baud, SERIAL_8N2, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS);
    /* our pins */
    for(i==0;i<len(dev_buttons);i++){
        pinMode(dev_buttons[i],INPUT_PULLUP);
    }
    pinMode(LED, OUTPUT);
    /* serial */
    rs.begin(9600);
    rs.println("Master Starting ...");
}
void loop() {
    bool loc_m, rem_m, loc_s;
    /* 
     * procedure is...
     * test for local master button
     * test for remote master button
     * test for local slave button
     *
     * if local or remote master: toggle master LED
     * if local slave, set slave switch on 
     * if not local slave, set slave switch off
     *
     * send status of local led & local slave
     */
    modbus_update();
    loc_m=digitalRead(dev_buttons[master_but]); // grab our master button
    rem_m=readRegs[0];                          // grab our remote master button
    if (loc_m | rem_m){
        digitalWrite(LED,true);
    } else {
        digitalWrite(LED,false);
    }
    // setup our status to slave
    writeRegs[0] = digitalRead(dev_buttons[remote_but]); // update data to be written to arduino slave
}
