
import cv2
import random
import matplotlib.pyplot as plt
import pyautogui
import numpy as np 

#cur = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]
start1 = ['1', '2', '3', '4', '5', '6', '7', '8', '0']
start2 = random.sample(start1, 9)
cur = [ start2[0:3], start2[3:6], start2[6:] ]
goal = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]
oper = ['up', 'down', 'right', 'left']
def readImageSafe(path):
    img =  readImageSafe(path)
    if img is None:
        print(f"⚠️ 이미지 로드 실패: {path}")
        return 255 * np.ones((100, 100, 3), dtype=np.uint8)  # 흰색 대체 이미지
    return img
def showPuzzle():
    imgList11 = ['puzzle/num_' + cur[0][0] + '.png', 'puzzle/num_' + cur[0][1] + '.png', 'puzzle/num_' + cur[0][2] + '.png']
    imgList12 = ['puzzle/num_' + cur[1][0] + '.png', 'puzzle/num_' + cur[1][1] + '.png', 'puzzle/num_' + cur[1][2] + '.png']
    imgList13 = ['puzzle/num_' + cur[2][0] + '.png', 'puzzle/num_' + cur[2][1] + '.png', 'puzzle/num_' + cur[2][2] + '.png']
    imgList1 = [ imgList11, imgList12, imgList13 ]
    #imgList2 = random.sample(imgList1, 9)

    fig, axes = plt.subplots(3, 3, figsize=(4, 4))
    plt.cla()
    plt.close()
    img1 = readImageSafe(imgList1[0][0])
    img2 = readImageSafe(imgList1[0][1])
    img3 =  readImageSafe(imgList1[0][2])
    img4 =  readImageSafe(imgList1[1][0])
    img5 =  readImageSafe(imgList1[1][1])
    img6 =  readImageSafe(imgList1[1][2])
    img7 =  readImageSafe(imgList1[2][0])
    img8 =  readImageSafe(imgList1[2][1])
    img9 =  readImageSafe(imgList1[2][2])
    img0 =  readImageSafe('puzzle/num_9.png')

    plt.subplot(3, 3, 1)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img1)

    plt.subplot(3, 3, 2)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img2)

    plt.subplot(3, 3, 3)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img3)

    plt.subplot(3, 3, 4)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img4)

    plt.subplot(3, 3, 5)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img5)

    plt.subplot(3, 3, 6)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img6)

    plt.subplot(3, 3, 7)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img7)

    plt.subplot(3, 3, 8)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img8)

    plt.subplot(3, 3, 9)
    plt.gca().axes.xaxis.set_visible(False)
    plt.gca().axes.yaxis.set_visible(False)
    plt.imshow(img9)

    #plt.subplots_adjust(left=0.01, bottom=0.01, right=0.99, top=0.99, wspace=0.00, hspace=0.00)

def movePuzzle(puzzle, x, y, oper):
    if(oper == 'up'):
        if(x - 1 < 0):
            return None
        else:
            tmp = puzzle[x][y]
            puzzle[x][y] = puzzle[x-1][y]
            puzzle[x-1][y] = tmp

            return puzzle

    elif(oper == 'down'):
        if (x + 1 >= 3):
            return None
        else:
            tmp = puzzle[x][y]
            puzzle[x][y] = puzzle[x + 1][y]
            puzzle[x + 1][y] = tmp

            return puzzle

    elif(oper == 'right'):
        if (y + 1 >= 3):
            return None
        else:
            tmp = puzzle[x][y]
            puzzle[x][y] = puzzle[x][y + 1]
            puzzle[x][y + 1] = tmp

            return puzzle

    elif(oper == 'left'):
        if (y - 1 < 0):
            return None
        else:
            tmp = puzzle[x][y]
            puzzle[x][y] = puzzle[x][y - 1]
            puzzle[x][y - 1] = tmp

            return puzzle

def checkZero(puzzle):
    x, y = 0, 0
    for i in range(3):
        for j in range(3):
            if puzzle[i][j] == '0':
                x, y = i, j
    return x, y

def add_point(event):
    #if event.inaxes != ax:
    #    return
    if event.button ==1:
        fore = pyautogui.getActiveWindow()
        pos = pyautogui.position()
        x = pos.x - fore.left
        y = pos.y - fore.top
        #print("Ŭ   : ", x, ", ", y)

        if (x >= 20 and x <= 170) and (y >= 40 and y <= 190):
            move = "no"
            if cur[0][1] == '0':
                move = 'right'
            elif cur[1][0] == '0':
                move = 'down'
            if move != 'no':
                next = movePuzzle(cur, 0, 0, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('1')

        if (x >= 180 and x <= 330) and (y >= 40 and y <= 190):
            move = "no"
            if cur[0][0] == '0':
                move = 'left'
            elif cur[0][2] == '0':
                move = 'right'
            elif cur[1][1] == '0':
                move = 'down'
            if move != 'no':
                next = movePuzzle(cur, 0, 1, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('2')

        if (x >= 340 and x <= 490) and (y >= 40 and y <= 190):
            move = "no"
            if cur[0][1] == '0':
                move = 'left'
            elif cur[1][2] == '0':
                move = 'down'
            if move != 'no':
                next = movePuzzle(cur, 0, 2, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('3')

        if (x >= 20 and x <= 170) and (y >= 200 and y <= 350):
            move = "no"
            if cur[0][0] == '0':
                move = 'up'
            elif cur[1][1] == '0':
                move = 'right'
            elif cur[2][0] == '0':
                move = 'down'
            if move != 'no':
                next = movePuzzle(cur, 1, 0, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('4')

        if (x >= 180 and x <= 330) and (y >= 200 and y <= 350):
            move = "no"
            if cur[0][1] == '0':
                move = 'up'
            elif cur[1][0] == '0':
                move = 'left'
            elif cur[1][2] == '0':
                move = 'right'
            elif cur[2][1] == '0':
                move = 'down'
            if move != 'no':
                next = movePuzzle(cur, 1, 1, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('5')

        if (x >= 340 and x <= 490) and (y >= 200 and y <= 350):
            move = "no"
            if cur[0][2] == '0':
                move = 'up'
            elif cur[1][1] == '0':
                move = 'left'
            elif cur[2][2] == '0':
                move = 'down'
            if move != 'no':
                next = movePuzzle(cur, 1, 2, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('6')

        if (x >= 20 and x <= 170) and (y >= 360 and y <= 510):
            move = "no"
            if cur[1][0] == '0':
                move = 'up'
            elif cur[2][1] == '0':
                move = 'right'
            if move != 'no':
                next = movePuzzle(cur, 2, 0, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('7')

        if (x >= 180 and x <= 330) and (y >= 360 and y <= 510):
            move = "no"
            if cur[2][0] == '0':
                move = 'left'
            elif cur[1][1] == '0':
                move = 'up'
            elif cur[2][2] == '0':
                move = 'right'
            if move != 'no':
                next = movePuzzle(cur, 2, 1, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('8')

        if (x >= 340 and x <= 490) and (y >= 360 and y <= 510):
            move = "no"
            if cur[2][1] == '0':
                move = 'left'
            elif cur[1][2] == '0':
                move = 'up'
            if move != 'no':
                next = movePuzzle(cur, 2, 2, move)
                print(move)
                showPuzzle()
                plt.show()
                if next == goal:
                    print("    ")
            print('9')
showPuzzle()
cid = plt.connect('button_press_event', add_point)
plt.subplots_adjust(left=0.01, bottom=0.01, right=0.99, top=0.99, wspace=0.01, hspace=0.01)
plt.show()