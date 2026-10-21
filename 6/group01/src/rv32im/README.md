# SAD RV32IM baseline

本資料夾提供不使用 SAD 自訂指令時，以 RV32IM 基本指令實作相同功能的參考片段。

## 實作檔案

- `sad_rv32im.S`：可組譯的 RV32IM baseline 函式，函式名為 `sad_accumulate_rv32im`

## 呼叫介面

- `a0`：輸入累加器 `rd_old`，也是結果
- `a1`：`rs1`
- `a2`：`rs2`
- `a0`：輸出 `rd_new`
- 輸入值以 signed 32-bit 二補數解讀，結果保留低 32 bits

## RV32IM 組合語言

```asm
# a0 = a0 + abs(a1 - a2)
# Uses t0..t2; no memory access and no pseudo-instruction.
slt   t0, a1, a2       # t0 = 1 when a1 < a2
sub   t1, a1, a2       # a1 - a2
sub   t2, a2, a1       # a2 - a1
beq   t0, zero, use_t1 # choose a1-a2 when a1 >= a2
add   t1, t2, zero      # t1 = a2-a1
use_t1:
add   a0, a0, t1       # accumulator + absolute difference
```

這段程式只使用 RV32I 指令；RV32I 是 RV32IM 的子集，因此符合 RV32IM 限制。

可直接使用 `sad_rv32im.S` 中的函式；其核心指令流程與上方片段一致。

## 指令統計

| 項目 | 數量 |
|---|---:|
| 靜態指令數 | 6 |
| 動態指令數（`a1 < a2`） | 6 |
| 動態指令數（`a1 >= a2`） | 5 |
| 分支指令數（靜態） | 1 |
| 分支指令數（動態） | 1 |

統計以列出的真實指令為準，不將註解或標籤計入；`add t1, t2, zero` 是實際 ADD 指令，不是 pseudo-instruction。
