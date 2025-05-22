
import numpy as np
import copy

cur = [ ['2', '8', '3'], ['1', '6', '4'], ['7', '0', '5'] ]
goal = [ ['1', '2', '3'], ['8', '0', '4'], ['7', '6', '5'] ]
oper = ['up', 'down', 'right', 'left']

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

class Node:
    def __init__(self, data, hval, level):
        self.data = data
        self.hval = hval
        self.level = level

def h(puzzle, goal):
    cnt = 0
    for i in range(3):
        for j in range(3):
            if puzzle[i][j] != goal[i][j]:
                cnt += 1
    return cnt

def f(puzzle, goal):
    return puzzle.level + h(puzzle.data, goal)

def astar(puzzle):
    visit = []
    queue = []
    start = Node(data=puzzle, hval=h(puzzle=puzzle, goal=goal), level=0)
    queue.append(start)

    while queue is not None:
        current = queue.pop(0)
        print(np.array(current.data))
        if (h(current.data, goal) == 0):
            return visit
        else:
            visit.append(current.data)
            x, y = checkZero(current.data)

            for op in oper:
                next = movePuzzle(copy.deepcopy(current.data), x, y, op)

                if next not in visit and next is not None:
                    queue.append(Node(next, h(next, goal), current.level + 1))
            queue.sort(key=lambda x: f(x, goal), reverse=False)
    return -1

astar(cur)