# SAD 測試

測試程式位於 [`c_baseline/test_sad.c`](../c_baseline/test_sad.c)，會呼叫 [`sad_accumulate`](../c_baseline/sad.c) 並逐筆比對預期結果。

## 執行

```text
cc -std=c11 -Wall -Wextra -Werror ../c_baseline/sad.c ../c_baseline/test_sad.c -o test_sad
./test_sad
```

成功時應輸出：

```text
12 tests passed.
```

測試向量也整理在 [`test_vectors.csv`](test_vectors.csv)。
