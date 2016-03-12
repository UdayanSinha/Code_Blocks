LIBRARY IEEE; -- These lines informs the compiler that the library IEEE is used
USE IEEE.std_logic_1164.all; -- contains the definition for the std_logic type plus some useful conversion functions

ENTITY tb_manchester_encode IS END tb_manchester_encode;

ARCHITECTURE test OF tb_manchester_encode IS
COMPONENT manchester_encode IS
PORT(input, clk: IN STD_LOGIC;
    man_out: OUT STD_LOGIC);
END COMPONENT;

SIGNAL input, clk: STD_LOGIC:='0';
SIGNAL man_out: STD_LOGIC;

BEGIN
T1: manchester_encode PORT MAP(input, clk, man_out);
input<='0',
       '1' AFTER 15 ns,
       '0' AFTER 25 ns,
       '1' AFTER 35 ns,
       '0' AFTER 45 ns,
       '1' AFTER 55 ns,
       '0' AFTER 65 ns,
       '1' AFTER 75 ns,
       '0' AFTER 85 ns;

clk<=NOT(clk) AFTER 2.5 ns;
END test;