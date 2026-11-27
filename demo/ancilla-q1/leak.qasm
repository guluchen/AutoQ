OPENQASM 3;
include "stdgates.inc";
qubit[3] q;

h q[2];
cx q[2], q[1];
z q[1];
