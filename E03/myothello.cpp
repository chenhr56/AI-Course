#include <stdio.h>
struct State
{
	int cnt[2];
	char a[15][15];
} now;
int work(int r, int c, char x, int fill){
	if (now.a[r][c] == 'B' || now.a[r][c] == 'W')
		return 0;
	char y = x == 'B' ? 'W' : 'B', ok = 0;
	for (int i = 0, dr[8] = {1, 1, 1, 0, 0, -1, -1, -1}, dc[8] = {1, 0, -1, 1, -1, 1, 0, -1}; i < 8; ++i)
		for (int rr = r + dr[i], cc = c + dc[i]; now.a[rr][cc] == y;)
			if (now.a[rr += dr[i]][cc += dc[i]] == x)
			{
				if (!fill)
					return 1;
				for (ok = 1, --now.cnt[x == 'W'], ++now.cnt[x == 'B']; rr != r || cc != c; rr -= dr[i], cc -= dc[i])
					now.a[rr][cc] = x, ++now.cnt[x == 'W'], --now.cnt[x == 'B'];
				break;
			}
	if (ok)
		now.a[r][c] = x, ++now.cnt[x == 'W'];
	return ok;
}
void showTable()
{
	for (int i = 1; now.a[i][1]; ++i)
		puts(now.a[i] + 1);
}
int dfs(char x, int dep, int alpha, int beta, int *ansr, int *ansc)
{
	*ansr = -1, *ansc = -1;
	if (!dep) //局面评估函数，这里是自己的颜色减去对手的颜色数
		return now.cnt[x == 'W'] - now.cnt[x == 'B'];
	int ok = 0;
	struct State tp = now; // 备份当前状态
	for (int i = 1; now.a[i][1]; ++i)
		for (int j = 1; now.a[i][j]; ++j)
			if (work(i, j, x, 1)) //如果这个位置可以下（填充进去）
			{
				ok = 1;
				int tmpr, tmpc, tmp = -dfs(x == 'B' ? 'W' : 'B', dep - 1, -beta, -alpha, &tmpr, &tmpc);
				now = tp; //（回溯，恢复原状态）
				if (alpha < tmp)
				{
					*ansr = i, *ansc = j, alpha = tmp;
					if (alpha >= beta) // 可以减枝
						break;
				}
			}
	if (!ok) //无可下子，丢给对手决定
	{
		int tmpr, tmpc;
		return -dfs(x == 'B' ? 'W' : 'B', dep - 1, -beta, -alpha, &tmpr, &tmpc);
	}
	return alpha;
}
int main(){
	int t, n = 6;
	for (scanf("%d", &t); t--; printf(t ? "\n" : "")){
		for (int i = 1; i <= n; ++i){
			scanf("%s", now.a[i] + 1);
			for (int j = 1; now.a[i][j]; ++j){
				if (now.a[i][j] == 'B') ++now.cnt[0];
				if (now.a[i][j] == 'W') ++now.cnt[1];
			}
		}
		char cur, cmd[9];
		for (scanf("%s", cmd), cur = cmd[0]; scanf("%s", cmd) != EOF;){
			if (cmd[0] == 'L'){
				int cnt = 0;
				for (int i = 1; i <= n; ++i)
					for (int j = 1; j <= n; ++j)
						if (work(i, j, cur, 0))
							printf("%s(%d,%d)", cnt++ ? " " : "", i, j);
				printf(cnt ? "\n" : "No legal move.\n");
			}
			else if (cmd[0] == 'M'){
				int r = cmd[1] - '0', c = cmd[2] - '0';
				if (!work(r, c, cur, 0))
					cur = cur == 'B' ? 'W' : 'B';
				work(r, c, cur, 1);
				cur = cur == 'B' ? 'W' : 'B';
				printf("Black - %2d White - %2d\n", now.cnt[0], now.cnt[1]);
			}
			else if (cmd[0] == 'Q'){
				showTable();
				break;
			}
			else if (cmd[0] == 'S'){
				printf("Black - %2d White - %2d\n", now.cnt[0], now.cnt[1]);
				printf("Cur - %c\n", cur);
				showTable();
			}
			else if (cmd[0] == 'A'){
				int r, c;
				dfs(cur, 10, -1e9, 1e9, &r, &c);
				if (r > 0 && c > 0)
					work(r, c, cur, 1);
				cur = cur == 'B' ? 'W' : 'B';
			}
		}
	}
}
/*
1
------
------
--WB--
--BW--
------
------
W
A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S A S
*/