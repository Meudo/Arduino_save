#include <Manchester.h>

/*


Manchester Receiver example


  In this example receiver will receive one 16 bit number per transmittion

Try different speeds using this constants, your maximum possible speed will 

  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/

#define RX_PIN 7
#define BUFFER_SIZE 7
uint8_t Message[BUFFER_SIZE];
int No_Data=0, Module=0, Signe=0, CheckSum=0;
float Temperature=0.0;

void setup() {
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_1200);
  //man.beginReceive();
  man.beginReceiveArray(BUFFER_SIZE, Message);
}

void loop() {
  if (man.receiveComplete()) {
    
/*
    Serial.write("<Debut>");
    //do something with the data in 'buffer' here before you start receiving to the same buffer again
    //Message[BUFFER_SIZE] = man.getMessage();
    
    for(uint8_t i=1; i<BUFFER_SIZE; i++)
      Serial.write(Message[i]);
      man.beginReceiveArray(BUFFER_SIZE, Message);
      Serial.println("<Fin>");
*/
/**/    uint16_t m = man.getMessage();

    switch (m)
    {
      case 0xFE : No_Data=0; break;
      case 0xFF : No_Data=6; break;
      default:               break;
    }

    switch (No_Data)
    {
      case 0 : Serial.print("<Debut>");                                       No_Data++; break;
      case 1 : Module=m;                                                      No_Data++; break;
      case 2 : Signe=(m==1 ? 1 : -1);                                         No_Data++; break;
      case 3 : Temperature = m; CheckSum = m;                                 No_Data++; break;
      case 4 : Temperature = Signe * (Temperature + ((float)m/100)); CheckSum += m;  No_Data++; break;
      case 5 : if (m==CheckSum) 
               {
                  Serial.print("Module "); Serial.print(Module); Serial.print(" : ");
                  Serial.print(Temperature);
               }
               else Serial.print("<Mesure Ko>");        
               No_Data=6; break;
      case 6 : if(m==0xFF) {Serial.println("<Fin>"); No_Data++;}
                else Serial.print("<Pas Fin>");
                No_Data++; break;
      default : Serial.print("<No trame hors limite : ");  Serial.print(No_Data); Serial.println(">"); No_Data++;
    }
    
   // Serial.print(m);Serial.print(" ");
    man.beginReceive(); //start listening for next message right after you retrieve the message
/**/
    
  }
}


