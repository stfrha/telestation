library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity KeyboardDecoder is
	port (
	    RESET   : in std_logic;
		CLK     : in std_logic;
		RE      : in std_logic;
		DATA    : out std_logic_vector(4 downto 0);
		INT_KEY : inout std_logic_vector(4 downto 0);
		KEYS    : in std_logic_vector(4 downto 0);
		IRQ_N   : out std_logic
	);
end KeyboardDecoder;

architecture behaviour of KeyboardDecoder is
--    signal INT_KEY : std_logic_vector(4 downto 0);
	signal INT_IRQ : std_logic;
	signal EQU : std_logic;
	signal UNLOCKED : std_logic;
	signal KEYS_D : std_logic_vector(4 downto 0);
begin
    
    EQU <= '1' when (INT_KEY = KEYS_D) else '0';

    IKP : process (RESET, CLK)
	begin
	    if (RESET = '0') then
	        INT_KEY <= (others => '1');
	        KEYS_D <= (others => '1');
	        INT_IRQ <= '1';
			UNLOCKED <= '0';
		elsif rising_edge(CLK) then
		
		    KEYS_D <= KEYS;
		
			if (RE = '0') then 
				INT_IRQ <= '1';
			elsif (INT_IRQ = '1') then
			    if (EQU = '0') then
			        INT_IRQ <= '0';
					UNLOCKED <= '1';
				end if;
			end if;
			
			if (UNLOCKED = '1') then
			    INT_KEY <= KEYS_D;
				UNLOCKED <= '0';
			end if;
		end if;
	end process;
	
	IRQ_N <= INT_IRQ;
	
	DATA <= INT_KEY when RE = '0' else "ZZZZZ";

end behaviour;