#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <windows.h>
#define PII pair<int, int>
#define PUZZLE vector<vector<int> >
#define DOMAIN vector<vector<set<int> > >
using namespace std;
//const int size=9;

long long node_find = 0;

LARGE_INTEGER node_start_time, node_end_time, frequency;
long long count_time = 0;

//DWO: Domain Wipe Out，表示该节点（变量）的值域Domain已经为空。
class Futoshiki{
public:
	int size = 0;
	PUZZLE puzzle;
    vector<pair<PII, PII>> less_constraints;
    Futoshiki(const string filename):size(0){
    	ifstream fin(filename.c_str());
    	string firstLine, tmp;
    	int con_num = 0;
    	getline(fin, firstLine);

    	stringstream ss;ss<<firstLine;
    	while(ss>>tmp) size++;
    	
    	fin.clear();fin.seekg(0);
    	
    	puzzle.assign(size, vector<int>(size, 0));

    	for (int i = 0; i < size; i++)  for (int j = 0; j < size; j++) 
    	    fin >> puzzle[i][j];

    	fin.clear();fin.seekg(0);
    	for(int _=1; _<=size; _++) getline(fin, firstLine);
    	while(getline(fin, firstLine)) con_num++;
    	fin.clear();fin.seekg(0);
    	for(int _=1; _<=size; _++) getline(fin, firstLine);

    	for (int i = 0; i < con_num; i++) {
    	    int x1, y1, x2, y2;
    	    fin >> x1 >> y1 >> x2 >> y2;
    	    //printf("%d-%d-%d-%d\n", x1, y1, x2, y2);
    	    add(x1, y1, x2, y2);
    	}
    	//puts("");
    	fin.close();
    }
    //计算总可行数
    int domainCount(const DOMAIN& domains) {
        int count = 0;
        for(int i = 0; i < size; i++) for(int j = 0; j < size; j++)
            count += domains[i][j].size();
        return count;
    }

    void add(int x, int y, int x1, int y1){
    	less_constraints.push_back({{x, y}, {x1, y1}});
    }

    bool isSolved(){
    	for(int i=0; i<size; i++) for(int j=0; j<size; j++) 
    		if(puzzle[i][j]==0) return false;
    	return true;
    }

    //初始化每个格子的可行域
    DOMAIN makeDomains(){
    	DOMAIN domains(size, vector<set<int> >(size, set<int>()));
    	for(int i = 0; i < size; i++) for(int j = 0; j < size; j++){
    		if(puzzle[i][j]==0) for(int k = 0; k < size; k++) domains[i][j].insert(k + 1);
    		else domains[i][j].insert(puzzle[i][j]);
    	}

    	for(int i = 0; i < size; i++) for(int j = 0; j < size; j++){
    		if(puzzle[i][j]!=0){
    			for(int ii = 0; ii < size; ii++) if(ii != i) domains[ii][j].erase(puzzle[i][j]);
    			for(int jj = 0; jj < size; jj++) if(jj != j) domains[i][jj].erase(puzzle[i][j]);
    		}
    	}
    	//清除不符合约束条件的数
    	for (int i = 0; i < (int)less_constraints.size(); i++) {
            PII sp = less_constraints[i].first;
            PII lp = less_constraints[i].second;
            if (puzzle[lp.first][lp.second] != 0) { 
                for (int k = puzzle[lp.first][lp.second]; k <= size; k++) domains[sp.first][sp.second].erase(k);
            }
            else {
                int minimum = *domains[sp.first][sp.second].begin();
                domains[lp.first][lp.second].erase(minimum);
            }
            if (puzzle[sp.first][sp.second] != 0) {
                for (int k = 1; k <= puzzle[sp.first][sp.second]; k++) {
                    domains[lp.first][lp.second].erase(k);
                }
            }
            else {
                int minimum = *domains[lp.first][lp.second].rbegin();//取最后元素
                domains[sp.first][sp.second].erase(minimum);
            }
        }
    	return domains;
    }

    //在每次迭代中使用MRV函数选择一个位置，并在其域中分配一个值。然后通过删除一些与赋值冲突的值来更新一些格子的域。
    DOMAIN updateDomains(DOMAIN domains, const PII& pos) {
    	// 检查列
    	for (int i = 0; i < size; i++) {
    	    if (i == pos.first) continue;
    	    else if (puzzle[i][pos.second] == puzzle[pos.first][pos.second]) return DOMAIN();		// DWO
    	    else {
    	        domains[i][pos.second].erase(puzzle[pos.first][pos.second]);
    	        if (domains[i][pos.second].size() == 0) return DOMAIN(); // DWO
    	    }
    	}

    	// 检查行
    	for (int j = 0; j < size; j++) {
    	    if (j == pos.second) continue;
    	    else if (puzzle[pos.first][j] == puzzle[pos.first][pos.second]) 
    	        return DOMAIN();  		// DWO
    	    
    	    else {
    	        domains[pos.first][j].erase(puzzle[pos.first][pos.second]);
    	        if (domains[pos.first][j].size() == 0) 
    	            return DOMAIN();  	// DWO
    	        
    	    }
    	}

	    // 检查约束条件
	    for (int i = 0; i < (int)less_constraints.size(); i++) {
	        PII sp = less_constraints[i].first;
	        PII lp = less_constraints[i].second;
	        if (pos == lp) {
	            for (int k = puzzle[pos.first][pos.second]; k <= size; k++) {
	                domains[sp.first][sp.second].erase(k);
	                if (puzzle[sp.first][sp.second] == 0 && domains[sp.first][sp.second].size() == 0) 
	                    return DOMAIN();  // DWO
	                
	            }
	        } 
	        else if (pos == sp) {
	            for (int k = 1; k <= puzzle[pos.first][pos.second]; k++) {
	                domains[lp.first][lp.second].erase(k);
	                if (puzzle[lp.first][lp.second] == 0 && domains[lp.first][lp.second].size() == 0) 
	                    return DOMAIN();  // DWO
	                
	            }
	        }
	    }
	    return domains;
	}

	//选择可行域最小的格子并返回其位置, minimum remaining values (MRV)
	PII MRV(const DOMAIN& domains) {
    	int val = 114514;  
    	PII pos = make_pair(-1, -1);
    	for (int i = 0; i < size; i++) {
    	    for (int j = 0; j < size; j++) {
    	        if (puzzle[i][j] == 0 && domains[i][j].size() < val) {
    	            val = domains[i][j].size();
    	            pos = make_pair(i, j);
    	        }
    	    }
    	}
    	return pos;
	}

	PUZZLE ForwardChecking(const DOMAIN& domains) {
	    if (isSolved()) return puzzle;
	    PII pos = MRV(domains);
	    for (auto p = domains[pos.first][pos.second].begin(); p != domains[pos.first][pos.second].end(); p++) {
	    	QueryPerformanceCounter(&node_start_time);
	        puzzle[pos.first][pos.second] = *p;
	        auto temp_domains = updateDomains(domains, pos);
	        if (temp_domains.size() != 0) { 
	        	node_find++;
	            PUZZLE res = ForwardChecking(temp_domains);
	            if (res.size() != 0) return res;
	        }
	        QueryPerformanceCounter(&node_end_time);
            count_time+=(node_end_time.QuadPart-node_start_time.QuadPart)*1000000 
                        / frequency.QuadPart;
	    }
	
	    puzzle[pos.first][pos.second] = 0;
	    return PUZZLE();  
	}

	void printPuzzle(){
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++) printf("%d ",puzzle[i][j]);
			printf("\n");
		}
		puts("======================");
	}

	void FilePrintPuzzle(const string filename) {
    	ofstream fout (filename.c_str());
    	for (int i = 0; i < size; i++) {
    	    for (int j = 0; j < size; j++) fout<<puzzle[i][j]<<" ";
    	    fout<<'\n';
    	}
    	fout<<"=================\n";
    	fout.close();
}
};
int main(){
	QueryPerformanceFrequency(&frequency);
	for(int t = 1; t <= 5; t++){
		node_find = 0;
        LARGE_INTEGER start_time, end_time, frequency;
        bool flag;
        string testcase = "TestCase/data"+to_string(t)+".txt";
        Futoshiki game(testcase);
		game.printPuzzle();
		DOMAIN domains = game.makeDomains();

		flag = QueryPerformanceFrequency(&frequency);
        if(flag) flag = QueryPerformanceCounter(&start_time);//start = clock();

		game.ForwardChecking(domains);

		if(flag) QueryPerformanceCounter(&end_time); //end = clock();
        long long dur_time = (end_time.QuadPart - start_time.QuadPart) * 1000000 / frequency.QuadPart;
        game.printPuzzle();

		string filename = "TestCase/res_fc"+to_string(t)+".txt";
        game.FilePrintPuzzle(filename);
        ofstream fout (filename.c_str());
        
        fout<<"Number of Nodes Searched is "<<node_find<<"\n";
        fout<<"Total Time is "<<dur_time<<" us\n\n";
        fout<<"Average Inference Time Per Node is "<<1.0*count_time/node_find<<" us/node\n\n";
        fout.close();
        printf("Number of Nodes Searched is %lld\n", node_find);
        printf("Total Time is %lld us\n", dur_time);
        printf("Average Inference Time Per Node is %lf us/node\n\n", 1.0*count_time/node_find);
	}
	
	
	return 0;
}