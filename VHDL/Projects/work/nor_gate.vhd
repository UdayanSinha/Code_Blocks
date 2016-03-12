LIBRARY IEEE; -- These lines informs the compiler that the library IEEE is used
USE IEEE.std_logic_1164.all; -- contains the definition for the std_logic type plus some useful conversion functions

ENTITY nor_gate IS
  PORT (a:IN STD_LOGIC;
  	b:IN STD_LOGIC;
    	q:OUT STD_LOGIC);
END nor_gate;

ARCHITECTURE behave OF nor_gate IS
BEGIN
  q<=a NOR b;
END behave;