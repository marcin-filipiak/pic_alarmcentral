/**********************************
          CENTRALKA
**********************************/

#include <18F25k80.h> 
#fuses INTRC_IO, NOPROTECT, PUT, BROWNOUT, NOWDT
#use delay(clock=8000000) 
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS) 

///--------LINES DEFINES----------
#define DEF_LINE_STATE 0 // domyslny stan na linii czujek, przeciwny jest alarmem
#define L1 PIN_A0
#define L2 PIN_A1
#define L3 PIN_A2
#define L4 PIN_A3

///--------SIRENE DEFINE---------
#define SIR PIN_C0

///--------BUTTON DEFINE--------
#define SET PIN_B1

//---------LED DEFINE------------
#define LED_GREEN PIN_A7
#define LED_RED   PIN_A6

///--------DEVICE STATE DEFINES----
#define st_unarmed     0
#define st_armed       1
#define st_alarm       2
#define st_postalarm  3


///--------CAN DEFINES----------
#include <can-18F4580.c>  // Use correct driver 

#define CAN_ALARM_ID          20 
#define CAN_KEY_ID               21
#define CAN_LIGHTSTATION_ID   22
#define CAN_BROADCAST_ID   1

///-------EEPROM MAP---------------
#define EEPROM_LINE1            0x00
#define EEPROM_LINE2            0x01
#define EEPROM_LINE3            0x02
#define EEPROM_LINE4            0x03
#define EEPROM_LINECAN          0x04

#define EEPROM_LINE1_DELAY      0x05
#define EEPROM_LINE2_DELAY      0x06
#define EEPROM_LINE3_DELAY      0x07
#define EEPROM_LINE4_DELAY      0x08
#define EEPROM_LINECAN_DELAY    0x09

#define EEPROM_CARD1            0x10
#define EEPROM_CARD2            0x11
#define EEPROM_CARD3            0x12
#define EEPROM_CARD4            0x13
#define EEPROM_CARD5            0x14
#define EEPROM_CARD6            0x15
#define EEPROM_CARD7            0x16
#define EEPROM_CARD8            0x17
#define EEPROM_CARD9            0x18
#define EEPROM_CARD10           0x19

#define EEPROM_SIRENE_DELAY    0x20
#define EEPROM_REGSTATE            0x21

///ZMIENNE GLOBALNE
unsigned char state; //stan urzadzenia aktualny; alarm itp.
unsigned char reg_state; //stan urzadzenia zarejestrowany; alarm itp.

//zmienne dla magistrali CAN
struct rx_stat rxstat; 
int32 rx_id; 
int32 tx_id; 
int8 rx_len;                
unsigned int8 buffer[8];
unsigned int8 buffer2[8];


//zmienne zegara
byte sec,min,hour;
byte wassec=0;
byte sirene_timer=0;

int8 alarm_delay=-1;

#include "conversion.c"
#include "ds1307.c"
#include "rtc.c"
#include "led.c"
#include "sirene.c"
#include "lines.c"
#include "cards.c"
#include "functions.c"
#include "can_tools.c"
#include "lightstation_admin.c"

//====================================== 
void main(void) 
{
 //odczekanie po podlaczeniu zasilania
 delay_ms(500);
 
 printf("\nAlarm ok\n\n");
 blinkgreenred(100);
 
 //initlines(); // wykrycie czujek na liniach
 can_init();  // inicjacja can
 
 int i = 0;

//jesli alarm byl w stanie uzbrojenia, alarmu, postalarmu i zostal wylaczony odczytujemy jego stan 
if(read_eeprom(EEPROM_REGSTATE)>0 && read_eeprom(EEPROM_REGSTATE)<4){
     state = read_eeprom(EEPROM_REGSTATE);
     reg_state = read_eeprom(EEPROM_REGSTATE);
     //jesli odlaczono w stanie alarmu, to nalezy uaktywnic syrene
     if (state==st_alarm){
          sirene_on();
     }
}
//centrala uruchamiana na swiezo
else {
     state = st_unarmed;
     reg_state = st_unarmed;
}

 sendalarmstate();
 
 while(1) 
  {

//jesli jest alarm to wyjemy syrena tyle ile jest ustawione
 if (state == st_alarm){
             sirene_delay();
 }
   //sprawdzanie czy zmienil sie stan urzadzenia np. z czuwania na alarm
   state_monitor();
   
   //informacja o stanie na kontrolkach
   led_state_display();
   
   //analiza stanu na liniach wykryla alarm       
   if (watchlines()!=0){
             int8 z = watchlines();
             //pobranie odczekiwania dla danej linii przy pierwszym wywolaniu
        if (alarm_delay==-1){
         //pobranie wartosci odczekiwania dla danej linii
         alarm_delay=line_wait(z);
        }
        //zmniejszenie czasu odczekiwania jesli 
             //jest on mniejszy od aktualnie odczekiwanego
        else{
      if (line_wait(z)<alarm_delay){
         alarm_delay=line_wait(z);
      }
        }


             //jesli jest uzbrojony        
             if((state == st_armed) || (state == st_postalarm)){
                   alarm(z);
             }
    }
    
    
    //nacisniecie przycisku set
    if (input(SET) == true){
                //jesli byl uzbrojony, to rozbroic
                if (state == st_armed){
                        printf("unarmed\n");
                        state = st_unarmed;
                        //wyslanie stanu do czytnika
                        sendalarmstate();
                }
                //jesli byl rozbrojony
                else { }
    }
    
    //otrzymanie komunikatu z uart
    if(kbhit()) {
        char c=getc();
        //jesli jest rozbrojony
        if (state == st_unarmed){
                switch (c){
                           case '1': printf(">lines auto config\n"); initlines(); break;
                           case '2': printf(">lines config\n"); lines_config(); break;
                           case '3': printf(">lines monitor\n"); lines_monitor(); break;
                           case '4': printf(">sirene config\n"); sirene_config(); break;
                           case '5': printf(">cards\n"); cards_config(); break;
                           case '6': printf(">time\n"); time_config(); break;
                           case '7': printf(">armed\n"); state = st_armed; break;
                           case '8': cantools(); break;
                           case '9': lightstationadmin(); break;
                           default: 
                                printf("\n---------------------------\n"); 
                                printf("Main menu alarm central\n"); 
                                printf("---------------------------\n\n"); 
                                printf("1 - lines auto config\n"); 
                                printf("2 - lines config\n");
                                printf("3 - lines monitor\n");
                                printf("4 - sirene config\n");
                                printf("5 - cards\n");
                                printf("6 - time\n");
                                printf("7 - arm\n");
                                printf("8 - CAN tools\n");
                                printf("9 - LightStation admin\n");
                                break;
                }
        }
        //jesli uzbrojony
        if (state == st_armed){
                printf("First press SET button to disarm\n");
        }
    }
    
    //otrzymanie komunikatu z can    
    if(can_getd(rx_id, buffer, rx_len, rxstat)) { 
       if(rx_id == CAN_ALARM_ID || rx_id ==  CAN_BROADCAST_ID) { 
        //pokazanie otrzymanego komunikatu z CAN
        printf("\nMessage From CAN: %d %d %d %d %d %d %d %d \n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
         
        //proszenie o zgloszenie
        if (buffer[0] == 'P'){
              caniamlive();
        }         

         //wywolanie alarmu z can
         if (buffer[0] == 'a' && buffer[1] =='2'){
                    alarm();
         }

         //pytanie o stan centrali
         if (buffer[0] == 's'){
                delay_ms(200);
                //wyslanie stanu do czytnika
                sendalarmstate();
         }

         //otrzymano dane karty
         if (buffer[0] == 'c'){                      
            //czy karta znana
            if (card_know(buffer[1]) == 1){
               //jesli byl alarm to rozbroic
                if (state == st_alarm){
                       printf("alarm off and unarmed\n");
                       state = st_unarmed;
                }
                else{
                  //jesli byl uzbrojony, to rozbroic
                   if (state == st_armed){
                          printf("unarmed\n");
                          state = st_unarmed;
                   }
                   //jesli byl rozbrojony to uzbroic
                   else {
                         printf("armed\n");
                         state = st_armed;
                   }
                }
                delay_ms(300);
              }
             //karta nieznana
             else{
                printf("unknown card\n");
             }
          } 

        } 
      } 
      
 
             
  } 

}