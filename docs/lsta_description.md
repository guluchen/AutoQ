## NFTA Format `*.lsta`

Since the underlying structure of a set of quantum states is still an NFTA in AutoQ, we reserve the `*.lsta` format for users to describe a set of quantum states with an NFTA. The *Constants* and *Constraints* sections remain, but the *Extended Dirac* section should be replaced with two new sections *Root States* and *Transitions* now. (Unary) states in an NFTA can be arbitrary strings (no need to enclose with double quotation marks).

### # Root States
This section is responsible for specifying a set of root states. It should contain only one line starting with "Root States" and ending with a set of root states separated by whitespaces.

### # Transitions
This section is responsible for specifying a set of transitions. One transition per line. Notice that the extension name `*.lsta` stands for Level-Synchronized (Nondeterministic Finite) Tree Automata which means that each transition is associated with a nonnegative integer denoting a set of choices in AutoQ. A (bottom-up) transition $f(q_1, q_2, ..., q_n) \to q$ associated with a set of choices $C$ is written as `[f,C](q1, q2, ..., qn) -> q`. Each pair of transitions `[f1,C1](...) -> q` and `[f2,C2](...) -> q` with the same `q` must satisfy $C_1 \\& C_2 = 0$. In each layer of all used transitions for an accepted quantum state, the bitwise $\\&$ of all used $C$'s must be positive. An internal symbol is a positive integer $i$ with arity $2$ for the $i$-th qubit. A leaf symbol has arity $0$ and usually gives the complex amplitude of a computational basis state.

In a `.lsta` postcondition for `autoq ver`, `[*,C] -> q` matches any complex amplitude, including zero. Each occurrence matches independently, even if several leaves use the same state `q`. The wildcard transition still uses its choice set `C` for level synchronization. Numeric and symbolic amplitudes elsewhere in the same pattern still use AutoQ's usual scaled comparison.

With `autoq ver --color-inclusion`, every accepting color sequence in the circuit output must also match in the postcondition. At each level both runs use the same choice bit. If the output admits mask `3` at a level, the verifier checks the two choices represented by masks `1` and `2` separately. Wildcard leaves still use their declared color masks, and the zero vector must match the supplied postcondition under its color sequence.

For example, with data qubit 1 and clean ancilla qubit 2, this postcondition allows arbitrary amplitudes at `|00>` and `|10>` and requires zero at `|01>` and `|11>`.

```
Root States root
Transitions
[1,1](data,data) -> root
[2,1](any,zero) -> data
[*,1] -> any
[0,1] -> zero
```

We close this section with the following example.
```
Constants
c0 := 0
Root States aR bR
Transitions
[1,1](aL1, aL1) -> aR
[2,1](qLow, q0) -> aL1
[1,1](bL1, bL1) -> bR
[2,1](q0, qHigh) -> bL1
[c0,1] -> q0
[p1,1] -> qLow
[p2,1] -> qHigh
Constraints
imag(p1) = 0
p1 ^ 2 < 1/8
imag(p2) = 0
p2 ^ 2 > 7/8
```
