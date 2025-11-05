//-------------ZEGAR--------------
void read_time()
{
   sec = bcdToDec(read_ds1307 (0)); // read second
   min = bcdToDec(read_ds1307 (1)); // read minute
   hour = bcdToDec(read_ds1307 (2)); // read hour
}

void set_time()
{
   write_ds1307 (4, decToBcd(1)); //day
   write_ds1307 (5, decToBcd(1)); //month
   write_ds1307 (6, decToBcd(0)); //year
   write_ds1307 (2, decToBcd(hour)); //hour;
   write_ds1307 (1, decToBcd(min)); //minute
   write_ds1307 (0, decToBcd(0)); //second

}


void time_config(){
        read_time();
        printf("%02d:%02d:%02d \n", hour,min,sec);
        printf("h - set hour\n");
        printf("m - set minute\n");
        printf("(u - up, d - down, s - set)\n");
        char c=getc();
        switch (c){
                   case 'h': 
                        while(c != 's'){
                                printf("%02d:%02d \n", hour,min);
                                c = getc();
                                if (c=='u'){
                                        if (hour<25) hour = hour + 1;
                                        else hour = 0;
                                }
                                if (c=='d'){
                                        if (hour>0) hour = hour - 1;
                                        else hour = 24;
                                }
                        }
                        set_time();
                        printf("saved\n");
                        break;
                   case 'm': 
                        while(c != 's'){
                                printf("%02d:%02d \n", hour,min);
                                c = getc();
                                if (c=='u'){
                                        if (min<59) min = min + 1;
                                        else min = 0;
                                }
                                if (c=='d'){
                                        if (min>0) min = min - 1;
                                        else min = 59;
                                }
                        }
                        set_time();
                        printf("saved\n");
                        break;
        }
}

