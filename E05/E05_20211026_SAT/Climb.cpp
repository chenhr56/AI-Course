#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <windows.h>
using namespace std;

int var_num, clause_num;
vector<vector<int> > var2clause;
vector<vector<int> > clause2var;
LARGE_INTEGER start_time, end_time, frequency;

int check_clause(vector<int>& c2v, vector<int>& var_value) {
    for (int num:c2v) {
        if ((num > 0 &&  var_value[abs(num)-1]) || 
            (num < 0 && !var_value[abs(num)-1])) {
            return 1;
        }
    }
    return 0;
}

struct State{
    vector<int> var_value;
    vector<int> clause_truth;
    State(){
        var_value.resize(var_num);
        clause_truth.resize(clause_num);
    }
};

bool cmp(State a, State b){
    int n1 = count(a.clause_truth.begin(), a.clause_truth.end(), 1);
    int n2 = count(b.clause_truth.begin(), b.clause_truth.end(), 1);
    return  n1 > n2;
}

vector<int> res_clause_truthfulness, res_var_value;
void serach(State *neiberState, State oriState){
    int true_num = count(oriState.clause_truth.begin(), oriState.clause_truth.end(), 1);
    if(true_num == clause_num){//找到全局最优解
        QueryPerformanceCounter(&end_time);
        printf("clause_truth: \n");
        for(int _=0; _<clause_num; _++){
            printf("%d ", oriState.clause_truth[_]);
        }
        printf("\nvar_value: \n");
        for(int _=0; _<var_num; _++){
            printf("%d ", oriState.var_value[_]);
        }
        long long dur_time = (end_time.QuadPart-start_time.QuadPart)*1000000 / frequency.QuadPart;
        printf("Total Time is %lld us\n", dur_time);
        exit(0);
    }
    for(int i = 0; i < var_num; i++){
        int n1 = count(neiberState[i].clause_truth.begin(), neiberState[i].clause_truth.end(), 1);
        int n2 = count(oriState.clause_truth.begin(), oriState.clause_truth.end(), 1);
        if(n1 < n2) break;
        State nextState[var_num];
        for(int i=0; i<var_num; i++){
            nextState[i].var_value.assign(neiberState[i].var_value.begin(), neiberState[i].var_value.end());
            nextState[i].var_value[i] ^= 1;
            for(int j=0; j<clause_num; j++)
                nextState[i].clause_truth[j]=check_clause(clause2var[j], nextState[i].var_value);
        }
        sort(nextState, nextState+var_num, cmp);
        serach(nextState, neiberState[i]);
    }
}

int main() {
    QueryPerformanceFrequency(&frequency);
    fstream fin; fin.open("data.txt");
    // 读取文件头
    string p, cnf; fin >> p >> cnf >> var_num >> clause_num;
    
    //printf("var_num = %d  clause_num = %d\n", var_num, clause_num);

    // 读取文件内容对两个map进行初始化
    var2clause.resize(var_num);
    clause2var.resize(clause_num);
    for (int clause_id = 0; clause_id < clause_num; clause_id++) {
        int var_id = -1;
        fin >> var_id;
        while (var_id != 0) {
            clause2var[clause_id].push_back(var_id);
            var2clause[abs(var_id)-1].push_back(clause_id);
            fin >> var_id;
        }
    }

    //bool clause_truthfulness[clause_num] = {}; // clause真值表
    //bool var_value[var_num] = {}; // 变量值列表，找到一组变量赋值使得clause真值表全为1
    State oriState;
    for(int i = 0; i < var_num; i++) 
        oriState.var_value[i] = (rand() % 2);

    //for(int i=0; i < var_num; i++) printf("%d ", oriState.var_value[i]);puts("");

    for(int i = 0; i < clause_num; i++) 
        oriState.clause_truth[i] = check_clause(clause2var[i], oriState.var_value);

    //for(int i=0; i < clause_num; i++) printf("%d ", oriState.clause_truth[i]);puts("");

    int num = count(oriState.clause_truth.begin(), oriState.clause_truth.end(), 1);
    //printf("origin clause_truth count is %d\n", num);

    State neiberState[var_num];
    for(int i=0; i<var_num; i++){
        neiberState[i].var_value.assign(oriState.var_value.begin(), oriState.var_value.end());
        neiberState[i].var_value[i] ^= 1;
        for(int j=0; j<clause_num; j++)
            neiberState[i].clause_truth[j]=check_clause(clause2var[j], neiberState[i].var_value);
    }
    sort(neiberState, neiberState+var_num, cmp);
/*
    puts("neiberState clause_truth:");
    
    for(int i=0; i<var_num; i++){
        int _ = count(neiberState[i].clause_truth.begin(),neiberState[i].clause_truth.end(), 1);
        printf("%d ", _);
    } puts("");
*/
    QueryPerformanceCounter(&start_time);
    serach(neiberState, oriState);

    return 0;
} 
