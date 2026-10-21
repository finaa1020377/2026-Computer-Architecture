# CP3：指令規格與編碼 — SAD

> 骨架，內容待組員填寫。刪除所有「> 填寫提示」與 `TODO` 後再繳交。

## 1. 本次目標

> 填寫提示：本 CP 要完成的項目，一句話（約 3 行）。

TODO

## 2. 設計內容

> 填寫提示：語意規格、編碼、指令數比較。規格格式見 HOMEWORK 6.7。
>
> 必附：
> - 虛擬碼
> - 3 個範例（含 1 個邊界值）
> - 機器碼與欄位說明
> - 指令數比較表
>
> 對應文件：[`instructions/SAD/`](../instructions/SAD/)

### 2.1 語意規格

- 語意（公式）：TODO
- 運算元與定義域：TODO
- 結果範圍：TODO
- 邊界條件與溢位處理：TODO
- 測試向量（至少 3 組，含 1 組邊界值）：TODO
- 對應的 RV32IM baseline 片段與指令數：TODO

### 2.2 虛擬碼

TODO

### 2.3 編碼

> 填寫提示：R-type，`opcode = 0001011`（0x0B）。`funct3`、`funct7` 由教師分配，不可自行更改；尚未取得正式值時，需標明範例中的值是假設值。

TODO

### 2.4 範例（含 1 個邊界值）

| 範例 | `rd_old` | `rs1` | `rs2` | `rd_new` | 機器碼與欄位說明 |
|---|---:|---:|---:|---:|---|
| 1 | TODO | TODO | TODO | TODO | TODO |
| 2 | TODO | TODO | TODO | TODO | TODO |
| 3（邊界值） | TODO | TODO | TODO | TODO | TODO |

### 2.5 關鍵決策的 5 Whys

| 層 | 為什麼？ | 回答（可驗證） |
|---|---|---|
| Why 1 | TODO | TODO |
| Why 2 | TODO | TODO |
| Why 3 | TODO | TODO |
| Why 4 | TODO | TODO |
| Why 5 | TODO | TODO |

## 3. 結果與分析

> 填寫提示：指令數比較表。必須標示計數方法（**靜態或動態**）；若使用模擬器，註明工具與版本。本次不要求 cycle 數，pipeline 尚未教授。

| 實作 | 計數方法 | 指令數 |
|---|---|---:|
| RV32IM baseline | TODO | TODO |
| SAD 自訂指令 | TODO | TODO |

TODO（分析與假設）

## 4. 遭遇的問題與解法

> 填寫提示：至少一個實際遇到的問題。

TODO

## 5. 白話說明

> 填寫提示：用非術語的方式解釋本 CP 的核心概念，200 字以內。

TODO

## 6. AI 使用說明

> 填寫提示（HOMEWORK 7.3）：工具名稱、使用時機與用途、哪些內容由本組修改及修改之處。

TODO
