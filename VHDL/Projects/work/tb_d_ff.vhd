LIBRARY IEEE; -- These lines informs the compiler that the library IEEE is used
USE IEEE.std_logic_1164.all; -- contains the definition for the std_logic type plus some useful conversion functions

ENTITY tb_d_ff IS END tb_d_ff;

ARCHITECTURE test OF tb_d_ff IS
COMPONENT d_ff IS
PORT(d, clk: IN STD_LOGIC;
    q: OUT STD_LOGIC);
END COMPONENT;

SIGNAL d, clk, q: STD_LOGIC;

BEGIN
    T1: d_ff PORT MAP(d, clk, q);
    d<='1',
       '0' AFTER 7.5 ns,
       '1' AFTER 15 ns,
       '0' AFTER 22.5 ns,
       '1' AFTER 30 ns,
       '0' AFTER 37.5 ns;
    clk<='0',
         '1' AFTER 5 ns,
         '0' AFTER 10 ns,
         '1' AFTER 15 ns,
         '0' AFTER 20 ns,
	 '1' AFTER 25 ns,
         '0' AFTER 30 ns,
         '1' AFTER 35 ns,
         '0' AFTER 40 ns,
	 '0' AFTER 45 ns;
end test;