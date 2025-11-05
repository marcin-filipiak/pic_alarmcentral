///-------------SYRENA-------------

//konfiguracja syreny
void sirene_config(){
        printf("Sirene will be active %d min after alarm\n",read_eeprom(EEPROM_SIRENE_DELAY));
   printf("Set to 1-1min 2-2min 3-5min\n");
        char c=getc();
        //1minuta
        if (c=='1'){
            write_eeprom(EEPROM_SIRENE_DELAY,1);
        }
        //2minuty
         if (c=='2'){
                 write_eeprom(EEPROM_SIRENE_DELAY,2);
         }
         //5minut
          if (c=='3'){
                 write_eeprom(EEPROM_SIRENE_DELAY,5);
          }
}

void alarm(int8 &line=0){
        state = st_alarm;
        printf("ALARM on line %d\n",line);
}

void sirene_on(){
        output_high(SIR);
        wassec=0;
        sirene_timer=0;
        read_time();
        printf("%02d:%02d:%02d Sirene ON\n", hour,min,sec);
}

void sirene_off(){
        output_low(SIR);
   read_time();
        printf("%02d:%02d:%02d Sirene OFF\n", hour,min,sec);
}


void sirene_delay(){
   //jesli syrena wyje wystarczajaco dlugo to wylaczamy i status post alarm
   if((sirene_timer/60) == read_eeprom(EEPROM_SIRENE_DELAY)){
       sirene_off();
       state = st_postalarm;
                 sirene_timer=sirene_timer+60;
   }
   //syrena jeszcze powinna wyc
   if((sirene_timer/60) < read_eeprom(EEPROM_SIRENE_DELAY)){
      //teraz sprawdzamy czas
      delay_ms(800);
      read_time();
           //printf("%02d:%02d:%02d\n", hour,min,sec);
      //jesli uplynela przynajmniej minuta
           if (wassec!=sec){
         wassec = sec;
         sirene_timer++;
         //printf("Sirene %d min is running\n",(sirene_timer/60));
      }
        }
   
}
