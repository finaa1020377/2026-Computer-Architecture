# SAD 指令規格

## 指令名稱

`SAD`（sum of absolute differences）

## 語意（公式）

```text
rd_new = rd_old + |signed(rs1) - signed(rs2)|
```

`rd_old` 是執行指令前的 `rd` 值。硬體先讀取 `rd_old`、`rs1`、`rs2`，計算完成後才將結果寫回 `rd`。

## 運算元與定義域

- `rs1`、`rs2`：signed 32-bit 二補數整數。
- `rd`：32-bit 累加器，同時是來源與目的。
- 三個暫存器欄位各為 5 bits。

## 結果範圍與溢位處理

絕對差的數學值範圍是 `0` 至 `0xFFFFFFFF`。累加結果保留低 32 bits：

```text
rd_new = (rd_old + abs(signed(rs1) - signed(rs2))) mod 2^32
```

例如 `rs1 = INT32_MAX`、`rs2 = INT32_MIN` 時，絕對差是 `0xFFFFFFFF`。

## 虛擬碼

```text
function SAD(rd_old, rs1, rs2):
    if signed(rs1) >= signed(rs2):
        difference = unsigned(rs1) - unsigned(rs2)
    else:
        difference = unsigned(rs2) - unsigned(rs1)
    return (rd_old + difference) mod 2^32
```

使用 unsigned 減法表示絕對差，避免 `INT32_MIN` 取負值或 signed overflow。

## R-type 編碼

```text
31        25 24    20 19    15 14  12 11     7 6       0
+------------+--------+--------+------+--------+---------+
|   funct7   |  rs2   |  rs1   |funct3|   rd   | opcode  |
+------------+--------+--------+------+--------+---------+
```

- `opcode = 0001011`（`0x0B`，custom-0）。
- `funct7` 與 `funct3` 需使用教師分配值；目前題目資料未提供固定值，因此不能自行指定正式編碼。
- 欄位組合公式：

```text
encoding = (funct7 << 25) | (rs2 << 20) | (rs1 << 15)
         | (funct3 << 12) | (rd << 7) | 0x0B
```

欄位範例（假設值僅供示範，不代表教師正式分配）：

```text
SAD x5, x6, x7
funct7=0000000, rs2=00111, rs1=00110, funct3=000, rd=00101
machine code = 0x0073028B
```

## 測試向量

| `rd_old` | `rs1` | `rs2` | `rd_new` |
|---:|---:|---:|---:|
| 100 | 10 | 13 | 103 |
| 0 | -10 | 13 | 23 |
| 77 | 42 | 42 | 77 |
| 0 | `INT32_MAX` | `INT32_MIN` | `0xFFFFFFFF` |

## RV32IM baseline 對照

對應片段見 [`../rv32im/README.md`](../rv32im/README.md)：

| 實作 | 靜態指令數 | 動態指令數 |
|---|---:|---:|
| RV32IM baseline | 6 | 5 或 6 |
| SAD 自訂指令 | 1 | 1 |

baseline 使用 `slt`、`sub`、`beq`、`add` 完成比較、絕對差與累加。
