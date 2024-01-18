#include <LocoNet.h>

// LocoNet PC Interface Demo

// Flow Control to prevent the PC sending more data than the LocoNet can handle
// is provided via the RS232 Clear-To-Send (CTS) pin. When the Serial receive
// buffer is less than RX_BUF_LOW then the 

// ZIM 2021-10-11 modified for speed selection, Tx-Pin changed

#define  IN_SPEED_SELECTION  3	// low = 19200Bd, High = 57600Bd (default)

#define  RX_CTS_PIN   9
#define  RX_BUF_LOW   32 
#define  RX_BUF_HIGH  96

#define  TX_PIN   7

static   LnBuf        LnTxBuffer ;
static   lnMsg        *LnPacket;

void setup()
{
	pinMode(IN_SPEED_SELECTION, INPUT_PULLUP);
	boolean bSelection57600(digitalRead(IN_SPEED_SELECTION) == HIGH);

    // Configure the serial port for 19200/57600 baud
  Serial.begin(bSelection57600? 57600 : 19200);
  
    // First initialize the LocoNet interface, specifying the TX Pin
  LocoNet.init(TX_PIN);
  
    // Initialize a LocoNet packet buffer to buffer bytes from the PC 
  initLnBuf(&LnTxBuffer) ;
  
    // Configure the CTS pin for hardware flow control to control 
    // the serial data flow from the PC to the LocoNet 
  pinMode(RX_CTS_PIN,OUTPUT);
  digitalWrite(RX_CTS_PIN,LOW);
}

void loop()
{  
    // Check for any received LocoNet packets
  LnPacket = LocoNet.receive() ;
  if( LnPacket )
  {
      // Get the length of the received packet
    uint8_t Length = getLnMsgSize( LnPacket ) ;

      // Send the received packet out byte by byte to the PC
    for( uint8_t Index = 0; Index < Length; Index++ )
#if defined(ARDUINO) && ARDUINO >= 100
      Serial.write(LnPacket->data[ Index ]);
#else 
      Serial.print(LnPacket->data[ Index ], BYTE);
#endif
  }

    // Check to see if there are any bytes from the PC
  if(int charWaiting = Serial.available())
  {
      // If the number of bytes waiting is less than RX_BUF_LOW enable CTS
    if( charWaiting < RX_BUF_LOW )
      digitalWrite(RX_CTS_PIN,LOW);

      // If the number of bytes waiting is more than RX_BUF_HIGH disable CTS
    else if( charWaiting > RX_BUF_HIGH )
      digitalWrite(RX_CTS_PIN,HIGH);

      // Read the byte
    uint8_t inByte = Serial.read() & 0xFF;
    
      // Add it to the buffer
    addByteLnBuf( &LnTxBuffer, inByte ) ;
    
      // Check to see if we have received a complete packet yet
    LnPacket = recvLnMsg( &LnTxBuffer ) ;
    if(LnPacket )
        // Send the received packet from the PC to the LocoNet
      LocoNet.send( LnPacket ) ;
  }
  else
      // Enable CTS anyway
    digitalWrite(RX_CTS_PIN,LOW);
}
