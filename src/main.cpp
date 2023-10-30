#include <avr/io.h>
#define FOSC 16000000
#define BAUD 9600
#define UBRR_Value FOSC/16/BAUD-1

void USART_Init( uint16_t ubrr)
{
  UBRR0H = (uint8_t)(ubrr >>8);
  UBRR0L = (uint8_t)ubrr;
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void USART_Transmit( uint8_t data )
{
  while(!(UCSR0A & (1 <<UDRE0)));
  UDR0 = data;

}

uint8_t USART_Receive(void)
{
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}
void USART_Flush( void )
{
uint8_t dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
int main(void)
{
  uint8_t data;
  USART_Init(UBRR_Value);
  USART_Flush();
  DDRB= 0xFF;

  while(1)
  {
    USART_Transmit(data=USART_Receive());
    PORTB=data;
  }

}