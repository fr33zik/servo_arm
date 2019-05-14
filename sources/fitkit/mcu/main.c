/*******************************************************************************
   main.c:
   original: LCD + keyboard demo

   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$

changed to:  MCU PWM demo
Changed by: Jaroslav Jablonický

*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include <stdlib.h>
#include <string.h>


#define SAMPL         6500
#define SMUL_MID      500
#define SMUL_LEFT     360
#define SMUL_RIGHT    650

char last_ch; //naposledy precteny znak
char char_cnt;
int interrupt_to_period = 1;
//TICKS_PER_SECOND / frequency / samples;
double ticks = 32768.0/50/SAMPL;
int index = 2;
int samples_mul[4] = {SMUL_MID,SMUL_MID,SMUL_MID,SMUL_MID}; //pole drzci pozice serv
unsigned char str_tmp[MAX_COMMAND_LEN+1];


/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
}


/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
  char ch;
  ch = key_decode(read_word_keyboard_4x4());
  if (ch != last_ch)
  {
    last_ch = ch;
    if (ch != 0)
    {
      //dekodovani klavesnice a nastaveni move polohy / vyber serva
      switch(ch)
      {
        case 'A':
          index = 0;
          break;
          case 'B':
          index = 1;
          break;
          case 'C':
          index = 2;
          break;
          case 'D':
          index = 3;
          break;
        case '1':
          samples_mul[index] = SMUL_LEFT;
          break;
        case '2':
          samples_mul[index]  = SMUL_MID;
          break;
        case '3':
          samples_mul[index]  = SMUL_RIGHT;
          break;
        case '4':
          if (((samples_mul[index] )-5) <= SMUL_LEFT )
          {
            samples_mul[index]  = SMUL_LEFT;
          }else
          {
            samples_mul[index]  =  samples_mul[index]  - 5;
          }
          break;
        case '6':
          if (((samples_mul[index]) +5) >= SMUL_RIGHT )
          {
            samples_mul[index]  = SMUL_RIGHT;
          }else
          {
            samples_mul[index]  = samples_mul[index]  + 5;
          }
          break;
        default:
          break;
      }

      term_send_crlf();
      term_send_str("Na klavesnici byla stisknuta klavesa \'");
      term_send_char(ch);
      term_send_char('\'');
      term_send_crlf();
      term_send_str(" >");

      if (char_cnt == 16) {
         LCD_clear();
         LCD_append_string("KB demo ");
         char_cnt = 0;
      }
      LCD_append_char(ch);
      if (char_cnt > 7) {
         LCD_rotate();
      }
      char_cnt++;

    }
  }
  return 0;
}



/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
 void get_data(unsigned char *str, int from)
{
  int i, j = 0;
  for (i = from; i < strlen(str); i++){ str_tmp[j++] = str[i]; }
  str_tmp[j] = 0;
}

unsigned char decode_user_cmd(char *UString, char *String)
{
  if (strcmp4(UString, "SIG "))  // zmena typu signalu
  {
      get_data(String, 4);  // do str_tmp ulozi UString od 4. znaku dal
    unsigned char *msg;

    if (strcmp3(str_tmp, "-90"))  //
    {
      samples_mul[index]  = SMUL_LEFT;
      msg = "Generuju 1ms";
    }else if (strcmp3(str_tmp, "+90"))  //
    {
      samples_mul[index]  = SMUL_RIGHT;
      msg = "Generuju 2ms";
    }else if (strcmp3(str_tmp, "000"))  //
    {
      samples_mul[index]  = SMUL_MID;
      msg = "Generuju 1.5ms";
    }
  }else{
   return (CMD_UNKNOWN);}

  return (USER_COMMAND);
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
  LCD_init();
  LCD_clear();
  LCD_append_string("KB demo ");
}


/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
  unsigned int cnt = 0;
  char_cnt = 0;
  last_ch = 0;

  initialize_hardware();
  keyboard_init();

  set_led_d6(1);                       // rozsviceni D6
 //set_led_d5(1);                       // rozsviceni D5

  //resetovani registru ktere sa budu pouzivat
  P2DIR |= 0x01;
  P2OUT &= 0xfe;
  P3DIR |= 0x01;
  P3OUT &= 0xfe;
  P4DIR |= 0x01;
  P4OUT &= 0xfe;
   P6DIR |= 0x01;
  P6OUT &= 0xfe;

  CCTL0 = CCIE;                             // povol preruseni pro casovac (rezim vystupni komparace)
  CCR0 = 0x8000;                            // nastav po kolika ticich (16384 = 0x4000, tj. za 1/2 s) ma dojit k preruseni
  TACTL = TASSEL_1 + MC_2;                  // ACLK (f_tiku = 32768 Hz = 0x8000 Hz), nepretrzity rezim
  while (1)
  {
    delay_ms(10);
    cnt++;
    if (cnt > 50)
    {
      cnt = 0;
      flip_led_d6();                   // negace portu na ktere je LED
    }

    keyboard_idle();                   // obsluha klavesnice
    terminal_idle();                   // obsluha terminalu
  }
}
interrupt (TIMERA0_VECTOR) Timer_A (void)
{
 // flip_led_d5();     //invertuj bit

 //preklapani vystupu
  switch(index)
  {
    case 0:
     P2OUT ^= 0x01;
     break;
    case 1:
     P3OUT ^= 0x01;
     break;
    case 2:
     P4OUT ^= 0x01;
     break;
    case 3:
     P6OUT ^= 0x01;
     break;
     default:
     break;

  }
  if(interrupt_to_period)
  {
    CCR0 += ticks*samples_mul[index];    // nastav po kolika ticich ma dojit k dalsimu preruseni( ma dojit k dobezne hrane)
  }else
  {
    CCR0 += ticks* (SAMPL-samples_mul[index]);    // nastav po kolika ticich ma dojit k dalsimu preruseni( ma dojit k nabezne hrane, zacatek dalsi periody)
  }
  interrupt_to_period ^= 0x01;
}
