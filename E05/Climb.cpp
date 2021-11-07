#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>
#include<ctime>
#include<cstdlib>
using namespace std;

bool check_clause(vector<int>& clause, bool* var_value) {
    for (auto it = clause.begin(); it != clause.end(); it++) {
        if (( *it > 0 && var_value[abs(*it)-1] ) || 
            ( *it < 0 && !var_value[abs(*it)-1] )) {
            return true;
        }
    }
    return false;
}

int main() {
    fstream fin;
    fin.open("data.txt");
    
    // 读取文件头
    string p, cnf;
    int var_num, clause_num;
    fin >> p >> cnf >> var_num >> clause_num;

    // 读取文件内容对两个map进行初始化
    vector<int> var2clause[var_num];
    vector<int> clause2var[clause_num];
    for (int clause_id = 0; clause_id < clause_num; clause_id++) {
        int var_id = -1;
        fin >> var_id;
        while (var_id != 0) {
            clause2var[clause_id].push_back(var_id);
            var2clause[abs(var_id)-1].push_back(clause_id);
            fin >> var_id;
        }
    }
    
    bool clause_truthfulness[clause_num] = {}; // clause真值表
    bool var_value[var_num] = {}; // 变量值列表，找到一组变量赋值使得clause真值表全为1
    
    // climb with random initialize, random walk
    while (true) {
        // climb with random initialize (随机初始化var_value)
        // TODO
	    // srand(time(0));
        // for (int i = 0; i < var_num; i++) {
        //     var_value[i] = bool(rand()%2);
        // }
        // for (int i = 0; i < clause_num; i++) {
        //     clause_truthfulness[i] = check_clause(clause2var[i], var_value);
        // }

        // climb with random walk (随机选择最好的一个邻居/随机选择一个邻居)
        // TODO
    }

    // 打印结果
    for (int i = 0; i < clause_num; i++) {
        cout << clause_truthfulness[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < var_num; i++) {
        cout << var_value[i] << ' ';
    }
    cout << endl;
    return 0;
}
