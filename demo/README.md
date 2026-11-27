# AutoQ clean ancilla demos

The three cases use the same circuit pattern on three qubits, with the clean ancilla in the first, middle, or last position. Each `pre.lsta` includes all four computational basis states of the two data qubits and initializes the ancilla to `|0⟩`. If the circuit returns the ancilla to `|0⟩` for all four inputs, linearity gives the same guarantee for any superposition of the data qubits.

## Ancilla positions

The amplitude patterns below follow the basis order `000, 001, 010, 011, 100, 101, 110, 111`. The bits correspond to `q[0]`, `q[1]`, and `q[2]` from left to right. OpenQASM indices start at 0 and LSTA levels start at 1.

- [ancilla-q0](./ancilla-q0) places the ancilla at `q[0]` (LSTA level 1) and uses `q[1]` as the CNOT control. The pattern is `(*, *, *, *, 0, 0, 0, 0)`.
- [ancilla-q1](./ancilla-q1) places the ancilla at `q[1]` (LSTA level 2) and uses `q[2]` as the CNOT control. The pattern is `(*, *, 0, 0, *, *, 0, 0)`.
- [ancilla-q2](./ancilla-q2) places the ancilla at `q[2]` (LSTA level 3) and uses `q[0]` as the CNOT control. The pattern is `(*, 0, *, 0, *, 0, *, 0)`.

## Circuit and postcondition

`circuit.qasm` applies `H` to the control qubit, then applies `CX(data, ancilla)`, `Z(ancilla)`, and `CX(data, ancilla)`. The last CNOT uncomputes the ancilla. `leak.qasm` omits that gate and leaves a nonzero amplitude on a branch where the ancilla bit is 1.

For the middle position, starting from `|000⟩`, `circuit.qasm` produces `(|000⟩ − |001⟩)/√2`, and `leak.qasm` produces `(|000⟩ − |011⟩)/√2`. The middle bit of `|011⟩` is 1, so the second output fails the check.

In the [middle case's postcondition](./ancilla-q1/post.lsta), `[2,1](f3,z3) -> f2` sends the ancilla's 0 branch to `f3` and its 1 branch to `z3`. The former reaches `[*,1] -> f4`, where each leaf can independently match any complex amplitude. Every leaf under `z3` reaches `[0,1] -> z4` and requires an exact zero.

The color field is a bitmask written in decimal. The data layers use `3`, which is `0b11` and permits either choice `1` (`0b01`) or `2` (`0b10`). With `--color-inclusion`, the postcondition must use the same chosen bit as the circuit output at each level. Wildcard leaves still participate in this check through their declared masks.

## Run the demos

From the repository root, run both circuits for each ancilla position.

```bash
for q in 0 1 2; do
    case_dir="demo/ancilla-q$q"
    ./build/cli/autoq ver --color-inclusion "$case_dir/pre.lsta" "$case_dir/circuit.qasm" "$case_dir/post.lsta"
    ./build/cli/autoq ver --color-inclusion "$case_dir/pre.lsta" "$case_dir/leak.qasm" "$case_dir/post.lsta"
done
```

For all three positions, `circuit.qasm` prints `[OK]` and `leak.qasm` prints `[failed]`. Check the printed verdict because both completed verifications return exit status 0.
