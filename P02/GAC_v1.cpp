#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <list>
#include <ctime>
#include <windows.h>
using namespace std;

#define POINT pair<int, int>
#define PUZZLE vector<vector<int> >
#define DOMAINS vector<vector<set<int> > >

long long node_find = 0;


struct Constraint {
    //0为不等式约束，>0为行约束，<0为列约束;
    //行列数从1开始计数, 且列值取负, 否则与不等式约束重复冲突。
    int type;
    // 不等式约束，relation.first < relation.second
    pair<POINT, POINT> relation;               
    Constraint(int t = 0, pair<POINT, POINT> r = {{-1, -1},{-1, -1}})
    : type(t), relation(r) {}
    //重载约束比较, 不加这个会报错
    bool operator==(const Constraint& rhs) { 
        return (type == rhs.type && relation == rhs.relation);
    }
};

class Futoshiki {
private:
    int size;     // puzzle尺寸, 通过读取文件确定
    PUZZLE puzzle;
    vector<Constraint> conArray; 
public:
    Futoshiki(const string filename);
    bool isSolved(); 
    void printPuzzle();
    void FilePrintPuzzle(const string filename);

    // 初始化每个棋盘格子域
    DOMAINS makeDomains(list<Constraint>& gacQ);  

    // MinimumPos, 与FC的MinimumPos类似
    POINT MinimumPos();

    // 在具体实现中, 队列使用C++ STL list
    // 把与变量pos相关的约束不重复地加入队列
    void AddConstraint2Q(POINT pos, list<Constraint>& gacQ); 

    PUZZLE Gac(const DOMAINS& domains, list<Constraint>& gacQ);   
    DOMAINS GacEnforce(DOMAINS domains, list<Constraint>& gacQ);  
};

Futoshiki::Futoshiki(const string filename){
    size = 0;
    ifstream fin(filename.c_str());
    string firstLine, tmp;
    int con_num = 0;
    //确定puzzle尺寸
    getline(fin, firstLine);
    stringstream ss;ss<<firstLine;
    while(ss>>tmp) size++;
    //重置文件指针
    fin.clear();fin.seekg(0);
    puzzle.assign(size, vector<int>(size, 0));
    
    for (int i = 0; i < size; i++)  for (int j = 0; j < size; j++) 
    fin >> puzzle[i][j];
    //重置文件指针
    fin.clear();fin.seekg(0);
    for(int _=1; _<=size; _++) getline(fin, firstLine);
    //确定约束条件个数
    while(getline(fin, firstLine)) con_num++;
    fin.clear();fin.seekg(0);
    for(int _=1; _<=size; _++) getline(fin, firstLine);
    
    for (int i = 0; i < con_num; i++) {
        int x1, y1, x2, y2;
        fin >> x1 >> y1 >> x2 >> y2;
        conArray.push_back(Constraint(0, {{x1, y1}, {x2, y2}}));
    }
    fin.close();
}

bool Futoshiki::isSolved() {
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) 
        if (puzzle[i][j] == 0)  return false;
    
    return true;
}

void Futoshiki::printPuzzle() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) printf("%d ", puzzle[i][j]);
        puts("");
    }
    puts("=================");
}

void Futoshiki::FilePrintPuzzle(const string filename) {
    ofstream fout (filename.c_str());
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) fout<<puzzle[i][j]<<" ";
        fout<<'\n';
    }
    fout<<"=================\n";
    fout.close();
}

POINT Futoshiki::MinimumPos() {
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++)
        if (puzzle[i][j] == 0) return {i, j};
    
    return POINT();
}

void Futoshiki::AddConstraint2Q(POINT pos, list<Constraint>& gacQ) {
    for (int i = 0; i < conArray.size(); i++) {
        if (conArray[i].type != 0) continue;// 不等式约束
        
        pair<POINT, POINT> relation = conArray[i].relation;
        if (pos == relation.first || pos == relation.second) {
            auto constr_position = find(gacQ.begin(), 
                                        gacQ.end(), conArray[i]);
            if (constr_position == gacQ.end())   // 不重复加入, 以下同理
                gacQ.push_back(conArray[i]);
        }
    }
    //行约束
    Constraint row_constraint(Constraint(pos.first + 1));
    auto row_position = find(gacQ.begin(), gacQ.end(), row_constraint);
    if (row_position == gacQ.end()) gacQ.push_back(row_constraint);  
        
    //列约束
    Constraint col_constraint(Constraint(-pos.second - 1));
    auto col_position = find(gacQ.begin(), gacQ.end(), col_constraint);
    if (col_position == gacQ.end()) gacQ.push_back(col_constraint);
}

DOMAINS Futoshiki::makeDomains(list<Constraint>& gacQ) {
    // 初始化
    DOMAINS domains(size, vector<set<int>>(size, set<int>()));
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++){
        if (puzzle[i][j] == 0) for (int k = 0; k < size; k++) 
            domains[i][j].insert(k + 1); 
        else domains[i][j].insert(puzzle[i][j]);
    }
    
    // 将所有行列约束加入gacQ
    for (int rc = 1; rc <= size; rc++) {
        gacQ.push_back(Constraint(rc));   
        gacQ.push_back(Constraint(-rc)); 
    }
    for (int i = 0; i < conArray.size(); i++) 
    gacQ.push_back(conArray[i]);
    
    return GacEnforce(domains, gacQ);  // 执行Gac_enforce
}

PUZZLE Futoshiki::Gac(const DOMAINS& domains, list<Constraint>& gacQ) {
    if (domains.size() == 0) return PUZZLE();   //DWO
    if (isSolved()) return puzzle;
    POINT pos = MinimumPos();
    set<int> field = domains[pos.first][pos.second];
    for (auto pd = field.begin(); pd != field.end(); pd++) {
        puzzle[pos.first][pos.second] = *pd;
        // 在domains副本上修改,否则原iteration失效
        auto temp_domains = domains;          
        temp_domains[pos.first][pos.second].clear();
        temp_domains[pos.first][pos.second].insert(*pd);
        AddConstraint2Q(pos, gacQ);
        
        temp_domains = GacEnforce(temp_domains, gacQ);
        
        if (temp_domains.size() != 0) {  // not DWO
            node_find++;
            PUZZLE res = Gac(temp_domains, gacQ); //gac递归
            if (res.size() != 0) return res; 
        }
    }
    puzzle[pos.first][pos.second] = 0;  // 回溯
    return PUZZLE();                    // DWO
}

/* Gac enforce */
DOMAINS Futoshiki::GacEnforce(DOMAINS domains, list<Constraint>& gacQ){
    while (!gacQ.empty()) {//类似于bfs
        Constraint constraint = gacQ.front(); gacQ.pop_front();
        // 已优化不等式约束
        if (constraint.type == 0) {
            POINT sp = constraint.relation.first;
            POINT lp = constraint.relation.second;
            int low = *domains[sp.first][sp.second].begin();
            for (int k = 1; k <= low; k++) {
                bool flag = domains[lp.first][lp.second].erase(k);
                if (flag) {//确定是否删除了， 好进行下一步操作, 以下同理
                    if (domains[lp.first][lp.second].size() == 0) 
                        return DOMAINS();  // DWO
                    AddConstraint2Q(lp, gacQ);
                }
            }
            int high = *domains[lp.first][lp.second].rbegin();
            for (int k = high; k <= size; k++) {
                bool flag = domains[sp.first][sp.second].erase(k);
                if (flag) {
                    if (domains[sp.first][sp.second].size() == 0) 
                        return DOMAINS();  // DWO
                    AddConstraint2Q(sp, gacQ);
                }
            }
        }
        // 已优化行约束
        else if (constraint.type > 0) {
            int row = constraint.type - 1;
            //判断唯一取值是否符合要求, 以下同理
            for (int j = 0; j < size; j++) if (domains[row][j].size() == 1) {
                int value = *domains[row][j].begin();
                for (int j1 = 0; j1 < size; j1++) if (j1 != j) {
                    bool flag = domains[row][j1].erase(value);
                    if (flag) {
                        if (domains[row][j1].size() == 0) return DOMAINS();  // DWO
                        AddConstraint2Q({row, j1}, gacQ);
                    }
                }
            }
        }
        // 已优化列约束
        else {
            int col = -constraint.type - 1;
            for (int i = 0; i < size; i++) if (domains[i][col].size() == 1) {
                int value = *domains[i][col].begin();
                for (int i1 = 0; i1 < size; i1++) if (i1 != i) {
                    bool flag = domains[i1][col].erase(value);
                    if (flag) {
                        if (domains[i1][col].size() == 0) return DOMAINS();  // DWO
                        AddConstraint2Q({i1, col}, gacQ);
                    }
                }
            }
        }
    }
    return domains;
}

int main() {
    for(int t = 1; t <= 5; t++){
        node_find = 0;
        LARGE_INTEGER start_time, end_time, frequency;
        bool flag;
        string testcase = "TestCase/data"+to_string(t)+".txt";
        Futoshiki game(testcase);
        list<Constraint> gacQ;
        game.printPuzzle();
        // 初始化各变量域并执行GacEnforce
        auto domains = game.makeDomains(gacQ); 

        flag = QueryPerformanceFrequency(&frequency);
        if(flag) flag = QueryPerformanceCounter(&start_time);//start = clock();

        PUZZLE result = game.Gac(domains, gacQ);

        if(flag) QueryPerformanceCounter(&end_time); //end = clock();
        long long dur_time = (end_time.QuadPart-start_time.QuadPart)*1000000 / frequency.QuadPart;

        game.printPuzzle();
        string filename = "TestCase/res_gac"+to_string(t)+".txt";
        game.FilePrintPuzzle(filename);
        ofstream fout (filename.c_str());
        fout<<node_find<<"\n";
        fout<<"time is "<<dur_time<<" us\n\n";
        fout.close();
        printf("node_find = %lld\n", node_find);
        printf("time is %lld us\n\n", dur_time);
    }
    puts("end");
    return 0;
}