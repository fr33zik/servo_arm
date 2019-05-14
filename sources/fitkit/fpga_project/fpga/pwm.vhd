 --  
 -- This file is part of the fr33ziks distribution (https://github.com/fr33zik/servo_arm).
 -- Copyright (c) 2019 Jaroslav Jablonicky.
 -- 
 -- This program is free software: you can redistribute it and/or modify  
 -- it under the terms of the GNU General Public License as published by  
 -- the Free Software Foundation, version 3.
 --
 -- This program is distributed in the hope that it will be useful, but 
 -- WITHOUT ANY WARRANTY; without even the implied warranty of 
 -- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 -- General Public License for more details.
 --
 -- You should have received a copy of the GNU General Public License 
 -- along with this program. If not, see <http://www.gnu.org/licenses/>.
 --


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity pwm is
port ( -- Sem doplnte popis rozhrani obvodu.

		RESET : IN std_logic;
		SMCLK : IN std_logic;
		
		--LED : OUT std_logic;
		--ROW : OUT std_logic_vector( 7 downto 0);
		DMXSEL : in std_logic_vector ( 1 downto 0);
		INPUT : in std_logic_vector ( 3 downto 0);
		OUTPUT : out std_logic_vector ( 3 downto 0);
		
		KIN       : out std_logic_vector(3 downto 0);
      	KOUT      : in  std_logic_vector(3 downto 0)
);
end pwm;



architecture main of pwm is

    -- Sem doplnte definice vnitrnich signalu.
	 --signal iled 	: 	std_logic_vector( 7 downto 0) := (others => '1');
	 --signal irow 	:	std_logic_vector( 7 downto 0) := (others => '0');
	 --signal cnt 	: 	std_logic_vector( 18 downto 0) := (others => '0');
	 --signal cnt8 	: 	std_logic_vector( 8 downto 0) := (others => '0');
	 --signal cnt50hz : 	std_logic					  := '0';
	 
	 
	 signal ref: std_logic_vector(18 downto 0):= (others => '0');
	 signal servo0 	: 	std_logic_vector( 15 downto 0) := x"2b5a";
	 signal servo1 	: 	std_logic_vector( 15 downto 0) := x"6528";
	 signal servo2 	: 	std_logic_vector( 15 downto 0) := x"9ef6";
	 signal servo3 	: 	std_logic_vector( 15 downto 0) := x"d8c4";
	 signal buf_reg 	: 	std_logic_vector ( 3 downto 0):= (others => '0');
--	 signal ref_next: std_logic_vector(3 downto 0):= (others => '0');
	 signal buf_next 	: 	std_logic_vector ( 3 downto 0):= (others => '0');
	 
	    -- keyboard 4x4
   signal kbrd_data_out   : std_logic_vector (15 downto 0);
   signal kbrd_data_vld  : std_logic;
   signal key_read_en   : std_logic;
	 	  


		  -- Keyboard 4x4
   component keyboard_controller
      port(
         CLK      : in std_logic;
         RST      : in std_logic;

         DATA_OUT : out std_logic_vector(15 downto 0);
         DATA_VLD : out std_logic;

         KB_KIN   : out std_logic_vector(3 downto 0);
         KB_KOUT  : in  std_logic_vector(3 downto 0)
      );
   end component;
	
begin

    -- 19bit citac
    -- znizuje frekvenci na 50hz
    -- synchronizuje vystup na serva na nabeznu hranu
	 process(RESET, SMCLK)
	begin
	if (RESET = '1') then
				ref <= (others => '0');
				buf_reg <= (others => '0');
	elsif (SMCLK'event) and (SMCLK='1') then
			ref <= ref+1;
			buf_reg <= buf_next;
			if (ref = x"24000") then 
				ref <= (others => '0');
			end if;
			
		end if;
	end process;	
	
	   -- Keyboard controller
   kbrd_ctrl: entity keyboard_controller(arch_keyboard)
      port map (
         CLK => SMCLK,
         RST => RESET,

         DATA_OUT => kbrd_data_out(15 downto 0),
         DATA_VLD => kbrd_data_vld,
         
         KB_KIN   => kin,
         KB_KOUT  => kout
      );
      
	--spracovani klavesnice
keyz: process(RESET, SMCLK)
      variable in_access : std_logic := '0';
   begin
      if SMCLK'event and SMCLK='1' then
      
         if in_access='0' then
            if kbrd_data_vld='1' then 
               in_access:='1';
               if kbrd_data_out(8)='1' then  -- key 3
                  servo0 <= servo0+ x"85";		--setp
						if servo0 >  x"39ce" then		--handle overstep
								servo0 <= x"39ce";
						end if;
               elsif kbrd_data_out(0)='1'  then  -- key 1
                  servo0 <= servo0 - x"85";
						if servo0 <  x"1ce7" then  --handle understep
								servo0 <= x"1ce7";
						end if;
					elsif kbrd_data_out(9)='1' then  -- key 6
                  servo1 <= servo1+ x"85";
						if servo1 >  x"739c" then --handle overstep
								servo1 <= x"739c";
						end if;
               elsif kbrd_data_out(1)='1'  then  -- key 4
                  servo1 <= servo1 - x"85";
						if servo1 <  x"56b5" then
								servo1 <= x"56b5";
						end if;
					elsif kbrd_data_out(10)='1' then  -- key 9
                  servo2 <= servo2+ x"85";
						if servo2 >  x"ad6a" then
								servo2 <= x"ad6a";
						end if;
               elsif kbrd_data_out(2)='1'  then  -- key 7
                  servo2 <= servo2 - x"85";
						if servo2 <  x"9083" then
								servo2 <= x"9083";
						end if;
					elsif kbrd_data_out(11)='1' then  -- key #
                  servo3 <= servo3+ x"85";
						if servo3 >  x"e738" then
								servo3 <= x"e738";
						end if;
               elsif kbrd_data_out(3)='1'  then  -- key *
                  servo3 <= servo3 - x"85";
						if servo3 <  x"ca51" then
								servo3 <= x"ca51";
						end if;
                          
               end if;
            end if;
         else
            if kbrd_data_vld='0' then 
               in_access:='0';
            end if;
         end if;
      end if;
      
   end process;
	
	--
	--comparator
	--nastavenie sirky pulzu
	--e73 .5ms
	--1ce7 1ms
	--39ce 2ms
	--posun mezi kanalmy je 2ms
	buf_next(0) <= '1' when (ref < servo0 ) or (ref = x"0000") else '0';
	buf_next(1) <= '1' when ((ref > x"39ce" ) and (ref < servo1 )) or (ref = x"39ce") else '0';
	buf_next(2) <= '1' when ((ref > x"739c" ) and (ref < servo2 )) or (ref = x"739c") else '0';
	buf_next(3) <= '1' when ((ref > x"ad6a" ) and (ref < servo3 )) or (ref = x"ad6a") else '0';
	
	
	OUTPUT <= buf_reg;
	
end main;
