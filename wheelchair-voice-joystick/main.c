/*

Programa para a interpreta��o do sinal oferecido pelos 2 sensores aceler�metro e sensor
de reconhecimento de voz para a movimenta��o de uma cadeira de rodas inteligente

Universidade Federal de Santa Catarina - Florian�polis, SC

Aluno: Felipe de Souza Tortato

Matr�cula: 13200964

E-mail: fstortato@gmail.com
        ftt@neo.ufsc.br
        
Telefone: (48) 9186-8383

*/

#include "C:\Users\Felipe\Documents\Projetos de Eletr�nica\Cadeira de rodas vers�o VOZ-JOYSTICK\Programa��o\main.h"
#include "Flex_LCD420.C" //Biblioteca LCD
#include "math.h" //Biblioteca de opera��es matem�ticas
#use  RS232(baud=9600, parity=N, bits=8, xmit=PIN_C6,rcv=PIN_A2) // configura serial

char pegaserial;
float xjoy,yjoy; // Vari�veis que recebem o valor dos joysticks
int dutydir, dutyesq; // Vari�veis que recebem o comando para a sa�da PWM
int x=0; // Trava do bot�o
int a=0; // Valor do bot�o

void main()
{

   setup_adc_ports(AN0_AN1_AN3);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_1,250,1);
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   set_pwm1_duty(dutydir);
   set_pwm2_duty(dutyesq);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);

   delay_ms(500);
   putc(0xAA);
   delay_ms(100);
   putc(0x37);
   delay_ms(500);
   putc(0xAA);
   delay_ms(100);
   putc(0x21);
   delay_ms(500);
   
   lcd_init(); // Inicia o LCD
   
while (true) // Enquanto o microncontrolador estiver ligado
   {
   
   setup_ccp1(CCP_PWM); // Inicia a porta PWM1
   set_pwm1_duty (dutydir); // Associa o valor do pwm1 � vari�vel dutydir
   setup_ccp2(CCP_PWM); // Inicia a porta PWM2
   set_pwm2_duty (dutyesq); // Associa o valor do pwm2 � vari�vel dutyesq
   
   /*if(!input(pin_a0) && x==0 && a==0) 
      {
      x=1; // Trava bot�o;
      a=1; // Liga o programa na fun��o voz
      }
   if(input(pin_a0)) x=0; // Destrava bot�o
   
   if(!input(pin_a0) && x==0 && a==1)
      {
      x=1; // Trava bot�o
      a=2; //Liga o programa na fun��o joystick
      }
   if(input(pin_a0)) x=0; // Destrava bot�o
   
   if(!input(pin_a0) && x==0 && a==2)
      {
      x=1; // Trava bot�o;
      a=0; // Desliga as fun��es
      }
   if(input(pin_a0)) x=0; // Destrava bot�o
   */
   a=1;
   switch (a)
      {
      case 0:
         lcd_gotoxy(1,1); // (Coluna 1, Linha 1) do LCD
         printf(lcd_putc,"Sem funcao");
         break;
         
      case 1:
         pegaserial = getc();
         lcd_gotoxy(1,1); // (Coluna 1, Linha 1) do LCD
         printf(lcd_putc,"Modo voz");
         lcd_gotoxy(1,2); // (Coluna 1, Linha 2) do LCD
         printf(lcd_putc,"Comando: %x", pegaserial);
         if (pegaserial==0x11)
         {
         dutydir=100;
         dutyesq=100;
         }
         if (pegaserial==0x12)
         {
         dutydir=0;
         dutyesq=0;
         }
         if (pegaserial==0x13)
         {
         dutydir=0;
         dutyesq=100;
         }
         if (pegaserial==0x14)
         {
         dutydir=100;
         dutyesq=0;
         }       
         break;
         
      case 2:
         lcd_gotoxy(1,1); // (Coluna 1, Linha 1) do LCD
         printf(lcd_putc,"Modo joystick");
         
         set_adc_channel(1); // Porta AN1
         xjoy=read_adc(); // Atribui o valor da entrada anal�gica ao X
         set_adc_channel(3); // Porta AN3
         yjoy=read_adc(); // Atribui o valor da entrada anal�gica ao Y
         
         lcd_gotoxy(1,2); // (Coluna 1, Linha 2) do LCD
         printf(lcd_putc,"%.0f", xjoy); // Mostrar no LCD o valor de X
         lcd_gotoxy(6,2); // (Coluna 6, Linha 2) do LCD
         printf(lcd_putc,"%.0f", yjoy); // Mostrar no LCD o valor de Y
         lcd_gotoxy(12,2); // (Coluna 12, Linha 2) do LCD
         printf(lcd_putc, "%d", x);
                          
         if (yjoy>=130) // Se inclina��o em Y for para frente
         {
            if (xjoy>122 && xjoy<130) // Joystick parado em rela��o a X
            {
            dutydir=(40*yjoy - 5200)/125; // Contribui��o do motor em y
            dutyesq=(40*yjoy - 5200)/125; // Contribui��o do motor em y
            }
            if (xjoy>=130) // Joystick inclinado para a direita
            {
            dutydir=(40*yjoy - 5200)/125 + (40*xjoy - 5200)/125; // Contribui��o do motor em Y + X
            dutyesq=(40*yjoy - 5200)/125; // Contribui��o do motor em Y
            }
            if (xjoy<=122) // Joystick inclinado para a esquerda
            {
            dutydir=(40*yjoy - 5200)/125; // Contribui��o do motor em Y
            dutyesq=(40*yjoy - 5200)/125 + (-40*xjoy + 4880)/122; // Contribui��o do motor em Y + X
            }
         }
         
         if (yjoy<=122) // Se a inclina��o em Y for para tr�s
            {
            dutydir=0; // Desliga o motor direito
            dutyesq=0; // Desliga o motor esquerdo
         
            if (xjoy>122 && xjoy<130) // Se o joystick estiver parado em rela��o a X
               {
               output_high(pin_d2); // Ligar o freio direito porta D2
               output_high(pin_d3); // Ligar o freio esquerdo porta D3
               }
            if (xjoy>=130) // Se estiver inclinado para a direita
               {
               output_high(pin_d2); // Ligar o freio direito porta D2
               output_low(pin_d3); // Desligar o freio esquerdo porta D3
               }
            if (xjoy<=122) // Se estiver inclinado para a esquerda
               {
               output_low(pin_d2); // Desligar o freio direito porta D2
               output_high(pin_d3); // Ligar o freio esquerdo porta D3
               }
            }
         if (yjoy>122 && yjoy<130)
            {
            output_low(pin_d2); // Desligar o freio direito porta D2
            output_low(pin_d3); // Desligar o freio esquerdo porta D3
            }
      break;      
      }
   }
}
