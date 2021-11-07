# P01 Pacman Game

| 学号    | 姓名 | 专业(方向)     |
| ------- | ---- | -------------- |
| 5201314 | 铁柱 | 混凝土移动工程 |

### 1.Idea of A* Algorithm (Use a few sentences to describe your understanding of the algorithm)

- blablabla

### 2. Idea of Min-Max and alpha-beta pruning algorithms

- The principle of Min-Max algorithm
- The principle of α-β pruning

### 3. Codes

**Question 1**

```python
visited = []
    priorityQueue = util.PriorityQueue()
    path = [] 

    start = {
        'state' : problem.getStartState(),
        'cost' : 0,
        'parent' : None,
        'action' : None,
        'h' : heuristic(problem.getStartState(), problem)
    }

    priorityQueue.push(start, start['cost'] + start['h'])

    while (not priorityQueue.isEmpty()):
        parent = priorityQueue.pop()

        if (parent['state'] not in visited):
            visited.append(parent['state'])

            if (problem.isGoalState(parent['state'])):
                break

            successors = problem.getSuccessors(parent['state'])

            for successor in list(successors):
                if (successor[0] not in visited):
                    child = {
                        'state' : successor[0],
                        'cost' : parent['cost'] + successor[2],
                        'parent' : parent,
                        'action' : successor[1],
                        'h' : heuristic(successor[0], problem)
                    }
                    priorityQueue.update(child, child['cost'] + child['h'])

    v = parent
    while (v['action'] != None):
        path = [v['action']] + path
        v = v['parent']
    
    return path
```

**Question 2**

```python
# 这里填写python代码
```

**Question 3**

```java
// 这里填写java代码
```

**Question 4**

```go
// 这里填写go代码
```

**Question 5**

```c#
// 这里填写c#代码
```

### 4.结果展示

![Here's a screenshot of your experimental results]()

### 5.结果分析

#### 1.Search in Pacman

- Briefly analyze the differences between the heuristic functions of the three problems, and why they can lead to the corresponding results 
- If u have innovation points, just write it down.

#### 2.Multi-Agent Pacman

- Briefly analyze the complexity difference between α-β pruning and minmax algorithm（hints: search depth and time）

### 6.Experimental experience
