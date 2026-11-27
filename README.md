# AutoQ clean ancilla verification

This branch extends [AutoQ](https://github.com/fmlab-iis/AutoQ), a quantum program verifier based on level-synchronized tree automata (LSTAs). It adds wildcard leaf amplitudes in `.lsta` postconditions and a verification mode that matches the same color sequence on both sides.

## Clean ancilla demos

Each example checks that a clean ancilla returns to `|0⟩` after a three-qubit circuit. The precondition fixes the ancilla to zero and includes all four computational basis inputs of the two data qubits. Linearity extends the restoration check to arbitrary data superpositions.

- [First qubit](./demo/ancilla-q0) uses ancilla `q[0]` at LSTA level 1.
- [Middle qubit](./demo/ancilla-q1) uses ancilla `q[1]` at LSTA level 2.
- [Last qubit](./demo/ancilla-q2) uses ancilla `q[2]` at LSTA level 3.

`circuit.qasm` restores the ancilla with a final CNOT. `leak.qasm` omits that gate and leaves nonzero amplitude on an ancilla-`1` branch. The [demo guide](./demo/README.md) lists the amplitude patterns and explains both outcomes.

## Build and run

On Ubuntu, Debian, macOS, or WSL2, install the dependencies and build AutoQ from the repository root.

```bash
./install-dependencies.sh
make
make test
```

Run the two variants of the middle-qubit example from the repository root. The commands print `[OK]` and `[failed]`, respectively.

```bash
case_dir=demo/ancilla-q1
./build/cli/autoq ver --color-inclusion "$case_dir/pre.lsta" "$case_dir/circuit.qasm" "$case_dir/post.lsta"
./build/cli/autoq ver --color-inclusion "$case_dir/pre.lsta" "$case_dir/leak.qasm" "$case_dir/post.lsta"
```

## Wildcards and color sequences

In a transition label `[symbol,C]`, `C` is a bitmask written as a decimal integer. Masks `1` (`0b01`) and `2` (`0b10`) each allow one choice. Mask `3` (`0b11`) allows either choice. Write `3` in the `.lsta` file. The literal `11` would be read as decimal eleven (`0b1011`).

At each level, the verifier takes the bitwise AND of the masks of the transitions used at that level. For example, `3 & 1 = 1`, so those two masks can synchronize on the choice with mask `1`. Masks `1` and `2` share no bit, so they cannot synchronize.

- `[*,C] -> q` matches any complex amplitude, including zero. Each leaf matches independently, even when multiple leaves use the same LSTA state.
- `[0,C] -> q` requires an exact zero amplitude. The demos put this condition on every leaf where the clean ancilla bit is `1`.
- With `--color-inclusion`, each accepting output run and each color sequence it admits must have a matching postcondition run using the same color at every level. Wildcard transitions still take part in color synchronization.

See the [LSTA format](./docs/lsta_description.md), the [HSL format](./docs/hsl_description.md), and the [supported OpenQASM syntax](./docs/qasm_description.md). Run `./build/cli/autoq --help` for the available commands or `./build/cli/autoq ver --help` for verification options.

## Citation
Please cite the following papers if you use this tool for your research.

* <summary>
  <a href="https://dl.acm.org/doi/10.1145/3704868">P. A. Abdulla, Y.-G. Chen, Y.-F. Chen, L. Holík, O. Lengál, J.-A. Lin, F.-Y. Lo, and W.-L. Tsai, “Verifying Quantum Circuits with Level-Synchronized Tree Automata,” in <em>Proceedings of the ACM on Programming Languages 9 (POPL)</em>, 2025, pp. 923–953.</a>
</summary>

```bibtex
@article{10.1145/3704868,
  title = {Verifying Quantum Circuits with Level-Synchronized Tree Automata},
  author = {Abdulla, Parosh Aziz and Chen, Yo-Ga and Chen, Yu-Fang and Hol\'{\i}k, Luk\'{a}\v{s} and Leng\'{a}l, Ond\v{r}ej and Lin, Jyun-Ao and Lo, Fang-Yi and Tsai, Wei-Lun},
  journal = {Proceedings of the ACM on Programming Languages},
  volume = {9},
  number = {POPL},
  year = {2025},
  pages = {923--953},
  doi = {10.1145/3704868}
}
```

* <summary>
  <a href="https://link.springer.com/chapter/10.1007/978-3-031-90660-2_5">Y.-F. Chen, K.-M. Chung, M.-H. Hsieh, W.-J. Huang, O. Lengál, J.-A. Lin, and W.-L. Tsai, “AutoQ 2.0: From Verification of Quantum Circuits to Verification of Quantum Programs,” in <em>International Conference on Tools and Algorithms for the Construction and Analysis of Systems (TACAS)</em>, 2025, pp. 87–108</a>
</summary>

```bibtex
@InProceedings{10.1007/978-3-031-90660-2_5,
  author = {Chen, Yu-Fang and Chung, Kai-Min and Hsieh, Min-Hsiu and Huang, Wei-Jia and Leng{\'a}l, Ond{\v{r}}ej and Lin, Jyun-Ao and Tsai, Wei-Lun},
  booktitle = {International Conference on Tools and Algorithms for the Construction and Analysis of Systems (TACAS)},
  title = {AutoQ 2.0: From Verification of Quantum Circuits to Verification of Quantum Programs},
  year = {2025},
  pages = {87--108},
  doi = {10.1007/978-3-031-90660-2_5}
}
```

## License

AutoQ is released under the [MIT License](./LICENSE).
