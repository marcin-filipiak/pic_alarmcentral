

///-------------LED----------------
void blinkgreenred(int t){
   output_high(LED_RED); 
   output_high(LED_GREEN);  
   delay_ms(t); 
   output_low(LED_RED); 
   output_low(LED_GREEN); 
}

void blinkgreen(int t){
   output_high(LED_GREEN); 
   delay_ms(t); 
   output_low(LED_GREEN); 
}

void blinkred(int t){
   output_high(LED_RED);  
   delay_ms(t); 
   output_low(LED_RED); 
}

void led_state_display(){
        if (state == st_armed){
                blinkred(100);
                delay_ms(800);
        }
        if (state == st_unarmed){
                blinkgreen(100);
                delay_ms(800);
        }
        if (state == st_alarm){
                blinkgreenred(500);
        }
	if (state == st_postalarm){
                blinkgreenred(500);
        }
}

