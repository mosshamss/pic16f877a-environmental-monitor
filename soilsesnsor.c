// LCD module connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End LCD module connections
int dht11_response, b;
char Data, RH_integral, RH_decimal, T_integral, T_decimal, checksum;
int x;
float percentage;
char pecenttxt[10], tempTxt[10], RHTxt[10];

void dht11_init(){
     dht11_response = 0;
     TRISD.B0 = 0; // configure RD0 as an output
     PORTD.B0 = 0; // start signal
     Delay_ms(18);
     PORTD.B0 = 1;
     Delay_us(40);
     TRISD.B0 = 1; // configure RD0 as an input
     Delay_us(40);
     if (PORTD.B0 == 0){
        Delay_us(80);
        if (PORTD.B0 == 1){
           dht11_response = 1;
           Delay_us(40);
           }
     }
}

char dht11_read(){
      for(b = 0; b<8; b++){
              while(PORTD.B0 == 0);
              Delay_us(30);
              if (PORTD.B0 == 0){
                 Data &= ~(1<<(7-b));
              }
              else{
                   Data |= (1<<(7-b));
                   while(PORTD.B0 == 1);
              }
      }
      return Data;
}


void main() {
       Lcd_Init();
       Lcd_Cmd(_LCD_CURSOR_OFF);
       ADC_Init();
       Delay_ms(10);
       Lcd_Out(2, 5, "hello");
       Lcd_Cmd(_LCD_CLEAR);
       while(1){
                x = ADC_Read(0);
                Delay_ms(10);
                percentage = (x*100.0)/1024.0;
                Lcd_Out(1, 1, "Moisture: ");
                IntToStr((int)percentage, pecenttxt);
                Ltrim(pecenttxt);
                Lcd_Out(1, 11, pecenttxt);
                Lcd_Chr_Cp('%');
                dht11_init();
                if (dht11_response == 1){
                   RH_integral = dht11_read();
                   RH_decimal = dht11_read();
                   T_integral = dht11_read();
                   T_decimal = dht11_read();
                   checksum = dht11_read();
                   if (checksum == (RH_integral + RH_decimal + T_integral + T_decimal)){
                      Lcd_Out(2, 1, "T: ");
                      IntToStr(T_integral, tempTxt);
                      Ltrim(tempTxt);
                      Lcd_Out(2, 4, tempTxt);
                      Lcd_Chr_Cp('C');
                      Lcd_Out(2, 8, "H: ");
                      IntToStr(RH_integral, RHTxt);
                      Ltrim(RHTxt);
                      Lcd_Out(2, 11, RHTxt);
                        Lcd_Chr_Cp('%');

                      
                   }
                }
                Delay_ms(250);
                Lcd_Cmd(_LCD_CLEAR);
       }
}