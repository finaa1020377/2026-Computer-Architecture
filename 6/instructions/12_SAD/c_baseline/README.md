# SAD C baseline

本資料夾提供 SAD 自訂指令的 CP1 C 參考實作與可執行測試。

## 語意

```text
rd_new = rd_old + |signed(rs1) - signed(rs2)|
```

- `rd_old` 是執行前的累加器值，`rd_new` 是寫回同一個 `rd` 的值。
- `rs1` 與 `rs2` 解讀為 signed 32-bit 二補數。
- 累加結果保留低 32 bits（wrap-around）。
- 先讀取 `rd_old`、`rs1`、`rs2`，完成計算後才寫回 `rd`。

## 編譯與執行

```text
cc -std=c11 -Wall -Wextra -Werror sad.c test_sad.c -o test_sad
./test_sad
```

成功時輸出：

```text
12 tests passed.
```
