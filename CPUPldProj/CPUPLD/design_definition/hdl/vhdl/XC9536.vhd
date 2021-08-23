-----------------------------------------------------------------
-- ROM chip:
-- 
--	Plats		Ref schema	Ref layout	IO pinne	I denna kod
-----------------------------------------------------------------
--	Övre		UU5		UU1		P11		ROM1
--	Nedre		UU6		UU2		P2		ROM0
-----------------------------------------------------------------

library IEEE;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity XC9536 is
	port (
		RESET       : in std_logic;
		PSEN        : in std_logic;
		ALE         : in std_logic;
		WE          : in std_logic;
		RE          : in std_logic;
		ADDRESS     : in std_logic_vector(15 downto 12);
		DATA        : inout std_logic_vector(3 downto 0);
		PROGRAM     : in std_logic;
		RAMCE       : out std_logic;
		RAMWE       : out std_logic;
		RAMRE       : out std_logic;
		ROM0CE      : out std_logic;
		ROM1CE      : out std_logic;
		ROMWE       : out std_logic;
		ROMRE       : out std_logic;
		LCDCE       : out std_logic;
		LCDE        : out std_logic;
		LCDRW       : out std_logic;
		DTMFCE      : out std_logic;
		DAWE        : out std_logic;
		FLASH0CE    : out std_logic;
		FLASH1CE    : out std_logic;
		FLASH_RE    : out std_logic;
		FLASH_WE    : out std_logic;
		KEYRE       : out std_logic;
		ROM_SELECT  : in std_logic;	--P46, 0 = ROM0, 1 = ROM1
		PROGROM_OE  : out std_logic;
		PROGROM_CE  : out std_logic	--PROGROM_WE pulled high by pull-up resistor
	);
end XC9536;

--                      A15 A14 A13 A12
--  F000-FFFF = IRAM     1   1   1   1
--  E000-EFFF = FLASHREG 1   1   1   0
--  D000-DFFF = PROGREG  1   1   0   1
--  C000-CFFF = KEYBOARD 1   1   0   0
--  B000-BFFF = FLASH    1   0   1   1
--  A000-AFFF = DA       1   0   1   0
--  9000-9FFF = DTMF     1   0   0   1
--  8000-8FFF = LCD      1   0   0   0

--  0000-7FFF = RAM      0   X   X   X


--      7000-7FFF =          0   1   1   1
--      6000-6FFF =          0   1   1   0
--      5000-5FFF =          0   1   0   1
--      4000-4FFF =          0   1   0   0
--      3000-3FFF =          0   0   1   1
--      2000-2FFF =          0   0   1   0
--      1000-1FFF =          0   0   0   1
--      0000-0FFF =          0   0   0   0



architecture behaviour of XC9536 is
	signal IFLASHCE : std_logic;
	signal IFREGCE : std_logic;
    signal IFLASHSEL : std_logic_vector(1 downto 0);
	signal IFLASHON : std_logic;
	
begin
	LCDRW <= WE;
	LCDE <= not (WE AND RE);

	ROM : process (ADDRESS, PROGRAM, RE, WE, PSEN, RESET, ROM_SELECT)
	begin
		if (RESET = '0') then
			if (PROGRAM = '0') then
	    			ROM0CE   <= 'Z';                  --Normal operation
    				ROM1CE   <= '1';
	    			ROMWE    <= 'Z';
    				ROMRE    <= 'Z';
			else 
		    		ROM0CE   <= '1';                  --Program operation
    				ROM1CE   <= 'Z';
    				ROMWE    <= 'Z';
				ROMRE    <= 'Z';
			end if;
			PROGROM_CE <= 'Z';
			PROGROM_OE <= 'Z';
		else
			if (PROGRAM = '0') then
				ROM1CE <= not ADDRESS(15);						--Normal operation
				ROM0CE <= ADDRESS(15);
				ROMRE <= PSEN;
				ROMWE <= '1';
				PROGROM_CE <= '1';
				PROGROM_OE <= '1';
			else
				-- ROMOCE: WE eller RE låga, ROM_SELECT låg, ADDRESS(15) hög
				-- ROM1CE: WE eller RE låga, ROM_SELECT hög, ADDRESS(15) hög

				ROM1CE <= (WE and RE) or ROM_SELECT or (not ADDRESS(15));	--Program operation
				ROM0CE <= (WE and RE) or (not ROM_SELECT) or (not ADDRESS(15));
				ROMRE <= RE;
				ROMWE <= WE;
				PROGROM_CE <= PSEN;
				PROGROM_OE <= PSEN;
			end if;
		end if;
	end process;
	
	RAM : process (ADDRESS, PROGRAM, RE, WE, RESET)
	begin
		if (RESET = '0') then
	    		RAMCE <= '1';
    			RAMWE <= '1';
    			RAMRE <= '1';
		else
			if (PROGRAM = '0') then
      			RAMCE <= ADDRESS(15);			--Normal operation
				RAMRE <= RE;
				RAMWE <= WE;
			else
	        		RAMCE <= '1';				--Program operation
      	  		RAMWE <= '1';
        			RAMRE <= '1';
			end if;
		end if;
	end process;
	    
    PERIPHERIAL : process (ADDRESS, PROGRAM, WE, RESET)
	begin
		if (RESET = '0') then
    		LCDCE   <= '1';
    		DTMFCE  <= '1';
    		DAWE    <= '1';
            IFLASHCE <= '1';
			IFREGCE <= '1';
			KEYRE <= '1';
		else
			if (PROGRAM = '0') then
                case (ADDRESS(15 downto 12)) is
				when "1000" => 
	            		LCDCE   <= '0';
      	      		DTMFCE  <= '1';
            			DAWE    <= '1';
					IFLASHCE <= '1';
					KEYRE <= '1';
	        			IFREGCE <= '1';
				when "1001" => 
      	      		LCDCE   <= '1';
            			DTMFCE  <= '0';
            			DAWE    <= '1';
                    		IFLASHCE <= '1';
					KEYRE <= '1';
      	  			IFREGCE <= '1';
				when "1010" =>
            			LCDCE   <= '1';
            			DTMFCE  <= '1';
            			DAWE    <= WE;
	                    	IFLASHCE <= '1';
					KEYRE <= '1';
      	  			IFREGCE <= '1';
				when "1011" =>
            			LCDCE   <= '1';
            			DTMFCE  <= '1';
            			DAWE    <= '1';
	                    	IFLASHCE <= '0';
					KEYRE <= '1';
      	  			IFREGCE <= '1';
				when "1100" =>
            			LCDCE   <= '1';
            			DTMFCE  <= '1';
            			DAWE    <= '1';
	                    	IFLASHCE <= '1';
					KEYRE <= '0';
      	  			IFREGCE <= '1';
				when "1110" =>
            			LCDCE   <= '1';
            			DTMFCE  <= '1';
            			DAWE    <= '1';
	                    	IFLASHCE <= '1';
					KEYRE <= '1';
      	  			IFREGCE <= '0';
				when others =>
	            		LCDCE   <= '1';
      	      		DTMFCE  <= '1';
            			DAWE    <= '1';
                  	  	IFLASHCE <= '1';
					KEYRE <= '1';
        				IFREGCE <= '1';
				end case;				    
			else
	        		LCDCE   <= '1';
      	  		DTMFCE  <= '1';
        			DAWE    <= '1';
				IFLASHCE <= '1';
	    			IFREGCE <= '1';
				KEYRE <= '1';
			end if;
		end if;
	end process;
		
    FLASH_WE <= WE when IFLASHCE = '0' else '1';
    FLASH_RE <= RE when IFLASHCE = '0' else '1';

	fregwrite : process(WE, RESET)
	begin
	    if RESET = '0' then 
		    IFLASHSEL <= "00";
		    IFLASHON <= '0';
		elsif rising_edge(WE) then
		    if (IFREGCE = '0') then
		        IFLASHSEL <= DATA(1 downto 0);
		        IFLASHON <= DATA(3);
			end if;
		end if;
	end process;

	DATA(1 downto 0) <= IFLASHSEL when ((RE='0') and (IFREGCE='0')) else "ZZ";
	DATA(2) <= '0' when ((RE='0') and (IFREGCE='0')) else 'Z';
	DATA(3) <= IFLASHON when ((RE='0') and (IFREGCE='0')) else 'Z';

    FLASH : process(IFLASHSEL, IFLASHON)
	begin
	    if (IFLASHON = '1') then
	        case IFLASHSEL is
			when "00" =>
			    FLASH0CE <= '0';
			    FLASH1CE <= '1';
			when "01" =>
			    FLASH0CE <= '1';
			    FLASH1CE <= '0';
			when others =>
			    FLASH0CE <= '1';
			    FLASH1CE <= '1';
			end case;
		else
		    FLASH0CE <= '1';
		    FLASH1CE <= '1';
		end if;
    end process;			
end behaviour;
				
				    
					
					
					    
--	WE	RE	ROM_S	ADDRESS(15)		ROM0	ROM1
--	0	0	0	0			1	1
--	0	0	0	1			0	1
--	0	0	1	0			1	1
--	0	0	1	1			1	0
--	0	1	0	0			1	1
--	0	1	0	1			0	1
--	0	1	1	0			1	1
--	0	1	1	1			1	0
--	1	0	0	0			1	1
--	1	0	0	1			0	1
--	1	0	1	0			1	1
--	1	0	1	1			1	0
--	1	1	0	0			1	1
--	1	1	0	1			1	1
--	1	1	1	0			1	1
--	1	1	1	1			1	1
