# group01：SAD 專題

本目錄為本組（巴巴boy）的正式作業，題目為 `SAD`（Sum of Absolute Differences）。規定見 [HOMEWORK.md 第五節](../HOMEWORK.md)。

## 小組資訊

| 項目 | 內容 |
|---|---|
| 組名 | 巴巴boy |
| 題目 | `SAD`（絕對差累加，`rd = rd + \|rs1 - rs2\|`） |
| 組長 | 洪震宇（1114405036） |
| 組員 | 羅昊文（1114405048）、古哲宇（1114405007）、林興庭（1114405025） |
| 開發分支 | 每位組員使用 `dev/<學號>` |

## 目錄結構

```
group01/
├── README.md                 本檔
├── CONTRIBUTIONS.md          個人貢獻聲明
├── instructions/
│   ├── README.md
│   └── SAD/
│       └── README.md         指令規格（語意、編碼、測試向量）與參考資料
├── report/
│   ├── README.md
│   ├── CP1.md
│   ├── CP2.md
│   ├── CP3.md
│   └── figures/
│       └── README.md
└── src/
    ├── README.md
    ├── c_baseline/           CP1：C 參考實作、測試、測資
    │   └── README.md
    └── rv32im/               CP2：RV32IM 組合語言與指令統計
        └── README.md
```

## 進度

| Checkpoint | 內容 | 實作 | 報告 |
|---|---|---|---|
| CP1 | C baseline | [`src/c_baseline/`](src/c_baseline/) | [`report/CP1.md`](report/CP1.md)（已完成） |
| CP2 | RV32IM 實作 | [`src/rv32im/`](src/rv32im/) | [`report/CP2.md`](report/CP2.md)（已完成） |
| CP3 | 指令規格與編碼 | [`instructions/SAD/`](instructions/SAD/) | [`report/CP3.md`](report/CP3.md)（已完成） |

## 繳交流程

各組員在 `dev/<學號>` 開發 → 組長在本組倉庫開 PR 並合併到 `main` → 在 `main` 打 tag（`cp1-submit`、`cp2-submit`、`cp3-submit`）。
