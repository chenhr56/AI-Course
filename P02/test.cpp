#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

int main(){
	int size = 0;
	vector<vector<int>> puzzle;
	ifstream fin("TestCase/data1.txt");
    if(!fin) {
        cerr << "[-] Failed to open file. Check again." << endl;
        exit(-1);
    }
    string firstLine, tmp;
    int con_num = 0;
    getline(fin, firstLine);

    stringstream ss;ss<<firstLine;
    while(ss>>tmp) size++;
       
    fin.clear();fin.seekg(0);

    puzzle.assign(size, vector<int>(size, 0));

    for (int i = 0; i < size; i++)  for (int j = 0; j < size; j++) 
        fin >> puzzle[i][j];

    for (int i = 0; i < size; i++){
    	for (int j = 0; j < size; j++) printf("-%d-",puzzle[i][j]);
    		puts("");
    }  
        
    fin.clear();fin.seekg(0);
    for(int _=1; _<=size; _++) getline(fin, firstLine);
    //ifstream conplace = fin;
    while(getline(fin, firstLine)) con_num++;
    fin.clear();fin.seekg(0);
    for(int _=1; _<=size; _++) getline(fin, firstLine);

    for (int i = 0; i < con_num; i++) {
        int x1, y1, x2, y2;
        fin >> x1 >> y1 >> x2 >> y2;
        printf("-%d-%d-%d-%d-\n",x1,y1,x2,y2);
    }
    fin.close();//conplace.close();

    return 0;
}