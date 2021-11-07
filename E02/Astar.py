from queue import PriorityQueue
from copy import deepcopy
matr = []
step = []

'''
运行方式：
运行后再控制台输入初始状态即可。
初始样例如下：
5 1 3 4
9 2 7 8
13 6 10 11
14 16 15 12

5 1 3 4
2 7 8 12
9 6 11 15
16 13 10 14

1 2 12 3
5 14 16 7
13 6 15 4
10 9 11 8

6 10 3 15
14 8 7 11
5 1 16 2
13 12 9 4

有兴趣的同学可以尝试更难的测例
12 4 2 8
5 7 9 3
16 10 11 14
15 13 6 1

15 11 7 1
5 10 2 9
3 4 6 12
13 14 8 16

1 6 16 15
8 10 7 14
2 3 13 11
9 12 5 4

7 11 4 16
15 9 8 12
6 2 1 3
14 13 10 5
'''


# 输入16个数字作为初始矩阵

for _ in range(4):
    matr.append(list(map(int, input().split())))

#matr = [[6, 10, 3, 15], [14, 8, 7, 11], [5, 1, 16, 2], [13, 12, 9, 4]]


def h1(a):
    """
    @description  :the number of misplaced tiles
    ---------
    @param  :
    a:当前矩阵
    -------
    @Returns  :
    返回h
    -------
    """

    ans = 0
    for i in range(4):
        for j in range(4):
            if a[i][j] != 4 * i + j + 1:
                ans = ans + 1

    pass
    return ans


def h2(a):
    """
    @description  :the sum of the distances of the tiles from their goal positions.
    ---------
    @param  :
    a:矩阵
    -------
    @Returns  :
    返回h
    -------
    """
#    aa = deepcopy(a)
    ans = 0
    for i in range(4):
        for j in range(4):
            ans = ans+abs(i-(a[i][j]-1) // 4)+abs(j-(a[i][j]-1) % 4)
    pass
    return ans


def printt(a):
    for i in range(4):
        file.write(*(a[i]))
#        print(*(a[i]))
#    print()
    file.write('\r\n')


def blank(a):
    for i in range(4):
        try:
            j = a[i].index(16)
            return i, j
        except:
            pass


def visial_path(paths):
    """
    @description  :可视化路径
    ---------
    @param  :
    paths:输入路径list
    -------
    @Returns  :None
    -------
    """
    for i, path in enumerate(paths):
        visial_matrix(path)
#        print("========= step " + str(i) + "\t=========")
        file.write("========== step " + str(i) + " ==========\r\n")


def visial_matrix(matrix):
    """
    @description  :可视化一个矩阵
    ---------
    @param  :
    matrix：输入一个矩阵
    -------
    @Returns  :None
    -------
    """
    for m in matrix:
        for k in m:
            if k < 10:
#                print(str(0) + str(k), end=' ')
                file.write(str(0) + str(k) + str(' '))
            else:
#                print(str(k), end=' ')
                file.write(str(k) + str(' '))
#        print()
        file.write('\r\n')


def tryy(a, i, j, d):
    """
    @description  :当前矩阵根据方向移动一个格子
    ---------
    @param  :
    a:当前矩阵
    i:空白x坐标
    j:空白y坐标
    d:移动方向
    -------
    @Returns  :
    返回移动后矩阵
    -------
    """

    aa = deepcopy(a)
    if d == 'U':
        if i + 1 < 4:
            aa[i][j], aa[i + 1][j] = aa[i + 1][j], aa[i][j]
            return aa
        else:
            return None
    elif d == 'L':
        if j+1 < 4:
            aa[i][j], aa[i][j + 1] = aa[i][j + 1], aa[i][j]
            return aa
        else:
            return None
    elif d == 'D':
        if i - 1 >= 0:
            aa[i][j], aa[i - 1][j] = aa[i - 1][j], aa[i][j]
            return aa
        else:
            return None
    elif d == 'R':
        if j - 1 >= 0:
            aa[i][j], aa[i][j - 1] = aa[i][j - 1], aa[i][j]
            return aa
        else:
            return None


def is_goal(arr):
    return arr == [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]]


def search(path, cost, bound, heuristic):
    global step
    '''
    补充代码
    1.取得path中最后一个状态
    2.计算f = g + h
    '''

    arr = path[-1]
    f = cost + heuristic(arr)

    if f > bound:
        return f, False
    if is_goal(arr):
        return f, True

    (bi, bj) = blank(arr)
    min = 0x3f3f3f3f

    for d in ['U', 'L', 'D', 'R']:
        '''
        1.利用tryy方法得出下一步的状态
        2.判断是否在path，不在加入path
        3.search搜索
        4.如果成功找到解，终止搜索
        5.否则增大搜索界限(也就是上面的min)，加深搜索
        '''
        newArr = tryy(arr, bi, bj, d)
        if newArr == None:
            continue
        else:
            if newArr not in path:
                path.append(newArr)
                t, flag = search(path, cost + 1, bound, heuristic)
                if flag == True:
                    return t, True
                if bound < min:
                    min = t
                path.pop()
            pass

    return min, False


def ida():
    global step
    bound = h2(matr)
    path = [matr]
    while True:
        bound, flag = search(path, 0, bound, h2)
        if flag == True:
            break
        if bound == 0x3f3f3f3f:
            print("impossible!")
            return
    visial_path(path)


file = open('test1_h2_5.txt', 'w')

ida()

