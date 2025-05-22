import cv2
import random
import matplotlib.pyplot as plt
import pyautogui
import numpy as np
import copy
import time

cur = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]
start1 = ['1', '2', '3', '4', '5', '6', '7', '8', '0']
start2 = random.sample(start1, 9)

goal = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]
oper = ['up', 'down', 'right', 'left']

#정답 확인인
def is_goal(puzzle, goal):
    return puzzle == goal

#A*용 노드
class Node:
    def __init__(self, data, hval, level, parent=None):
        self.data = data
        self.hval = hval
        self.level = level
        self.parent = parent

#휴스틱 함수 가져오기
def h(puzzle, goal):
    cnt = 0
    for i in range(3):
        for j in range(3):
            if puzzle[i][j] != goal[i][j]:
                cnt += 1
    return cnt

#경로 복원
def reconstruct_path(node):
    path = []
    while node:
        path.append(node.data)
        node = node.parent
    return path[::-1] 

#현재 상태 평가
def f(puzzle, goal):
    return puzzle.level + h(puzzle.data, goal)

def to_tuple(board):
    return tuple(tuple(row) for row in board)

#A*로직 
def astar(puzzle):
    queue = []
    visited = set()
    start = Node(data=puzzle, hval=h(puzzle, goal), level=0, parent=None)
    queue.append(start)

    while queue:
        current = queue.pop(0)
        if h(current.data, goal) == 0:
            return reconstruct_path(current)

        visited.add(to_tuple(current.data))
        x, y = checkZero(current.data)

        for op in oper:
            next_state = movePuzzle(copy.deepcopy(current.data), x, y, op)
            if next_state is not None and to_tuple(next_state) not in visited:
                queue.append(Node(next_state, h(next_state, goal), current.level + 1, parent=current))

        queue.sort(key=lambda x: f(x, goal))
    return None

def play_solution(path):
    global cur
    for state in path:
        cur = copy.deepcopy(state)
        showPuzzle()
        plt.pause(0.3)  # 0.3초 간격으로 자동 이동
    print("성공!")
    plt.close()

def showPuzzle():
    imgList11 = ['puzzle/num_' + cur[0][0] + '.png', 'puzzle/num_' + cur[0][1] + '.png', 'puzzle/num_' + cur[0][2] + '.png']
    imgList12 = ['puzzle/num_' + cur[1][0] + '.png', 'puzzle/num_' + cur[1][1] + '.png', 'puzzle/num_' + cur[1][2] + '.png']
    imgList13 = ['puzzle/num_' + cur[2][0] + '.png', 'puzzle/num_' + cur[2][1] + '.png', 'puzzle/num_' + cur[2][2] + '.png']
    imgList1 = [ imgList11, imgList12, imgList13 ]
    #imgList2 = random.sample(imgList1, 9)

    fig, axes = plt.subplots(3, 3, figsize=(4, 4))
    plt.cla()
    plt.close()
    img1 = cv2.imread(imgList1[0][0])
    img2 = cv2.imread(imgList1[0][1])
    img3 = cv2.imread(imgList1[0][2])
    img4 = cv2.imread(imgList1[1][0])
    img5 = cv2.imread(imgList1[1][1])
    img6 = cv2.imread(imgList1[1][2])
    img7 = cv2.imread(imgList1[2][0])
    img8 = cv2.imread(imgList1[2][1])
    img9 = cv2.imread(imgList1[2][2])
    img0 = cv2.imread('puzzle/num_9.png')

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

def shuffle_puzzle(goal_state=None, max_shuffle=50):
    """
    goal_state가 없으면 기본 goal 상태에서 shuffle_count번 무작위로 유효하게 이동합니다.
    항상 도달 가능한 상태만 반환합니다.
    """
    if goal_state is None:
        puzzle = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]
    else:
        puzzle = copy.deepcopy(goal_state)
    
    x, y = checkZero(puzzle)

    shuffle_count = random.randint(10, max_shuffle)

    for _ in range(shuffle_count):
        direction = random.choice(oper)
        new_puzzle = copy.deepcopy(puzzle)
        result = movePuzzle(new_puzzle, x, y, direction)
        if result:
            puzzle = result
            x, y = checkZero(puzzle)
    
    return puzzle
cur=shuffle_puzzle()

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
                cur[:] = next 
                print(move)
                showPuzzle()
                plt.show()
                if is_goal(cur, goal):
                    print("성공") #성공 터미널
                    plt.close() #창 닫히기
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
                cur[:] = next 
                showPuzzle()
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur,goal):
                    print("성공")
                    plt.close()
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
                cur[:]=next
                showPuzzle()
                plt.show()
                if is_goal(cur,goal):
                    print("성공")
                    plt.close()
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
                cur[:]=next
                showPuzzle()            
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur,goal):
                    print("성공")
                    plt.close()
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
                cur[:] = next 
                showPuzzle()
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur, goal):
                    print("성공")
                    plt.close()
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
                cur[:] = next 
                showPuzzle()
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur, goal):
                    print("성공")
                    plt.close()
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
                cur[:] = next 
                showPuzzle()
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur, goal):
                    print("성공")
                    plt.close()
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
                cur[:] = next 
                showPuzzle()
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur, goal):
                    print("성공")
                    plt.close()
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
                cur[:] = next 
                showPuzzle()
                solution = astar(cur)
                if solution:
                    play_solution(solution)
                else:
                    print("해결 불가")
                plt.show()
                if is_goal(cur, goal):
                    print("성공")
                    plt.close()
            print('9')
showPuzzle()
cid = plt.connect('button_press_event', add_point)
plt.subplots_adjust(left=0.01, bottom=0.01, right=0.99, top=0.99, wspace=0.01, hspace=0.01)
plt.show()