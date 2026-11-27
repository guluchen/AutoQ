OPENQASM 3;
include "stdgates.inc";
qubit[3] q;

h q[1];
cx q[1], q[0];
z q[0];
cx q[1], q[0];
