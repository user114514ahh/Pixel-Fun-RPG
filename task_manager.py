import pygame
import sys
import os

# 強制讓 Pygame 視窗在螢幕正中央開啟
os.environ['SDL_VIDEO_CENTERED'] = '1'

def run_minigame(task_id):
    pygame.init()
    
    # 設定視窗大小為 680x480
    screen = pygame.display.set_mode((680, 480))
    pygame.display.set_caption(f"Education Task - Level {task_id + 1}")
    
    # 設定字型
    font_large = pygame.font.Font(None, 64)
    font_small = pygame.font.Font(None, 36)
    
    # 根據 NPC 編號 (task_id) 決定題目
    if task_id == 0:
        question = "What is 15 + 27 ?"
        answer = "42"
    elif task_id == 1:
        question = "Sequence: 2, 4, 8, 16, ?"
        answer = "32"
    elif task_id == 2:
        question = "3 apples + 5 apples = ?"
        answer = "8"
    else:
        question = "Unknown Task"
        answer = "0"

    user_input = ""
    running = True
    
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                # 玩家點擊右上角 X 關閉視窗 -> 任務取消 (回傳 1)
                pygame.quit()
                sys.exit(1)
                
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    # 按下 Enter 檢查答案
                    if user_input == answer:
                        pygame.quit()
                        sys.exit(0) # 答對了 -> 任務成功 (回傳 0)
                    else:
                        user_input = "" # 答錯清空，讓玩家重打
                elif event.key == pygame.K_BACKSPACE:
                    user_input = user_input[:-1]
                else:
                    # 限制只能輸入數字
                    if event.unicode.isnumeric():
                        user_input += event.unicode

        # 繪製背景 (深藍色)
        screen.fill((30, 40, 60))
        
        # 繪製提示文字
        hint_text = font_small.render("Type the answer and press ENTER", True, (150, 150, 150))
        screen.blit(hint_text, (140, 50))
        
        # 繪製題目
        q_text = font_large.render(question, True, (255, 255, 255))
        screen.blit(q_text, (100, 150))
        
        # 繪製玩家輸入框
        input_box = pygame.Rect(100, 250, 480, 60)
        pygame.draw.rect(screen, (200, 200, 200), input_box)
        
        # 繪製玩家輸入的文字
        ans_text = font_large.render(user_input, True, (0, 0, 0))
        screen.blit(ans_text, (110, 260))
        
        pygame.display.flip()

if __name__ == "__main__":
    # 接收來自 C 程式的參數 (NPC 編號)
    if len(sys.argv) > 1:
        try:
            task_id = int(sys.argv[1])
            run_minigame(task_id)
        except ValueError:
            sys.exit(1)
    else:
        sys.exit(1)