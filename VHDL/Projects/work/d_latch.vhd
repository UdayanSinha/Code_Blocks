LIBRARY IEEE; -- These lines informs the compiler that the library IEEE is used
USE IEEE.std_logic_1164.all; -- contains the definition for the std_logic type plus some useful conversion functions

ENTITY d_latch IS
PORT(d, clk: IN STD_LOGIC;
    q: OUT STD_LOGIC);
END d_latch;

ARCHITECTURE behave OF d_latch IS
BEGIN

PROCESS(clk, d)
BEGIN
    IF (clk='1') THEN
	 q<=d;
    END IF;
END PROCESS;
END behave;