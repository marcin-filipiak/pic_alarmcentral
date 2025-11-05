
//wywolaj wszystkie urzadzenia na can
void candiscover(){
       clear_buffer();
       buffer[0] = 'P';
       can_putd(CAN_BROADCAST_ID, buffer, sizeof(buffer), 1, 1, 0);
}


//wyslanie komunikatu na can - manualnie
void canmanualmsg(){
    
    printf("\nRecipient Id:");
    int8 id =  getnum();
    printf("\n msg:");
 
    int8 a[8];  
    for(int8 x=0; x<8; x++){
      a[x] = getnum();
    }
    
    printf("\nSend y/n");
    char c=getc();
    switch (c){
            case 'y':  can_putd(id, a, sizeof(a), 1, 1, 0); break;
            case 'n':  break;
            default: break;
   }
  printf("\nok");
}

void cantools(){
   printf("\n---------CAN TOOLS---------\n");           
   printf("1 - discover devices\n");
   printf("2 - send message on can\n");

   while (kbhit());

   char c=getc();
   switch (c){
            case '1': printf(">devices on CAN:\n"); candiscover(); break;
            case '2': printf(">NOT IMPLEMENTED\n"); canmanualmsg(); break;
   }
}
