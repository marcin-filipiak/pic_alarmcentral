
//-----konfiguracja kart---------------
void cards_config(){
        unsigned int8 readed=0;
        unsigned int8 added=0;
        printf("1 - list\n");
        printf("2 - add card\n");
        printf("3 - remove card\n");
        char c=getc();
        switch (c){
           //lista kart
           case '1':
                for(int8 eeprom_address=EEPROM_CARD1;eeprom_address<EEPROM_CARD10+1;eeprom_address++){
                        printf("%d : %d\n",(eeprom_address-EEPROM_CARD1),read_eeprom(eeprom_address));
                }
                break;
           //dodaj karte
           case '2': 
                printf("waiting for new card (or any key to abort)....\n");
                while((readed != 1)&&(!kbhit())){
                //odebrano dane z can
                if(can_getd(rx_id, buffer, rx_len, rxstat)) 
                { 
                   //dane dla tego urzadzenia
                   if(rx_id == CAN_ALARM_ID){
                      //dane zawieraja crc karty
                      if (buffer[0] == 'c'){
		      //przelecenie po pamieci gdzie sa zapisane crc kart
                      for(int8 eeprom_address=EEPROM_CARD1;eeprom_address<EEPROM_CARD10+1;eeprom_address++){
			  //jesli karta juz jest w pamieci to konczymy
			  if (read_eeprom(eeprom_address)==buffer[1]){
				printf("Card was added here\n");
				added = 1;
                                break;
			  }
                          //pewnie karty niebylo w pamieci wiec dopisujemy
                          if (read_eeprom(eeprom_address)==-1 || read_eeprom(eeprom_address)==0){
                                write_eeprom(eeprom_address,buffer[1]);
                                printf("Card %d added on %d place\n",buffer[1],(eeprom_address-EEPROM_CARD1));
                                added = 1;
                                break;
                          }
                      }
		      //karta nie zostala dodana i skonczyl sie przelot po wolnej pamieci
                      if (added == 0){
                        printf("Not empty space for new card\n");
                      }
                      readed = 1;
                      }
                   }
                }
             	}                             
                break;
           //usun karte
           case '3':
                for(int8 eep=EEPROM_CARD1;eeprom_address<EEPROM_CARD10+1;eeprom_address++){
                        printf("%d : %d\n",(eep-EEPROM_CARD1),read_eeprom(eep));
                }
                printf("Id to remove:\n\n");
                c=getc();
                //c z zakresu od 0 do 9
                if (c>47 && c<58){
                        //przeliczenie z ascii tak by uzyskac numer dec
                        c = c-48;
                        //wykasowanie zawartosci
                        write_eeprom(EEPROM_CARD1+c,0);
                        printf("removed\n");
                }
                else printf("Wrong id\n");
                break;
           }
}


//----Czy karta jest znana
unsigned int8 card_know(int8 buffer){
 unsigned int8 know =0; //czy znana karta 1-tak 0-nie

            //przeszukanie w pamieci czy znana karta
            for(int8 eeprom_address=EEPROM_CARD1;eeprom_address<EEPROM_CARD10+1;eeprom_address++){
                  if (read_eeprom(eeprom_address)== buffer){
                          know = 1;
                          break;
                  }
            }
  return know;
}


