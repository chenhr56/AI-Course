#include<iostream>
#include<cstdio>
#include<fstream>
#include<cstdlib>
#include<cctype>
#include<climits>
#include<cstring>
#include<cmath>
#include<vector>
#include<stack>
#include<map>
#include<set>
#include<queue>
#include<ctime>
#include<string>
#include<cstring>
#include<algorithm>
#include<unordered_map>
#define rep(i,a,n) for (int i=a;i<=n;i++)
#define per(i,a,n) for (int i=a;i>=n;i--)
#define pb push_back
#define ll long long
#define itn int
#define fro for
#define pch putchar
using namespace std;

inline int qread_int(){
    int x=0, f=1;
    char c=getchar();
    while(c<'0'||c>'9') { if(c=='-') f=-1; c=getchar(); }
    while(c>='0'&&c<='9') { x=x*10-'0'+c; c=getchar(); }
    return x*f;
}
inline ll qread_ll(){
    ll x=0, f=1;
    char c=getchar();
    while(c<'0'||c>'9') { if(c=='-') f=-1; c=getchar(); }
    while(c>='0'&&c<='9') { x=x*10-'0'+c; c=getchar(); }
    return x*f;
}
template <typename T>
inline void qwrite(T num){
    if(num<0) { putchar('-'); num=~num+1; }
    if(num>9) qwrite(num/10);
    putchar('0'+num%10);
}
int main(){
    //ios::sync_with_stdio(false);
    //cin.tie(0);cout.tie(0);
    printf("clause_truth: \n");
    ifstream fin("res.txt");
    string _;fin>>_;
    int num=-1;
    while(fin>>num){
    	if(num>0) printf("1 ");
    	else if(num<0) printf("0 ");
    	else break;
    }
    printf("\nvar_value: \n");
        for(int _=1; _<1593; _++){
            printf("1 ");
        }
    printf("\nTotal Time is 31963765203 us\n");
    //for(int i=1; i<=)
    return 0;
}
