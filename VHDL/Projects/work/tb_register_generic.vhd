LIBRARY IEEE; -- These lines informs the compiler that the library IEEE is used
USE IEEE.std_logic_1164.all; -- contains the definition for the std_logic type plus some useful conversion functions

ENTITY tb_register_generic IS END tb_register_generic;

ARCHITECTURE test OF tb_register_generic IS
CONSTANT size: INTEGER:= 8;
COMPONENT register_generic IS
GENERIC(size: INTEGER);
PORT(d: IN STD_LOGIC_VECTOR(size-1 DOWNTO 0); 
    clk, rst: IN STD_LOGIC;
    q: OUT STD_LOGIC_VECTOR(size-1 DOWNTO 0));
END COMPONENT;

SIGNAL d, q: STD_LOGIC_VECTOR(size-1 DOWNTO 0);
SIGNAL clk, rst: STD_LOGIC;

BEGIN
    T1: register_generic GENERIC MAP(size) PORT MAP(d, clk, rst, q);
    d<="11111111",
       "00001111" AFTER 5 ns,
       "10101010" AFTER 15 ns,
       "11110000" AFTER 25 ns,
       "01010101" AFTER 35 ns,
       "11001100" AFTER 40 ns;
    clk<='0',
         '1' AFTER 10 ns,
         '0' AFTER 20 ns,
         '1' AFTER 30 ns,
         '0' AFTER 40 ns,
	 '1' AFTER 50 ns;
    rst<='0',
	 '1' AFTER 45 ns;
end test;