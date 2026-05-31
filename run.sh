#!/bin/bash

echo "========================================"
echo "  Pixel RPG - Ubuntu 24.04 環境建置與啟動"
echo "========================================"

# 1. 安裝必要的系統套件 (C編譯器、圖形庫依賴、Python虛擬環境工具)
echo "[1/5] 檢查並安裝系統依賴套件 (可能需要輸入 sudo 密碼)..."
sudo apt-get update
sudo apt-get install -y build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev python3 python3-venv

# 2. 下載並編譯 Linux 版的 Raylib
echo "[2/5] 準備 Raylib 函式庫..."
if [ ! -d "raylib" ]; then
    echo "正在從 GitHub 下載 Raylib..."
    git clone https://github.com/raysan5/raylib.git
    cd raylib/src
    make PLATFORM=PLATFORM_DESKTOP
    cd ../..
else
    echo "Raylib 已存在，跳過下載。"
fi

# 3. 編譯你的 C 遊戲主程式
echo "[3/5] 編譯 C 語言主程式..."
gcc main.c -o game -I raylib/src -L raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
if [ $? -ne 0 ]; then
    echo "❌ 編譯失敗！請檢查 main.c 的程式碼。"
    exit 1
fi

# 4. 建立 Python 虛擬環境並安裝 Pygame (符合 Ubuntu 24.04 PEP 668 規範)
echo "[4/5] 設定 Python 虛擬環境與 Pygame..."
if [ ! -d "venv" ]; then
    python3 -m venv venv
fi
# 啟動虛擬環境
source venv/bin/activate
# 在虛擬環境中安裝 pygame-ce
pip install pygame-ce

# 5. 執行遊戲
echo "[5/5] 環境建置完成！啟動遊戲..."
echo "========================================"
./game

# 遊戲關閉後，退出虛擬環境
deactivate