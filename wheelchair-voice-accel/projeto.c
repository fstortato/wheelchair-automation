/*

Programa para a interpretação do sinal oferecido pelos 2 sensores acelerômetro e sensor
de reconhecimento de voz para a movimentação de uma cadeira de rodas inteligente

Universidade Federal de Santa Catarina - Florianópolis, SC

Aluno: Felipe de Souza Tortato

Matrícula: 13200964

E-mail: fstortato@gmail.com
        ftt@neo.ufsc.br
        
Telefone: (48) 9186-8383

*/

#include "C:\Users\Felipe\Documents\Projetos de Eletrônica\Cadeira de rodas versão ACEL-VOZ\projeto.h"
#include "Flex_LCD420.C" //Biblioteca LCD
#include "math.h" //Biblioteca de operações matemáticas

float xvar,yvar,xfix,yfix,xjoy,yjoy; //Variáveis que recebem o valor da aceleração dos dois acelerômetros
float difx,dify; // Variáveis que recebem o valor da diferença entre os valores fixos e variáveis
int x=0;
int dutydir, dutyesq; // Variáveis que recebem o comando para a saída PWM

void main()
{

   setup_adc_ports(AN0_AN1_AN4_AN5_AN6_AN7_VREF_VREF);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_1,99,1);
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   set_pwm1_duty(dutydir);
   set_pwm2_duty(dutyesq);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);

lcd_init(); // Inicia o LCD
   
while (true) // Enquanto o microncontrolador estiver ligado
   {
   setup_ccp1(CCP_PWM); // Inicia a porta PWM1
   set_pwm1_duty (dutydir); // Associa o valor do pwm1 à variável dutydir
   setup_ccp2(CCP_PWM); // Inicia a porta PWM2
   set_pwm2_duty (dutyesq); // Associa o valor do pwm2 à variável dutyesq
   
// Posição da chave:

   if (input(pin_d2)) // Se a chave estiver na posição acelerômetro
   {
      // Recebendo o valor das entradas analógicas
      
      set_adc_channel(0); // Porta AN0
      xvar=read_adc(); // Atribui o valor da entrada analógica ao X variável

      set_adc_channel(1); // Porta AN1
      yvar=read_adc(); // Atribui o valor da entrada analógica ao Y variável

      set_adc_channel(5); // Porta AN5
      xfix=read_adc(); // Atribui o valor da entrada analógica ao X fixo

      set_adc_channel(6); // Porta AN6
      yfix=read_adc(); // Atribui o valor da entrada analógica ao Y fixo

      // Mostrando o valor das entradas no LCD:

      lcd_gotoxy(1,1); // (Coluna 1, Linha 1) do LCD
      printf(lcd_putc,"%.0f", xvar); // Mostrar no LCD o valor de X variável
      delay_ms(500); // Delay para os valores não alterarem tão rapidamente
      lcd_gotoxy(1,2); // (Coluna 1, Linha 2) do LCD
      printf(lcd_putc,"%.0f", yvar); // Mostrar no LCD o valor de Y variável
      delay_ms(500); // Delay para os valores não alterarem tão rapidamente
      lcd_gotoxy(6,1); //(Coluna 6, Linha 1) do LCD
      printf(lcd_putc,"%.0f", xfix); // Mostrar no LCD o valor de X fixo
      delay_ms(500); // Delay para os valores não alterarem tão rapidamente
      lcd_gotoxy(6,2); //(Coluna 6, Linha 2) do LCD
      printf(lcd_putc,"%.0f", yfix); // Mostrar no LCD o valor de Y fixo
      delay_ms(500); // Delay para os valores não alterarem tão rapidamente

      // Cálculos com essas variáveis:
   
      difx = xvar - xfix;
      dify = yvar - yfix;
   
      // Mostrando os resultados no LCD:

      lcd_gotoxy(13,1); // (Coluna 13, Linha 1) do LCD
      printf(lcd_putc,"%3.1d",dutydir); // Mostrar no LCD o valor da diferença entre xvar e xfix
      lcd_gotoxy(13,2); // (Coluna 13, Linha 2) do LCD
      printf(lcd_putc,"%3.1d",dutyesq); // Mostrar no LCD o valor da diferença entre yvar e yfix

      // Condições de funcionamento:
   
      if (difx>40) // Se a variação de xvar em relação a xfix for maior do que 40 
      {
         dutydir=25; // Velocidade da roda direita em 25%
         dutyesq=0; // Velocidade da roda esquerda em 0%
         output_low(pin_d0); // Desligar o freio direito porta D0
         output_low(pin_d1); // Desligar o freio esquerdo porta D1
      }
      
      if (difx<-40) // Se a variação de xvar em relação a xfix for menor do que -40 
      {
         dutydir=0; // Velocidade da roda direita em 0%
         dutyesq=25; // Velocidade da roda esquerda em 25%
         output_low(pin_d0); // Desligar o freio direito porta D0
         output_low(pin_d1); // Desligar o freio esquerdo porta D1
      }
   
      if (difx<40 && difx>-40) // Se estiver na posição média
      {
         if (x==1) // Se a velocidade incial for igual a 25
         {
            dutydir=25; // Velocidade da roda direita em 25%
            dutyesq=25; // Velocidade da roda esquerda em 25%
            output_low(pin_d0); // Desligar o freio direito porta D0
            output_low(pin_d1); // Desligar o freio esquerdo porta D1
         }
   
         if (x==0) // Se a velocidade incial for igual a 0
         {
            dutydir=0; // Velocidade da roda direita em 0%
            dutyesq=0; // Velocidade da roda esquerda em 0%
            output_low(pin_d0); // Desligar o freio direito porta D0
            output_low(pin_d1); // Desligar o freio esquerdo porta D1
         }
      }
   
      if (dify>40) // Se a variação de yvar em relação a yfix for maior do que 40
      {
         if (x==0) // Se a velocidade incial for 0
         {
            dutydir=25; // Velocidade da roda direita em 25%
            dutyesq=25; // Velocidade da roda esquerda em 25%
            output_low(pin_d0); // Desligar o freio direito porta D0
            output_low(pin_d1); // Desligar o freio esquerdo porta D1
            //delay_ms(1500); // Delay para a pessoa ter um tempo pra voltar para a posição inicial
            x=1; // Varíavel para mostrar se os motores estão ligados ou não (ligados)
         }
      
         if (x==1) // Se a velocidade inicial for 25
         {
            dutydir=50; // Velocidade da roda direita em 50%
            dutyesq=50; // Velocidade da roda esquerda em 50%
            output_low(pin_d0); // Desligar o freio direito porta D0
            output_low(pin_d1); // Desligar o freio esquerdo porta D1
         }
      }
         
      if (dify<-40) // Se a variação de yvar em relação a yfix for menor do que -40 
      {
         dutydir=0; // Velocidade da roda direita em 0%
         dutyesq=0; // Velocidade da roda esquerda em 0%
         output_high(pin_d0); // Ligar o freio direito porta D0
         output_high(pin_d1); // Ligar o freio esquerdo porta D1
         x=0; // Varíavel para mostrar se os motores estão ligados ou não (desligados)
      }
   
      if (dify<40 && dify>-40) // Posição média
      {
         output_low(pin_d0); // Desligar o freio direito porta D0
         output_low(pin_d1); // Desligar o freio esquerdo porta D1
      }
   }
   if (input(pin_d3)) // Se a chave estiver na posição joystick
   {
      // Recebendo o valor das entradas analógicas
      
      set_adc_channel(4); // Porta AN4
      xjoy=read_adc(); // Atribui o valor da entrada analógica ao X do joystick

      set_adc_channel(7); // Porta AN7
      yjoy=read_adc(); // Atribui o valor da entrada analógica ao Y do joystick
    
      
      if (yjoy>=130) // Se inclinação em Y for para frente
      {
         if (xjoy>122 && xjoy<130) // Joystick parado em relação a X
         {
            dutydir=(40*yjoy - 5200)/125; // Contribuição do motor em y
            dutyesq=(40*yjoy - 5200)/125; // Contribuição do motor em y
         }
         if (xjoy>=130) // Joystick inclinado para a direita
         {
            dutydir=(40*yjoy - 5200)/125 + (40*xjoy - 5200)/125; // Contribuição do motor em Y + X
            dutyesq=(40*yjoy - 5200)/125; // Contribuição do motor em Y
         }
         if (xjoy<=122) // Joystick inclinado para a esquerda
         {
            dutydir=(40*yjoy - 5200)/125; // Contribuição do motor em Y
            dutyesq=(40*yjoy - 5200)/125 + (-40*xjoy + 4880)/122; // Contribuição do motor em Y + X
         }
      }
      if (yjoy<=122) // Se a inclinação em Y for para trás
      {
         dutydir=0; // Desliga o motor direito
         dutyesq=0; // Desliga o motor esquerdo
         
         if (xjoy>122 && xjoy<130) // Se o joystick estiver parado em relação a X
         {
            output_high(pin_d0); // Ligar o freio direito porta D0
            output_high(pin_d1); // Ligar o freio esquerdo porta D1
         }
         if (xjoy>=130) // Se estiver inclinado para a direita
         {
         output_high(pin_d0); // Ligar o freio direito porta D0
         output_low(pin_d1); // Desligar o freio esquerdo porta D1
         }
         if (xjoy<=122) // Se estiver inclinado para a esquerda
         {
         output_low(pin_d0); // Desligar o freio direito porta D0
         output_high(pin_d1); // Ligar o freio esquerdo porta D1
         }
      }
      if (yjoy>122 && yjoy<130)
      {
         output_low(pin_d0); // Desligar o freio direito porta D0
         output_low(pin_d1); // Desligar o freio esquerdo porta D1
      }
   }
   }
}
