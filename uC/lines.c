
///------------LINIE ALARMU----------------
//sprawdzanie linii alarmu, zwrot id naruszonej linii jesli jest wlaczona
unsigned char watchlines(){
         if ((input(L1) != DEF_LINE_STATE) && (read_eeprom(EEPROM_LINE1) == 1)){
                return 1;
          }         
          if ((input(L2) != DEF_LINE_STATE) && (read_eeprom(EEPROM_LINE2) == 1)){
                return 2;
          }
          if ((input(L3) != DEF_LINE_STATE) && (read_eeprom(EEPROM_LINE3) == 1)){
                return 3;
          }
          if ((input(L4) != DEF_LINE_STATE) && (read_eeprom(EEPROM_LINE4) == 1)){
                return 4;
          }
          return 0;
}

//pobranie czasu odczekiwania po wywolaniu z danej linii 1-4 natomiast 5to CAN
unsigned char line_wait(int8 line){
	  //domyslnie zero odczekiwania
	  int8 wait=0;
	  //sprawdzenie ile ma odczekac po wywolaniu z danej linii
	  switch(line){
		
		case 1:    
			wait=read_eeprom(EEPROM_LINE1_DELAY);
			break;

		case 2:    
			wait=read_eeprom(EEPROM_LINE2_DELAY);
			break;


		case 3:    
			wait=read_eeprom(EEPROM_LINE3_DELAY);
			break;


		case 4:    
			wait=read_eeprom(EEPROM_LINE4_DELAY);
			break;
		//odczekanie po wywolaniu z can
		case 5:    
			wait=read_eeprom(EEPROM_LINECAN_DELAY);
			break;



          }
          return wait;
}

void lines_monitor(){
	do{
	    if (input(L1)){ printf("1");}else{ printf("0");}
	    if (input(L2)){ printf("1");}else{ printf("0");}
	    if (input(L3)){ printf("1");}else{ printf("0");}
	    if (input(L4)){ printf("1");}else{ printf("0");}
	    printf("\n");
	    delay_ms(800);
	}while(!kbhit());
}

//sprawdzenie do jakich linii podlaczone sa czujki i ich wlaczenie
void initlines(){
          //wylaczenie wszystkich linii
          write_eeprom(EEPROM_LINE1,0);
          write_eeprom(EEPROM_LINE2,0);
          write_eeprom(EEPROM_LINE3,0);
          write_eeprom(EEPROM_LINE4,0);
          
          //jesli jest podpieta czujka wlacz linie1
          if (input(L1) != DEF_LINE_STATE){
                write_eeprom(EEPROM_LINE1,1);
                printf("Line1 ON\n");
          }    
          else{
                write_eeprom(EEPROM_LINE1,0);
                printf("Line1 OFF\n");
          }     
          //jesli jest podpieta czujka wlacz linie2
          if (input(L2) != DEF_LINE_STATE){
                write_eeprom(EEPROM_LINE2,1);
                printf("Line2 ON\n");
          }
          else{
                write_eeprom(EEPROM_LINE2,0);
                printf("Line2 OFF\n");
          }     
          //jesli jest podpieta czujka wlacz linie3
          if (input(L3) != DEF_LINE_STATE){
                write_eeprom(EEPROM_LINE3,1);
                printf("Line3 ON\n");
          }
          else{
                write_eeprom(EEPROM_LINE3,0);
                printf("Line3 OFF\n");
          }               
          //jesli jest podpieta czujka wlacz linie4
          if (input(L4) != DEF_LINE_STATE){
                write_eeprom(EEPROM_LINE4,1);
                printf("Line4 ON\n");
          }
          else{
                write_eeprom(EEPROM_LINE4,0);
                printf("Line4 OFF\n");
          }     
}


///------------config linii alarmu------------------
void lines_config(){
        printf("1 - Line ON/OFF\n");
        printf("2 - Line delay\n");
        char c=getc();
        switch (c){
                   //wlaczenie wylaczenie linii
                   case '1': 
                        printf(">Line ON/OFF\n");
         		int8 xx;
                        for(xx=0;xx<5;xx++){
                                printf("Line %d is set to ",xx);
                                if (read_eeprom(xx) == 1){
                                        printf("ON\n");
                                }
                                else {
                                        printf("OFF\n");
                                }
                                printf("Set to 1-ON 2-OFF\n");
                                c = getc();
                                if (c=='1'){
                                        write_eeprom(xx,1);
                                        printf("Set ON\n");
                                }
                                else {
                                        write_eeprom(xx,0);
                                        printf("Set OFF\n");
                                }
                        }
                        break;
                   //opoznienie linii
                   case '2': 
                        printf(">Line delay\n");
                        for(int x=EEPROM_LINE1_DELAY;x<(EEPROM_LINE4_DELAY+1);x++){
                                printf("Line %d delay %d sec \n",x,read_eeprom(x));
                                printf("Set to 1-0sec 2-30sec 3-60sec\n");
                                c = getc();
                                //wlaczenie zaraz po naruszeniu 
                                if (c=='1'){
                                        write_eeprom(x,0);
                                }
                                //30sec po naruszeniu
                                if (c=='2'){
                                        write_eeprom(x,30);
                                }
                                //60sec po naruszeniu
                                if (c=='3'){
                                        write_eeprom(x,60);
                                }
                        }                        
                        break;
        }
}
