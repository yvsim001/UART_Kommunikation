#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000
#define BAUD 9600
#define UBRR_Value FOSC/16/BAUD-1

void USART_Init( uint16_t ubrr)
{
  UBRR0H = (uint8_t)(ubrr >>8);
  UBRR0L = (uint8_t)ubrr;
  UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
  while(!(UCSR0A & (1 <<UDRE0)));
  UDR0 = data;

}

unsigned char USART_Receive(void)
{
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void USART_Flush( void )
{
uint8_t dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

void Init_ADC(void)
{
  DDRC &= (1<<PC0);   //pin A0
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADPS0)|(1<<ADPS1);  //clock_ADC = 16Mhz/8 = 2Mhz
  ADMUX  |= (1<<REFS1);
  ADMUX  |= 0x00;
  ADMUX  |= (1<<ADLAR);
  DIDR0 |= (1<<ADC0D);
}

volatile unsigned char data;
uint16_t temperatur;
uint8_t y;


int main(void)
{
  

  USART_Init(UBRR_Value);
  USART_Flush();
  Init_ADC();
  DDRB= 0xFF;

  sei();


  while(1)
  {
    ADCSRA |= (1<<ADSC);
    while(ADCSRA &(1<<ADSC));
    y=ADC;
    y=y>>6;
    temperatur = ((y-102)*100)/204;
    PORTD = temperatur;
  }
}
ISR(USART_RX_vect)
{
  USART_Transmit(data= temperatur);
  PORTB = data;
}