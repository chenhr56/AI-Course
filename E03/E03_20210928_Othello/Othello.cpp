
#include <iostream>
#include <stdlib.h>

using namespace std;

int const MAX = 65534;

int  deepth = 10;         //����������  ���ɵ��ڣ�

//����Ԫ��   ���ӣ���ɫ�����ֱ���

enum Option
{
	WHITE = -1, SPACE, BLACK	//�Ƿ�������  //����
};

struct Do
{
	pair<int , int > pos;	
	int score;
};

struct WinNum
{	enum Option color;
	int stable;										// �˴�����Ӯ�����
};							





//��Ҫ����    ���̼��������ӵ����в���������
struct Othello														    
{

	WinNum cell[6][6];													//������������6*6������
	int whiteNum;														//������Ŀ
	int blackNum;														//������


	void Create(Othello *board);										//��ʼ������
	void Copy(Othello *boardDest, const  Othello *boardSource);			//��������
	void Show(Othello *board);											//��ʾ����
	int Rule(Othello *board, enum Option player);						//�ж������Ƿ���Ϲ���
	int Action(Othello *board, Do *choice, enum Option player);			//����,���޸�����
	void Stable(Othello *board);										//����Ӯ�����
	int Judge(Othello *board, enum Option player);						//���㱾�����ӷ���
};//��Ҫ����








//�����С�������-�¼�֦ 
Do * Find(Othello *board, enum Option player, int step, int min, int max, Do *choice)
{
	int i, j, k, num;
	Do *allChoices;
	choice->score = -MAX;
	choice->pos.first = -1;
	choice->pos.second = -1;

	num = board->Rule(board, player);
	if (num == 0)    /* �޴����� */
	{
		if (board->Rule(board, (enum Option) - player))    /* �Է���������,�öԷ���.*/
		{
			Othello tempBoard;
			Do nextChoice;
			Do *pNextChoice = &nextChoice;
			board->Copy(&tempBoard, board);
			pNextChoice = Find(&tempBoard, (enum Option) - player, step - 1, -max, -min, pNextChoice);
			choice->score = -pNextChoice->score;
			choice->pos.first = -1;
			choice->pos.second = -1;
			return choice;
		}
		else    /* �Է�Ҳ�޴�����,��Ϸ����. */
		{
			int value = WHITE*(board->whiteNum) + BLACK*(board->blackNum);
			if (player*value>0)
			{
				choice->score = MAX - 1;
			}
			else if (player*value<0)
			{
				choice->score = -MAX + 1;
			}
			else
			{
				choice->score = 0;
			}
			return choice;
		}
	}
	if (step <= 0)    /* �Ѿ����ǵ�step��,ֱ�ӷ��ص÷� */
	{
		choice->score = board->Judge(board, player);
		return choice;
	}

	allChoices = (Do *)malloc(sizeof(Do)*num);
	k = 0;
	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			if (i == 0 || i == 5 || j == 0 || j == 5)
			{
				if (board->cell[i][j].color == SPACE && board->cell[i][j].stable)
				{
					allChoices[k].score = -MAX;
					allChoices[k].pos.first = i;
					allChoices[k].pos.second = j;
					k++;
				}
			}
		}
	}

	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			if ((i == 2 || i == 3 || j == 2 || j == 3) && (i >= 2 && i <= 3 && j >= 2 && j <= 3))
			{
				if (board->cell[i][j].color == SPACE && board->cell[i][j].stable)
				{
					allChoices[k].score = -MAX;
					allChoices[k].pos.first = i;
					allChoices[k].pos.second = j;
					k++;
				}
			}
		}
	}

	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			if ((i == 1 || i == 4 || j == 1 || j == 4) && (i >= 1 && i <= 4 && j >= 1 && j <= 4))
			{
				if (board->cell[i][j].color == SPACE && board->cell[i][j].stable)
				{
					allChoices[k].score = -MAX;
					allChoices[k].pos.first = i;
					allChoices[k].pos.second = j;
					k++;
				}
			}
		}
	}

	for (k = 0; k<num; k++)
	{
		Othello tempBoard;
		Do thisChoice, nextChoice;
		Do *pNextChoice = &nextChoice;
		thisChoice = allChoices[k];
		board->Copy(&tempBoard, board);
		board->Action(&tempBoard, &thisChoice, player);
		pNextChoice = Find(&tempBoard, (enum Option) - player, step - 1, -max, -min, pNextChoice);
		thisChoice.score = -pNextChoice->score;

		if (thisChoice.score>min && thisChoice.score<max)    /* ����Ԥ�Ƶĸ���ֵ */
		{
			min = thisChoice.score;
			choice->score = thisChoice.score;
			choice->pos.first = thisChoice.pos.first;
			choice->pos.second = thisChoice.pos.second;
		}
		else if (thisChoice.score >= max)    /* �õĳ���Ԥ�� */
		{
			choice->score = thisChoice.score;
			choice->pos.first = thisChoice.pos.first;
			choice->pos.second = thisChoice.pos.second;
			break;
		}
		/* ������֪����ֵ */
	}
	free(allChoices);
	return choice;
}

int main()					
{
	Othello board;
	Othello *pBoard = &board;
	enum Option player , present ;
	Do choice;
	Do *pChoice = &choice;
	int  num , result = 0;
	char restart = ' ';

start:						
	player = SPACE;			
	present = BLACK;                  
	num = 4;
	restart = ' ';
	
	cout << ">>>�˻���ս��ʼ�� \n";
	
	
		

		while (player != WHITE && player != BLACK)
		{
			cout << ">>>��ѡ��ִ����(��),��ִ����(��)������1Ϊ���壬-1Ϊ����" << endl;
			scanf("%d", &player);
			cout << ">>>�����ж�:  \n";

			
			if (player != WHITE && player != BLACK)
			{
				cout << "���벻���Ϲ淶������������\n";
			}
		}

		board.Create(pBoard);					

		while (num<36)													// ������δ����36�� 
		{
			char *Player = "";
			if (present == BLACK)
			{
				Player = "����(��)";
			}
			else if (present == WHITE)
			{
				Player = "����(��)";
			}

			if (board.Rule(pBoard, present) == 0)						//δ�����������ӿ���
			{
				if (board.Rule(pBoard, (enum Option) - present) == 0)
				{
					break;                 
				}

				cout << Player << "GAME OVER! \n";
			}
			else
			{
				int i, j;
				board.Show(pBoard);

				if (present == player)     
				{
					while (1)
					{
						cout << Player << " \n >>>�������������꣨�ո���� �硰3 5��������3�е�5�У�:\n";
						
						cin >> i>> j;		
						i--;
						j--;
						pChoice->pos.first = i;
						pChoice->pos.second = j;

						if (i<0 || i>5 || j<0 || j>5 || pBoard->cell[i][j].color != SPACE || pBoard->cell[i][j].stable == 0)
						{
							cout <<">>>�˴����Ӳ����Ϲ���������ѡ��   \n";
							board.Show(pBoard);
						}
						else
						{
							break;
						}
					}
					system("cls");
					cout << ">>>��� ������÷�Ϊ     " << pChoice->score << endl;
					system("pause");
					cout << ">>>�����������" << pChoice->score << endl;
				}
				else   //AI����
				{   
					cout << Player << "..........................";
					
					pChoice = Find(pBoard, present, deepth, -MAX, MAX, pChoice);
					i = pChoice->pos.first;
					j = pChoice->pos.second;
					system("cls");
					cout << ">>>AI ������÷�Ϊ     " << pChoice->score << endl;
				}


				board.Action(pBoard, pChoice, present);
				num++;
				cout << Player << ">>>AI��" << i + 1 << "," << j + 1<<"���ӣ������ˣ�";
			}

			present = (enum Option) - present;    //����ִ����
		}


		board.Show(pBoard);


		result = pBoard->whiteNum - pBoard->blackNum;

		if (result>0)
		{
			cout << "\n����������������(��)ʤ������������\n";
		}
		else if (result<0)
		{
			cout << "\n����������������(��)ʤ������������\n";
		}
		else
		{
			cout << "\n����������������ƽ�֡���������������\n";
		}

		cout << "\n ����������������GAME OVER!����������������\n";
		cout << "\n";
		
		while (restart != 'Y' && restart != 'N')
		{
			cout <<"|������������������������������������������|\n";
			cout <<"|                                          | \n";
			cout <<"|                                          |   \n";
			cout <<"|>>>>>>>>>>>>>>>>Again?(Y,N)<<<<<<<<<<<<<<<|\n";
			cout <<"|                                          | \n";
			cout <<"|                                          |  \n";
			cout <<"|������������������������������������������|\n";
			cout << "                                            \n";
			cout << "                                            \n";
			cout << "                                            \n";
			cout << " ����������                 ����������       \n";
			cout << " |   YES  |                 |   NO   |      \n";
			cout << " ����������                 ����������      \n";

			cin >> restart;
			if (restart == 'Y')
			{
				goto start;
			}
		}

	
	return 0;
}






void Othello::Create(Othello *board)
{
	int i, j;
	board->whiteNum = 2;
	board->blackNum = 2;
	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			board->cell[i][j].color = SPACE;
			board->cell[i][j].stable = 0;
		}
	}
	board->cell[2][2].color = board->cell[3][3].color = WHITE;
	board->cell[2][3].color = board->cell[3][2].color = BLACK;
}


void Othello::Copy(Othello *Fake, const  Othello *Source)
{
	int i, j;
	Fake->whiteNum = Source->whiteNum;
	Fake->blackNum = Source->blackNum;
	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			Fake->cell[i][j].color = Source->cell[i][j].color;
			Fake->cell[i][j].stable = Source->cell[i][j].stable;
		}
	}
}

void Othello::Show(Othello *board)
{
	int i, j;
	cout << "\n  ";
	for (i = 0; i<6; i++)
	{
		cout << "  " << i + 1;
	}
	cout << "\n    ����������������������������������\n";
	for (i = 0; i<6; i++)
	{
		cout << i + 1 << "--��";
		for (j = 0; j<6; j++)
		{
			switch (board->cell[i][j].color)
			{
			case BLACK:
				cout << "��";
				break;
			case WHITE:
				cout << "��";
				break;
			case SPACE:
				if (board->cell[i][j].stable)
				{
					cout << " +��";
				}
				else
				{
					cout << "  ��";
				}
				break;
			default:    /* ������ɫ���� */
				cout << "* ��";
			}
		}
		cout << "\n    ����������������������������������\n";
	}

	cout << ">>>����(��)����Ϊ:" << board->whiteNum << "         ";
	cout << ">>>����(��)����Ϊ:" << board->blackNum << endl << endl << endl;
}


int Othello::Rule(Othello *board, enum Option player)
{
	int i, j;
	unsigned num = 0;
	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			if (board->cell[i][j].color == SPACE)
			{
				int x, y;
				board->cell[i][j].stable = 0;
				for (x = -1; x <= 1; x++)
				{
					for (y = -1; y <= 1; y++)
					{
						if (x || y)    /* 8������ */
						{
							int i2, j2;
							unsigned num2 = 0;
							for (i2 = i + x, j2 = j + y; i2 >= 0 && i2 <= 5 && j2 >= 0 && j2 <= 5; i2 += x, j2 += y)
							{
								if (board->cell[i2][j2].color == (enum Option) - player)
								{
									num2++;
								}
								else if (board->cell[i2][j2].color == player)
								{
									board->cell[i][j].stable += player*num2;
									break;
								}
								else if (board->cell[i2][j2].color == SPACE)
								{
									break;
								}
							}
						}
					}
				}

				if (board->cell[i][j].stable)
				{
					num++;
				}
			}
		}
	}
	return num;
}


int Othello::Action(Othello *board, Do *choice, enum Option player)
{
	int i = choice->pos.first, j = choice->pos.second;
	int x, y;

	if (board->cell[i][j].color != SPACE || board->cell[i][j].stable == 0 || player == SPACE)
	{
		return -1;
	}


	board->cell[i][j].color = player;
	board->cell[i][j].stable = 0;


	if (player == WHITE)
	{
		board->whiteNum++;
	}
	else if (player == BLACK)
	{
		board->blackNum++;
	}



	for (x = -1; x <= 1; x++)
	{
		for (y = -1; y <= 1; y++)
		{

			//��Ҫ��ÿ������8�����ϼ�������Ƿ���Ϲ����ܷ���ӣ�


			if (x || y)
			{
				int i2, j2;
				unsigned num = 0;
				for (i2 = i + x, j2 = j + y; i2 >= 0 && i2 <= 5 && j2 >= 0 && j2 <= 5; i2 += x, j2 += y)
				{
					if (board->cell[i2][j2].color == (enum Option) - player)
					{
						num++;
					}
					else if (board->cell[i2][j2].color == player)
					{
						board->whiteNum += (player*WHITE)*num;
						board->blackNum += (player*BLACK)*num;

						for (i2 -= x, j2 -= y; num>0; num--, i2 -= x, j2 -= y)
						{
							board->cell[i2][j2].color = player;
							board->cell[i2][j2].stable = 0;
						}
						break;
					}
					else if (board->cell[i2][j2].color == SPACE)
					{
						break;
					}
				}
			}
		}
	}
	return 0;
}


void Othello::Stable(Othello *board)
{
	int i, j;
	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			if (board->cell[i][j].color != SPACE)
			{
				int x, y;
				board->cell[i][j].stable = 1;

				for (x = -1; x <= 1; x++)
				{
					for (y = -1; y <= 1; y++)
					{
						/* 4������ */
						if (x == 0 && y == 0)
						{
							x = 2;
							y = 2;
						}
						else
						{
							int i2, j2, flag = 2;
							for (i2 = i + x, j2 = j + y; i2 >= 0 && i2 <= 5 && j2 >= 0 && j2 <= 5; i2 += x, j2 += y)
							{
								if (board->cell[i2][j2].color != board->cell[i][j].color)
								{
									flag--;
									break;
								}
							}

							for (i2 = i - x, j2 = j - y; i2 >= 0 && i2 <= 5 && j2 >= 0 && j2 <= 5; i2 -= x, j2 -= y)
							{
								if (board->cell[i2][j2].color != board->cell[i][j].color)
								{
									flag--;
									break;
								}
							}

							if (flag)    /* ��ĳһ�������ȶ� */
							{
								board->cell[i][j].stable++;
							}
						}
					}
				}
			}
		}
	}
}

int Othello::Judge(Othello *board, enum Option player)
{
	/*int value = 0;
	int i, j;
	Stable(board);
	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			value += (board->cell[i][j].color)*(board->cell[i][j].stable);
		}
	}

	value += 64 * board->cell[0][0].color;
	value += 64 * board->cell[0][5].color;
	value += 64 * board->cell[5][0].color;
	value += 64 * board->cell[5][5].color;
	value -= 32 * board->cell[1][1].color;
	value -= 32 * board->cell[1][4].color;
	value -= 32 * board->cell[4][1].color;
	value -= 32 * board->cell[4][4].color;

	return value*player;*/
	int value=0;
	Stable(board);

	int x = 0, y = 0, i = 0, j = 0, k = 0,
		my_tiles = 0, opp_tiles = 0, 
		my_front_tiles = 0, opp_front_tiles = 0;

	double p = 0, m = 0, f = 0, d = 0;

	int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};
	int V[6][6] = {
					{15, 2, 6, 6, 3, 15},
					{2, -3, 0, 0, -3, 2},
					{6, 0, -3, -3, 0, 6},
					{6, 0, -3, -3, 0, 6},
					{2, -3, 0, 0, -3, 2},
					{15, 2, 6, 6, 3, 15},
				  };
/*
		V[0] = {20, -3, 11, 8, 8, 11, -3, 20};
    	V[1] = {-3, -7, -4, 1, 1, -4, -7, -3};
    	V[2] = {11, -4, 2, 2, 2, 2, -4, 11};
    	V[3] = {8, 1, 2, -3, -3, 2, 1, 8};
    	V[4] = {8, 1, 2, -3, -3, 2, 1, 8};
    	V[5] = {11, -4, 2, 2, 2, 2, -4, 11};
    	V[6] = {-3, -7, -4, 1, 1, -4, -7, -3};
    	V[7] = {20, -3, 11, 8, 8, 11, -3, 20};

		V[0] = {15, 2, 6, 6, 3, 15};
		V[1] = {2, -3, 0, 0, -3, 2};
		V[2] = {6, 0, -3, -3, 0, 6};
		V[3] = {6, 0, -3, -3, 0, 6};
		V[4] = {2, -3, 0, 0, -3, 2};
		V[5] = {15, 2, 6, 6, 3, 15};
*/
	for(i = 0; i < 6; i++)
		for(j = 0; j < 6; j++)  {
			if(board->cell[i][j].color == player)  {
				d += V[i][j];
				my_tiles++;
			} else if(board->cell[i][j].color == (enum Option) - player)  {
				d -= V[i][j];
				opp_tiles++;
			}
			if(board->cell[i][j].color != SPACE)   {
				for(k=0; k<8; k++)  {
					x = i + X1[k]; y = j + Y1[k];
					if(x >= 0 && x < 6 && y >= 0 && y < 6 &&
					   board->cell[x][y].color == SPACE) {
						if(board->cell[i][j].color == player)  my_front_tiles++;
						else opp_front_tiles++;
						break;
					}
				}
			}
		}
	if(my_tiles > opp_tiles)
		p = (100.0 * my_tiles) / (my_tiles + opp_tiles);
	else if(my_tiles < opp_tiles)
		p = -(100.0 * opp_tiles) / (my_tiles + opp_tiles);
	else p = 0;

	if(my_front_tiles > opp_front_tiles)
		f = -(100.0 * my_front_tiles) / (my_front_tiles + opp_front_tiles);
	else if(my_front_tiles < opp_front_tiles)
		f = (100.0 * opp_front_tiles) / (my_front_tiles + opp_front_tiles);
	else f = 0;

	my_tiles = Rule(board, player);
	opp_tiles = Rule(board, (enum Option) - player);
	if(my_tiles > opp_tiles)
		m = (1.0 * 75 * my_tiles)/(my_tiles + opp_tiles);//75=100*6/8
	else if(my_tiles < opp_tiles)
		m = -(1.0 * 75 * opp_tiles)/(my_tiles + opp_tiles);
	else m = 0;

	value = (10 * p) + (78.922 * m) + (74.396 * f) + (10 * d);
	return (int)value;
}
