//wlaczanie swiatla z modulacja
void softlight(){
    clear_buffer();
    buffer[0]='m'; //modulacja swiatlem sciemnianie i rozjasnianie

    printf("\n>Soft light modulation for LED");

    printf("\nLight station Id:");
    int8 id =  getnum();
    buffer[1]=id;

    printf("\nactivation y/n");
    char c=getc();
    switch (c){
            case 'y': buffer[2]=1; break;
            case 'n': buffer[2]=0; break;              
    }
    can_putd(CAN_LIGHTSTATION_ID, buffer, sizeof(buffer), 1, 1, 0);   
    printf("\nok");
}

//wlaczenie swiatla po wykryciu ruchu
void setlightontik(){
    clear_buffer();
    buffer[0]='l';

    printf("\nlight on motion y/n");
    char c=getc();
    switch (c){
            case 'y': buffer[1]=1; break;
            case 'n': buffer[1]=0; break;              
    }
    can_putd(CAN_LIGHTSTATION_ID, buffer, sizeof(buffer), 1, 1, 0);   
    printf("\nok");
}

void lightstationadmin(){
   printf("\n----LIGHTSTATION ADMIN-----\n");              
   printf("1 - soft light\n");
   printf("2 - light on motion\n");

   while (kbhit());

   char c=getc();
   switch (c){
            case '1': softlight(); break;
            case '2': setlightontik(); break;
           
   }
}
