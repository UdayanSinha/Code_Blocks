USE work.opc_pack.all;

ENTITY tb_opc_circuit IS END tb_opc_circuit;

ARCHITECTURE test of tb_opc_circuit is
COMPONENT opc_circuit IS
PORT(a, b: IN opc;
    q, qxor, qxnor: OUT opc);
END COMPONENT;

SIGNAL a, b, q, qxor, qxnor: opc;
SIGNAL clk1, clk2: bit;
BEGIN
T1: opc_circuit PORT MAP(a, b, q, qxor, qxnor);
PROCESS
BEGIN
FOR i IN opc5 LOOP
a<=i;
q<=i;
WAIT ON clk1 UNTIL clk1='1';
END LOOP;
END PROCESS;

PROCESS
BEGIN
FOR j IN opc5 LOOP
b<=j;
q<=j;
WAIT ON clk2 UNTIL clk2='1';
END LOOP;
END PROCESS;

clk1<=NOT(clk1) AFTER 10 ns;
clk2<=NOT(clk2) AFTER 50 ns;
END test;