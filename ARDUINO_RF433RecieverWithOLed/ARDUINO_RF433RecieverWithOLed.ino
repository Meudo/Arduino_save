/********* Ecran *******************/
/*#include <SPI.h>            
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>*/
/********* Ecran *******************/

/********** Reception ****************/
#include <VirtualWire.h>
/********** Reception ****************/

/********* Ecran *******************/
/*#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif*/
/********* Ecran *******************/

/********** Reception ****************/
#define NO_MODULE 13
#define RX_PIN 11
typedef struct {
  char NoModule=NO_MODULE;
  float Temperature;
} MaStructure;
/********** Reception ****************/

void setup()   {                
  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  /***************** Init Ecran (Debut)**********************************************/
/*  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
//  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,0);
  display.println("Chambre William");
  display.setTextColor(WHITE, BLACK);
  display.println("---------------------");
  display.setCursor(0,16);
  display.setTextSize(2);
  display.print("Temperature");
//  display.startscrollleft(0x00, 0x0F);
//  display.startscrollright(0x00, 0x0F);
//  display.startscrolldiagright(0x00, 0x07);
//  display.startscrolldiagleft(0x00, 0x07);
//  display.stopscroll();
//  display.stopscroll();
  display.display();*/
/***************** Init Ecran (Fin) **********************************************/
  //delay(2000);

/***************** Init Reception (Debut) **********************************************/
  vw_set_rx_pin(RX_PIN);
  vw_setup(2000);
  vw_rx_start(); // On peut maintenant recevoir des messages
/***************** Init Reception (Fin) **********************************************/

/***************** Clignottement LED (Debut) **********************************************/
  for (int i=0; i<10; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    if (i==4) {delay(100);}
  }
/***************** Clignottement LED (Fin) **********************************************/

}


void loop()
{
  //MaStructure message;
  //byte taille_message = sizeof(MaStructure);
  int cpt;
  byte taille_message = sizeof(cpt);

  /* 
   La variable "taille_message" doit impérativement être remise à 
   la taille de la structure avant de pouvoir recevoir un message. 
   Le plus simple est d'utiliser une variable locale pour ne pas
   avoir à réassigner la valeur à chaque début de loop().
   */

  // On attend de recevoir un message
  vw_wait_rx();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);

  if (vw_get_message((byte *) &cpt, &taille_message)) {
    // On copie le message, qu'il soit corrompu ou non

    Serial.print("cpt="); // Affiche le message
    Serial.print(cpt);

/*  if (vw_get_message((byte *) &message, &taille_message)) {
    // On copie le message, qu'il soit corrompu ou non

    Serial.print("NoModule="); // Affiche le message
    Serial.print(message.NoModule);
    Serial.print(" Temperature=");
    Serial.println(message.Temperature);*/
  }
}


