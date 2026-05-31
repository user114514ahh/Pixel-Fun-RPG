# Pixel-Fun-RPG

這是一個結合了經典像素風格 RPG 引擎與 Python 互動任務系統的教育型遊戲專案。本專案旨在透過 C 語言實現 RPG 核心邏輯，並利用 Python 執行教育主題的小遊戲，降低系統互動複雜度，提升教育內容的彈性。

## Demo

[https://user-images.githubusercontent.com/83382087/240593118-c6c66916-ae01-4e73-a2ca-e79132c9c7b1.mp4](https://private-user-images.githubusercontent.com/124040450/600610892-ebea8f3b-0c18-4597-83d0-0bd81a6656c6.mp4?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3ODAyNDEwODIsIm5iZiI6MTc4MDI0MDc4MiwicGF0aCI6Ii8xMjQwNDA0NTAvNjAwNjEwODkyLWViZWE4ZjNiLTBjMTgtNDU5Ny04M2QwLTBiZDgxYTY2NTZjNi5tcDQ_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjYwNTMxJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI2MDUzMVQxNTE5NDJaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT05MDQ2NzM3MDU4YTczOGQ0MTA4YTRjODk2OTA2OWFlYmY5MmRkNGQ4YjlhOGE4NGNiZWNmZjYxMzk3YjY1YWI5JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZyZXNwb25zZS1jb250ZW50LXR5cGU9dmlkZW8lMkZtcDQifQ.0uGb5NaCvc3Rky-PbpdDQfbWb2u2NnPTzBLzOMjCXuQ)

## 系統架構

- **RPG 引擎**: 使用 C 語言與 `raylib` 函式庫開發。
- **任務系統**: 由 C 程式呼叫 Python 腳本 (`task_manager.py`) 執行互動任務，並根據回傳結果進行遊戲內的後續邏輯判斷。
- **開發環境**: 專為 Ubuntu 24.04 設計，透過 Shell 腳本自動化建置。

## 專案結構

```apl
.
├── assets/             # 存放遊戲貼圖 (PNG)
├── ground.txt          # 地圖地面層資料
├── object.txt          # 地圖物件層資料
├── main.c              # RPG 核心程式碼
├── task_manager.py     # Python 任務管理器
├── run.sh              # 一鍵建置與執行腳本
└── .gitignore          # Git 忽略檔案設定
```

## 環境需求與執行步驟

本專案已自動化環境建置流程，請依照以下步驟在 **Ubuntu 24.04** 上執行：

1. **下載專案**:

	Bash

	```bash
	git clone https://github.com/user114514ahh/Pixel-Fun-RPG.git
	cd Pixel-Fun-RPG
	```

2. **執行啟動腳本**:

	給予執行權限並執行：

	Bash

	```bash
	chmod +x run.sh
	./run.sh
	```

	*此腳本會自動執行以下任務：*

	- 安裝必要的系統依賴套件 (build-essential, Python 等)。
	- 下載並編譯 `raylib`。
	- 編譯 `main.c` 成為執行檔 `game`。
	- 建立並設定 Python 虛擬環境。

3. **開始遊戲**:

	執行腳本後，遊戲視窗將自動開啟。

	- **WASD**: 移動角色。
	- **E**: 與 NPC 對話。
	- **ENTER**: 在對話框中確認並進入任務（觸發 Python 小遊戲）。

## 遊戲功能亮點

- **跨環境協作**: 透過 `.gitattributes` 與正確的編譯架構，完美解決跨平台開發問題。
- **視覺效果**: 支援 1080x720 解析度，並內建黑色漸層淡入淡出效果（Fade Effect）。
- **攝影機機制**: 具備攝影機邊界鎖定功能，確保視角不會超出地圖範圍。

## 開發者與貢獻

- 本專案由林以軒進行核心架構設計與開發，使用C。
