# src/tools/

CP3 的編碼輔助工具，不屬於 CP1／CP2 的執行期程式。

| 檔案 | 用途 |
|---|---|
| [`sad_encoding.py`](sad_encoding.py) | 依 R-type 欄位公式產生 SAD 的三組範例機器碼，並與報告 2.4 節的預期值比對，不符就丟出 `AssertionError` |

## 用法

```text
python sad_encoding.py
```

輸出（與 [`../../report/CP3.md`](../../report/CP3.md) 2.3 節一致）：

```text
SAD x10, x11, x12: 0x00C5850B
  0000000 | 01100 | 01011 | 000 | 01010 | 0001011
SAD x5, x6, x7: 0x0073028B
  0000000 | 00111 | 00110 | 000 | 00101 | 0001011
SAD x15, x16, x17: 0x0118078B
  0000000 | 10001 | 10000 | 000 | 01111 | 0001011
```

執行畫面：[`../../report/figures/sad_encoding_run.png`](../../report/figures/sad_encoding_run.png)

## 注意

- `funct7 = 0000000`、`funct3 = 000` 是**佔位假設值**，教師尚未公布正式值。
  公布後只需修改 `sad_encoding.py` 最上方的 `FUNCT7`、`FUNCT3` 兩個常數，
  並同步更新 `main()` 內的預期值。
- 腳本只驗證編碼位元，不做語意模擬。32-bit wrap 的語意由
  [`../c_baseline/sad.c`](../c_baseline/sad.c) 負責。
