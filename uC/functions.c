
//wczytuje numer 0-999 z "klawiatury" poprzez UART
int getnum()
{
  int a; // Accumulator
  char c; // Current character
  int  d; // Current digit
  int i; // Array index

  char s[3];
  printf("[");
  for(int8 x=0;x<3;x++){
      s[x] = getc();
      printf("%c",s[x]);
  }
  printf("]");

  i = 0; // Init array index
  a = 0; // Init accumulator to zero.
  c = s[i]; // Get first character
  i++; //Advance the index

  // Convert while not null terminator and is a numeric character
  while((c!=0)&&(c>='0')&&(c<='9'))
  {
    d = c - '0'; // Convert character to digit
    a = a * 10;  // Make room for digit
    a = a + d; // Add digit to accumulator
    c = s[i]; // Get next character
    i++; //Advance the index
  }
  return(a);
}




void clear_buffer(){
   for(int8 x=0;x<8;x++){
      buffer[x]=0;
   }
}


void sendalarmstate(){
   //wyslanie stanu do czytnika
   clear_buffer();
   buffer[0]='a';
   buffer[1]=state;
   can_putd(CAN_BROADCAST_ID, buffer, sizeof(buffer), 1, 1, 0);
   printf("\nMessage To CAN: %d %d %d %d %d %d %d %d \n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
}

void state_monitor(){
   //sprawdzenie czy zmienil sie stan urzadzenia i podjecie dzialan
   if (state != reg_state){
         reg_state = state;
        write_eeprom(EEPROM_REGSTATE,state);
        //wyslanie stanu do klawiatury
        sendalarmstate();
        //jesli alarm, wlaczenie syreny
        if (state == st_alarm){
             sirene_on();
         }
        //jesli rozbrojony, wylaczenie syreny
        if (state == st_unarmed){
               sirene_off();
         }
   }
}

//rozgloszenie istnienia urzadzenia w sieci
void caniamlive(){
        delay_ms(10*CAN_ALARM_ID);
        clear_buffer();
        buffer[0] = 'L';
        buffer[1] = CAN_ALARM_ID;
        can_putd(CAN_BROADCAST_ID, buffer, sizeof(buffer), 1, 1, 0);
}

