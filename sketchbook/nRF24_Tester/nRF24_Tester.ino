#include <SPI.h>
#define CE_pin 4
#define CSN_pin 5
#define IRQ_pin 2
#define MOSI_pin 51
#define MISO_pin 50
#define SCK_pin 52  

//The global variables used by everyone
byte data_in[8], data2, data3;
int pin_state;

void setup(){//     setup start     setup start     setup start     setup start     setup start
  
  Serial.begin(115200);//start Serial
  Serial.println("Setting Up");
  delay(100);
  NRF_Init();//set up the SPI and define pins
  NRF_set_RX_payload(0, 8);//pipe 0-5, bytes 1-32
  NRF_get_address(7, 1);//Get Reg7 Status, 1=print screen
  NRFwrite_bit_write(0, 0, 1);//register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
  NRFwrite_bit_write(0, 1, 1);//register, bit, and value 0,1,1 PowerUP
  NRFwrite_bit_write(0, 4, 1);//RT Mask turns off the RT interrupt
  NRFwrite_bit_write(0, 5, 1);//TX Mask turns off the TX interrupt
  //250kbps comm
  NRFwrite_bit_write(6, 5, 1);//TX Mask turns off the TX interrupt
  NRFwrite_bit_write(6, 3, 0);//TX Mask turns off the TX interrupt
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX
  digitalWrite(CSN_pin, HIGH);
  
  NRF_ClearInterrupts();//clears any interrupts
  delay(100);
  attachInterrupt(0, get_data, FALLING);//kick things off by attachin the IRQ interrupt
}//      end setup      end setup      end setup      end setup      end setup      end setup

void loop()
{//     loop start     loop start     loop start     loop start     loop start
  
}//   end loop   end loop   end loop   end loop   end loop   end loop   end loop   end loop

void transmit(byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h){//     transmit start     transmit start     transmit start
  //mode pin and value don't mena anything yet, but they will be
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX, get rid of anything that might be in there
  digitalWrite(CSN_pin, HIGH);  
  
  delay(1);
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B10100000);//load TX payload
  data_in[0] = SPI.transfer(a);//action digital Read
  data_in[1] = SPI.transfer(b);//pin number
  data_in[2] = SPI.transfer(c);
  data_in[3] = SPI.transfer(d);//pin number
  data_in[4] = SPI.transfer(e);
  data_in[5] = SPI.transfer(f);
  data_in[6] = SPI.transfer(g);//pin number
  data_in[7] = SPI.transfer(h);
  digitalWrite(CSN_pin, HIGH);
   
  digitalWrite(CE_pin, LOW);//pull CE pin LOW
  delay(1);//small delay
  NRFwrite_bit_write(0, 0, 0);//go into TX mode
  delay(1);//small delay
  digitalWrite(CE_pin, HIGH);
  delay(1);//this is the time CE pin must be HIGH for before going back into RX mode
  //delay(1) seems to work best for this.  any longer or shorter doesn't work as well
  NRFwrite_bit_write(0, 0, 1);//go back into RX mode

}//    end  transmit    end  transmit    end  transmit    end  transmit    end  transmit

void get_data(){// get data start get data start get data start get data start get data start
  // this routine is called when the IRQ pin is pulled LOW by the NRF
  
  int i;
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B01100001);//read the payload
  data_in[0] = SPI.transfer(B00000000);
  data_in[1] = SPI.transfer(B00000000);
  data_in[2] = SPI.transfer(B00000000);
  data_in[3] = SPI.transfer(B00000000);
  data_in[4] = SPI.transfer(B00000000);
  data_in[5] = SPI.transfer(B00000000);
  data_in[6] = SPI.transfer(B00000000);
  data_in[7] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);

  for(i=7; i>=0; i--)
  Serial.print(char(data_in[i]));//just print out whatever was recieved
  Serial.println("  ");
  
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);


NRFwrite_bit_write(7,6,1);//clear the RX interrupt flag

}//     END   get  data     END   get  data     END   get  data     END   get  data     END   get  data

void NRF_Init(){//  start NRF init  start NRF init  start NRF init  start NRF init  start NRF init
  
  pinMode(CE_pin, OUTPUT);//chip enable set as output
  pinMode(CSN_pin, OUTPUT);//chip select pin as output
  pinMode(MOSI_pin, OUTPUT);//SPI data out
  pinMode(MISO_pin, INPUT); //SPI data in
  pinMode(SCK_pin, OUTPUT);//SPI clock out
  Serial.println("NRF Pins Initialized");
  SPI.setBitOrder(MSBFIRST);//SPI Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 - 8MHz
  digitalWrite(CE_pin, HIGH);//RX mode
  digitalWrite(CSN_pin, HIGH);//SPI idle
  SPI.begin();//start up the SPI library
  Serial.println("NRF Ready");
}//    END NRF_init    END NRF_init    END NRF_init    END NRF_init    END NRF_init    END NRF_init

void NRF_set_RX_payload(byte pipe, byte bytes){//start NRF payload     start NRF payload
  byte address=pipe+32+16+1;// a register write starts at 32, so add on the 1 and 16 to get you to at R17
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);//write register 11 RX_PW_P0
  data_in[1] = SPI.transfer(bytes);//3 bytes for now
  digitalWrite(CSN_pin, HIGH);
  Serial.print("RX Payload Set RX_PW_P");
  Serial.print(pipe);
  Serial.print(" for ");
  Serial.print(bytes);
  Serial.println(" bytes");
}//    END payload     END NRF payload    END payload     END NRF payload

void NRF_ClearInterrupts(){//    start clear interrupts      start clear interrupts      start clear interrupts  
 //there are three interrupt flags in the NRF.  Thsi routine checks them, and if set, it will clear them
 
  NRF_get_address(7, 0);//RT interrupt
  if(bitRead(data_in[1], 4))
  NRFwrite_bit_write(7,4,1);
  
  NRF_get_address(7, 0);//TX interrupt
  if(bitRead(data_in[1], 5))
  NRFwrite_bit_write(7,5,1);
  
  NRF_get_address(7, 0);//RX interrupt
  if(bitRead(data_in[1], 6))
  NRFwrite_bit_write(7,6,1);

}//   END clear interrupts   END clear interrupts   END clear interrupts   END clear interrupts

//     MASSIVE ROUTINE HERE - PUT ALL ROUTINES ABOVE THIS

void NRF_get_address(byte address, byte info){// START Get Address   START Get Address   START Get Address  
  //send the address and either a 1 or 0 if you want to do a serial print of the address
  //after a call to this routine, data_in[1] will equal the address you called
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);
  data_in[1] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);
  if(info==1){// if the user wanted it, you will get a print out of the register - good fo debugging
  Serial.print("R");
  Serial.print(address);
  switch (address) {
  case 0:
   Serial.print(" CONFIG REGISTER =");
   Serial.println(data_in[1]);
   Serial.print("PRIM_RX = ");
   if(bitRead(data_in[1],0))
   Serial.println("PRX");
   else
   Serial.println("PTX");
   
   Serial.print("PWR_UP = ");
   if(bitRead(data_in[1],1))
   Serial.println("POWER UP");
   else
   Serial.println("POWER DOWN");  
   
    Serial.print("CRCO = ");
   if(bitRead(data_in[1],2))
   Serial.println("2Bytes");
   else
   Serial.println("1Byte");   
   
   Serial.print("EN_CRC = ");
   if(bitRead(data_in[1],3))
   Serial.println("Enabled");
   else
   Serial.println("Disabled");  
 
    Serial.print("MASK_MAX_RT = ");
   if(bitRead(data_in[1],4))
   Serial.println("Interrupt not reflected on the IRQ pin");
   else
   Serial.println("Reflect MAX_RT as active low interrupt on the IRQ pin");  
 
   Serial.print("MASK_TX_DS = ");
   if(bitRead(data_in[1],5))
   Serial.println("Interrupt not reflected on the IRQ pin");
   else
   Serial.println("Reflect TX_DS as active low interrupt on the IRQ pin");  
   
   Serial.print("MASK_RX_DR = ");
   if(bitRead(data_in[1],6))
   Serial.println("Interrupt not reflected on the IRQ pin");
   else
   Serial.println("Reflect RX_DR as active low interrupt on the IRQ pin");  
   break;//0
case 1:
   Serial.print(" EN_AA REGISTER Enhanced ShockBurst =");
   Serial.println(data_in[1]);
break;//1
case 2:
   Serial.print(" EN_RXADDR REGISTER Enabled RX Addresses =");
   Serial.println(data_in[1]);
break;//2
case 3:
   Serial.print(" SETUP_AW REGISTER Setup of Address Widths =");
   Serial.println(data_in[1]);
break;//3
case 4:
   Serial.print(" SETUP_RETR REGISTER Setup of Automatic Retransmission =");
   Serial.println(data_in[1]);
break;//4
case 5:
   Serial.print(" RF_CH REGISTER RF Channel =");
   Serial.println(data_in[1]);
break;//5
case 6:
   Serial.print(" RF_SETUP REGISTER RF Setup Register =");
   Serial.println(data_in[1]);
   Serial.print("RF Power = ");
   Serial.print(bitRead(data_in[1],2));
   Serial.println(bitRead(data_in[1],1));
   Serial.print("RF_DR_HIGH = ");
   Serial.println(bitRead(data_in[1],3));
   Serial.print("PLL_LOCK = ");
   Serial.println(bitRead(data_in[1],4));
   Serial.print("RF_DR_LOW = ");
   Serial.println(bitRead(data_in[1],5));  
   Serial.print("CONT_WAVE = ");
   Serial.println(bitRead(data_in[1],7));   
break;//6
case 7:
   Serial.print(" STATUS REGISTER  =");
   Serial.println(data_in[1]);
   Serial.print("TX_FULL = ");
   if(bitRead(data_in[1],0))
   Serial.println("TX FIFO full");
   else
   Serial.println("TX FIFO Not full");
   
   Serial.print("RX_P_NO = ");
   if(bitRead(data_in[1],1)&&(data_in[1],2)&&(data_in[1],3))
   Serial.println("RX FIFO Empty");
   else
   Serial.println(bitRead(data_in[1],1)+(bitRead(data_in[1],2)<<1)+(bitRead(data_in[1],2)<<2)); 
   Serial.print("MAX_RT Interrupt = ");
   Serial.println(bitRead(data_in[1],4));
   Serial.print("TX_DS Interrupt = ");
   Serial.println(bitRead(data_in[1],5));
   Serial.print("RX_DR Interrupt = ");
   Serial.println(bitRead(data_in[1],6));
break;//7
case 8:
   Serial.print(" OBSERVE_TX REGISTER Transmit observe register  =");
   Serial.println(data_in[1]);
   Serial.print("ARC_CNT = "); 
   Serial.println(bitRead(data_in[1],0)+(bitRead(data_in[1],1)<<1)+(bitRead(data_in[1],2)<<2)+(bitRead(data_in[1],3)<<3));
   Serial.print("PLOS_CNT = "); 
   Serial.println(bitRead(data_in[1],4)+(bitRead(data_in[1],5)<<1)+(bitRead(data_in[1],6)<<2)+(bitRead(data_in[1],7)<<3));
break;//8
case 9:
   Serial.print(" RPD REGISTER Received Power Detector =");
   Serial.println(bitRead(data_in[1],0));
break;//9
case 10:
   Serial.print(" RX_ADDR_P0 LSB =");
   Serial.println(data_in[1]);
break;//10
case 11:
   Serial.print(" RX_ADDR_P1 LSB =");
   Serial.println(data_in[1]);
break;//11
case 12:
   Serial.print(" RX_ADDR_P2 LSB =");
   Serial.println(data_in[1]);
break;//12
case 13:
   Serial.print(" RX_ADDR_P3 LSB =");
   Serial.println(data_in[1]);
break;//13
case 14:
   Serial.print(" RX_ADDR_P4 LSB =");
   Serial.println(data_in[1]);
break;//14
case 15:
   Serial.print(" RX_ADDR_P5 LSB =");
   Serial.println(data_in[1]);
break;//15
case 16:
   Serial.print(" TX_ADDR LSB =");
   Serial.println(data_in[1]);
break;//16
case 17:
   Serial.print(" RX_PW_P0 RX payload =");
   Serial.println(data_in[1]);
break;//17
case 18:
   Serial.print(" RX_PW_P1 RX payload =");
   Serial.println(data_in[1]);
break;//18
case 19:
   Serial.print(" RX_PW_P2 RX payload =");
   Serial.println(data_in[1]);
break;//19
case 20:
   Serial.print(" RX_PW_P3 RX payload =");
   Serial.println(data_in[1]);
break;//20
case 21:
   Serial.print(" RX_PW_P4 RX payload =");
   Serial.println(data_in[1]);
break;//21
case 22:
   Serial.print(" RX_PW_P5 RX payload =");
   Serial.println(data_in[1]);
break;//22

case 23:
   Serial.print(" FIFO_STATUS Register =");
   Serial.println(data_in[1]);
   Serial.print("RX_EMPTY = ");
   if(bitRead(data_in[1],0))
   Serial.println("RX FIFO empty");
   else
   Serial.println("Data in RX FIFO");
   
   Serial.print("RX_EMPTY = ");
   if(bitRead(data_in[1],1))
   Serial.println("RX FIFO full");
   else
   Serial.println("Available locations in RX FIFO");
 
    Serial.print("TX_EMPTY = ");
   if(bitRead(data_in[1],4))
   Serial.println("TX FIFO empty");
   else
   Serial.println("Data in TX FIFO");  
  
      Serial.print("TX_FULL = ");
   if(bitRead(data_in[1],5))
   Serial.println("TX FIFO full");
   else
   Serial.println("Available locations in TX FIFO"); 
   Serial.print("TX_REUSE = ");
   Serial.println(bitRead(data_in[1],6));
break;//23
  }//switch
  }//if 1
}// END get_address END get_address END get_address END get_address END get_address END get_address END get_address
  void NRFwrite_bit_write(byte address, byte bit_add, byte val){//   start bit write   start bit write   start bit write
  //This routine writes single bits of a register, without affecting the rest of the register
  
  NRF_get_address(address, 0);//first read out the register
  if(val==1)//if we want to write a one to the bit then set the bit in the register we read
  bitSet(data_in[1],bit_add);
  else
  bitClear(data_in[1],bit_add);//clear it if not
  
  digitalWrite(CSN_pin, LOW);//now we'll write the modified data back in
  data_in[0] = SPI.transfer(32+address);//a write to a register adds 32
  data_in[1] = SPI.transfer(data_in[1]);//write the modified register
  digitalWrite(CSN_pin, HIGH);
}//    END bit write    END bit write    END bit write    END bit write    END bit write    END bit write
  

  
  

