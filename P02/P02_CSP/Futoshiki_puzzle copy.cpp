//
// Created by GreenArrow on 2021/10/12.
//

#include <iostream>
#include <vector>

using namespace std;

class FutoshikiPuzzle {
public:
    vector<vector<int>> maps;
    vector<pair<pair<int, int>, pair<int, int>>> less_constraints;
    int nRow, nColumn;
    //表示第x行中某个数字是否存在
    int Count_RowNumbers[5][6];
    //表示第y列某个数字是否存在
    int Count_ColumnNumbers[5][6];

    void initial() {
        //初始地图
        maps = {{0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 4}};
        nRow = maps.size();
        nColumn = maps[0].size();

        //初始化行列约束
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (maps[i][j] != 0) {
                    Count_RowNumbers[i][maps[i][j]]++;
                    Count_ColumnNumbers[j][maps[i][j]]++;
                }
            }
        }
        //添加比较符号约束
        addConstraints(0, 1, 0, 0);
        addConstraints(0, 0, 1, 0);
        addConstraints(1, 1, 1, 2);
        addConstraints(1, 2, 1, 3);
        addConstraints(1, 3, 1, 4);
        addConstraints(2, 1, 2, 2);
        addConstraints(4, 1, 4, 1);
    }

    void addConstraints(int x, int y, int x1, int y1) {
        less_constraints.push_back({{x,  y},
                                    {x1, y1}});
    }

    //check函数检查当前位置是否可行，
    //以下注释掉的内容是back tracking算法的check函数
    //你们需要自行实现GAC算法的check部分
    bool check(int x, int y) {
//        for (int i = 1; i < 10; i++) {
//            if (Count_RowNumbers[x][i] > 1 || Count_ColumnNumbers[y][i] > 1) {
//                return false;
//            }
//        }
//
//        for (auto &less_constraint : less_constraints) {
//            if (less_constraint.first.first == x && less_constraint.first.second == y) {
//                if (maps[x][y] == 9) {
//                    return false;
//                }
//                if (maps[less_constraint.second.first][less_constraint.second.second] > 0 &&
//                    maps[less_constraint.second.first][less_constraint.second.second] <= maps[x][y]) {
//
//                    return false;
//                }
//            }
//        }
//
//        for (auto &less_constraint : less_constraints) {
//            if (less_constraint.second.first == x && less_constraint.second.second == y) {
//                if (maps[x][y] == 1) {
//
//                    return false;
//                }
//                if (maps[less_constraint.first.first][less_constraint.first.second] > 0 &&
//                    maps[less_constraint.first.first][less_constraint.first.second] >= maps[x][y]) {
//
//                    return false;
//                }
//            }
//        }
//        return true;
    }

    //显示结果
    void show() {
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nColumn; j++) {
                cout << maps[i][j] << " ";
            }
            cout << endl;
        }
        cout << "======================" << endl;
    }
    //搜索流程，可以不用修改这部分
    bool search(int x, int y) {

        if (maps[x][y] == 0) {
            for (int i = 1; i < 10; i++) {
                maps[x][y] = i;
                Count_RowNumbers[x][i]++;
                Count_ColumnNumbers[y][i]++;
                if (check(x, y)) {
                    if (x == 8 && y == 8) {
                        return true;
                    }
                    int next_x, next_y;
                    if (y != 8) {
                        next_x = x;
                        next_y = y + 1;
                    } else {
                        next_x = x + 1;
                        next_y = 0;
                    }


                    if (search(next_x, next_y)) {
                        return true;
                    }
                }
                maps[x][y] = 0;
                Count_RowNumbers[x][i]--;
                Count_ColumnNumbers[y][i]--;
            }
        } else {
            if (x == 8 && y == 8) {
                return true;
            }
            int next_x, next_y;
            if (y != 8) {
                next_x = x;
                next_y = y + 1;
            } else {
                next_x = x + 1;
                next_y = 0;
            }


            if (search(next_x, next_y)) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    FutoshikiPuzzle *futoshikiPuzzle = new FutoshikiPuzzle();
    futoshikiPuzzle->initial();
    futoshikiPuzzle->show();
    futoshikiPuzzle->search(0, 0);
    futoshikiPuzzle->show();
}