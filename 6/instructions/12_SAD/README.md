# SAD：Sum of Absolute Differences

## 題目資訊

| 項目 | 內容 |
|---|---|
| 指令名稱 | `SAD` |
| 題目類別 | AI / 視覺類 |
| 運算元類型 | 二元（累加） |
| 輸入定義 | `rd` 為累加器，同時是來源與目的 |
| 語意公式 | `rd_new = rd_old + \|signed(rs1) - signed(rs2)\|` |
| 溢位處理 | 結果保留低 32 bits（modulo `2^32`） |

SAD 用來計算兩個 signed 32-bit 數值的絕對差，並加到 `rd` 原本的累加值。執行時必須先讀取 `rd_old`、`rs1` 與 `rs2`，完成計算後再將結果寫回 `rd`。

正式語意依課程規格：[HOMEWORK.md 第六節](../../HOMEWORK.md#六題目語意規格)。

## 小組資訊

| 項目 | 內容 |
|---|---|
| 組名 | 巴巴boy |
| 組長 | 洪震宇（1114405036） |
| 組員 | 羅昊文（1114405048）、古哲宇（1114405007）、林興庭（1114405025） |
| 開發分支 | `dev/1114405036`、`dev/1114405048`、`dev/1114405007`、`dev/1114405025` |

## 實作內容

| Checkpoint | 內容 | 位置 |
|---|---|---|
| CP1 | C 參考實作與測試 | [`c_baseline/`](./c_baseline/) |
| CP2 | RV32IM baseline 與指令統計 | [`rv32im/`](./rv32im/) |
| CP3 | SAD 語意、虛擬碼與 R-type 編碼 | [`spec/`](./spec/) |
| CP1–CP3 | 測試向量與預期輸出 | [`tests/`](./tests/) |
| 參考資料 | RISC-V ISA 與 SAD 演算法來源 | [`references/`](./references/) |
| 個人貢獻 | 組員分工與貢獻比例 | [`CONTRIBUTIONS.md`](./CONTRIBUTIONS.md) |

## 測試

CP1 的 C baseline 包含 12 組測試，涵蓋：

- 一般絕對差與反向運算元
- 負數與正數
- `rs1 == rs2` 的零差值
- `INT32_MIN`、`INT32_MAX` 邊界
- 最大 signed 絕對差
- 32-bit 累加 wrap-around

進入 [`c_baseline/`](./c_baseline/) 後執行：

```text
cc -std=c11 -Wall -Wextra -Werror sad.c test_sad.c -o test_sad
./test_sad
```

成功時應輸出：

```text
12 tests passed.
```
