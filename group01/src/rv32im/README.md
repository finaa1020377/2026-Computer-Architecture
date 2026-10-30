# SAD RV32IM baseline

本資料夾提供不使用 SAD 自訂指令時，以 RV32IM 基本指令實作相同功能的參考片段。

## 呼叫介面

- `a0`：輸入累加器 `rd_old`，也是結果
- `a1`：`rs1`
- `a2`：`rs2`
- `a0`：輸出 `rd_new`
- 輸入值以 signed 32-bit 二補數解讀；減法、絕對值與累加都保留低 32 bits

## RV32IM 組合語言

```asm
# a0 = a0 + abs(a1 - a2)
# Uses t1, t2; no memory access, no pseudo-instruction, no branch.
sub   t1, a1, a2     # t1 = a1 - a2 (mod 2^32)
srai  t2, t1, 31     # t2 = 0 if t1 >= 0, 0xFFFFFFFF if t1 < 0
xor   t1, t1, t2     # t1 = t1 or ~t1, depending on the sign
sub   t1, t1, t2     # t1 = |a1 - a2| (mod 2^32)
add   a0, a0, t1     # accumulator + absolute difference
```

這段程式只使用 RV32I 指令；RV32I 是 RV32IM 的子集，因此符合 RV32IM 限制。
`srai` 產生的遮罩讓 `(d ^ m) − m` 同時處理 `rs1 ≥ rs2` 與 `rs1 < rs2` 兩種情況，
所以不需要比較與分支，也自然符合「減法與絕對值均以 32-bit wrap 計算」的規格。

可組譯的完整版本見 [`sad.S`](sad.S)：上面 5 條指令原樣放在 `sad_accumulate` 內，
最後多一條 `jalr zero, 0(ra)`（等同 `ret`）作為函式返回。這條屬於呼叫成本，
不計入下表的核心指令統計。

可組譯的完整版本見 [`sad.S`](sad.S)：上面 6 條指令原樣放在 `sad_accumulate` 內，最後多一條 `jalr zero, 0(ra)`（等同 `ret`）作為函式返回。這條屬於呼叫成本，不計入下表的核心指令統計。

## 指令統計

| 項目 | 數量 |
|---|---:|
| 靜態指令數 | 5 |
| 動態指令數（無分支，每次執行相同） | 5 |
| 分支指令數（靜態） | 0 |
| 分支指令數（動態） | 0 |

統計以列出的真實指令為準，不將註解或標籤計入；程式沒有分支或迴圈，
因此每次執行的動態指令數固定為 5。
