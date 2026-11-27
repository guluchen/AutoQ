# AutoQ clean ancilla 測試案例

這三個案例用同一個三量子位元電路模板，分別把 clean ancilla 放在第一、中間和最後一個位置。每個 `pre.lsta` 都包含其餘兩個 data qubit 的四種計算基底輸入，並把 ancilla 初始化為 `|0⟩`。只要這四種輸入的輸出都落在 ancilla 為 `|0⟩` 的子空間，由線性可知任意 data 疊加態也滿足同一條件。

## 三個位置

以下的葉振幅順序固定為 `000, 001, 010, 011, 100, 101, 110, 111`，由左到右對應 `q[0]、q[1]、q[2]`。OpenQASM 的 qubit 編號從 `0` 開始，LSTA 的層標籤從 `1` 開始。

- [ancilla-q0](./ancilla-q0) 把 ancilla 放在 `q[0]`（LSTA 第 `1` 層），以 `q[1]` 控制 CNOT。葉振幅規格是 `(*, *, *, *, 0, 0, 0, 0)`。
- [ancilla-q1](./ancilla-q1) 把 ancilla 放在 `q[1]`（LSTA 第 `2` 層），以 `q[2]` 控制 CNOT。葉振幅規格是 `(*, *, 0, 0, *, *, 0, 0)`。
- [ancilla-q2](./ancilla-q2) 把 ancilla 放在 `q[2]`（LSTA 第 `3` 層），以 `q[0]` 控制 CNOT。葉振幅規格是 `(*, 0, *, 0, *, 0, *, 0)`。

## 電路與 postcondition

`circuit.qasm` 先對控制用的 data qubit 做 `H`，再依序做 `CX(data, ancilla)`、`Z(ancilla)`、`CX(data, ancilla)`。最後一道 CNOT 還原 ancilla。`leak.qasm` 少了這一道，會在 ancilla 等於 `1` 的分支留下非零振幅。

例如，中間位置的案例從 `|000⟩` 出發，`circuit.qasm` 輸出 `(|000⟩ − |001⟩)/√2`，而 `leak.qasm` 輸出 `(|000⟩ − |011⟩)/√2`。`|011⟩` 的中間位元是 `1`，所以第二個輸出會被拒絕。

在 [中間位置的 post.lsta](./ancilla-q1/post.lsta)，`[2,1](f3,z3) -> f2` 把 ancilla 的 `0`、`1` 分支分別接到 `f3` 和 `z3`。`f3` 最後接到 `[*,1] -> f4`，讓每個允許的葉各自匹配任意複數振幅。`z3` 下方都接到 `[0,1] -> z4`，要求振幅恰好為 `0`。

`--color-inclusion` 也要求 postcondition 逐層使用和電路輸出相同的 color。這些案例在 data 層用 mask `3` 允許位元 `1` 和 `2`，因此 wildcard 只放寬振幅，葉轉移的 color 仍然參與檢查。

## 執行方式

以下指令從儲存庫根目錄依序驗證三個位置。

```bash
for q in 0 1 2; do
    case_dir="demo/ancilla-q$q"
    ./build/cli/autoq ver --color-inclusion "$case_dir/pre.lsta" "$case_dir/circuit.qasm" "$case_dir/post.lsta"
    ./build/cli/autoq ver --color-inclusion "$case_dir/pre.lsta" "$case_dir/leak.qasm" "$case_dir/post.lsta"
done
```

每組的 `circuit.qasm` 應顯示 `[OK]`，`leak.qasm` 應顯示 `[failed]`。AutoQ 會在輸出中顯示判定結果，這兩種正常完成的驗證都可能回傳 exit code `0`。
