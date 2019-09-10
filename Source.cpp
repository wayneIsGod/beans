#include<stdio.h>
#include<stdlib.h>
#include"player.h"
#define MAX_INT 2147483647;
#define MIN_INT -2147483647;
void minValue(int house[], int depth, int depthMAX, int *mValue, int *action,const int N, int alpha, int beta);
void print(int house[],const int N)
{
	printf("{");
	for(int i=0;i<N/2-1;i++)
	{
		printf("%d ",house[i]);
	}
	printf("[%d] ",house[N/2-1]);
	for(int i=N/2;i<N-1;i++)
	{
		printf("%d ",house[i]);
	}
	printf("[%d]} \n",house[N-1]);
}
bool IsFinal(int* house)
{
	int pass=0;
	for(int j=0;j<=6;++j)
	{
		if(house[j]!=0&&j!=6)
		{
			pass=0;
			break;
		}
		if(j==6)    
		{    
			return 1;
		}    
	}
	for(int j=7;j<=13;++j)
	{
		if(house[j]!=0&&j!=13)
		{
			pass=0;
			break;
		}
		if(j==13)
		{
			return 1;
		}
	}
	if(pass==0)
	{
		return 0;
	}
}
void finalScoring(int house[])
{
	for (int i = 6; i < 14; i+=7) 
	{
		for (int j = i - 6; j < i; ++j)
		{
			house[i] += house[j];
			house[j] = 0;
		}
	}		
}
bool relocation(int house[], int pickedHouse,const int N)
{
	int bean = house[pickedHouse], ownStorage, enemyStorage;
	if (pickedHouse < N/2-1) 
	{
		enemyStorage = N-1;
		ownStorage = N/2-1;
	}
	else 
	{
		ownStorage = N-1;
		enemyStorage = N/2-1;
	}
	house[pickedHouse] = 0;
	while (bean != 0) 
	{
		if (++pickedHouse == N)
			pickedHouse = 0;
		if (pickedHouse == enemyStorage)
			continue;
		house[pickedHouse] += 1;
		--bean;
	}
	if (house[pickedHouse] == 1 && pickedHouse != ownStorage && (ownStorage == N/2-1 ? pickedHouse < N/2-1 : pickedHouse>N/2-1))
	{
		int enemyPartner = (ownStorage * 2 - pickedHouse) % N;
		if (house[enemyPartner] > 0)
		{
			house[pickedHouse] = 0;
			house[ownStorage] += 1 + house[enemyPartner];
			house[enemyPartner] = 0;
		}
	}
	return pickedHouse == ownStorage;
}
int evaluate(int house[],const int N)
{
	return house[N-1] - house[N/2-1];
}
void finalScoring(int house[],const int N)
{
	for (int i = N/2-1; i < N; i+=N/2) 
	{
		for (int j = i - (N/2-1); j < i; ++j)
		{
			house[i] += house[j];
			house[j] = 0;
		}
	}		
}
int listSuccessors(int house[],int actions[],int start)
{
	int count=0;
	for (int i = start; i < start+6; ++i)
	{
		if (house[i] != 0) 
		{
			actions[count] = i;
			++count;
		}		
	}
	return count;
}

void maxValue(int house[], int depth, int depthMAX, int *mValue, int *action,const int N , int alpha, int beta)
{
	if (depth >= depthMAX)
	{
		*mValue = evaluate(house,N);
		return;
	}
	int* nextActions=(int*)malloc(sizeof(int)*(N/2-1));
	int numberOfSuccessor = listSuccessors(house, nextActions,7);
	if (numberOfSuccessor == 0)
	{
		finalScoring(house,N);
		*mValue = evaluate(house,N);
		return;
	}
	int* successorHouse=(int*)malloc(sizeof(int)*N);
	int sAcction,sMValue,maxvAction,maxv = MIN_INT;	
	for (int i = 0; i < numberOfSuccessor; ++i)	
	{
		for (int j = 0; j < 14; ++j)
			successorHouse[j] = house[j];
		if (relocation(successorHouse, nextActions[i], N))
			maxValue(successorHouse, depth + 2, depthMAX, &sMValue, &sAcction, N, alpha, beta);		
		else
			minValue(successorHouse, depth + 1, depthMAX, &sMValue, &sAcction, N, alpha, beta);		
		if (maxv<sMValue) 
		{
			maxv = sMValue;
			maxvAction = nextActions[i];
		}
		if(alpha<sMValue)
		{
			alpha= sMValue;
		}
		if(alpha >= beta)
		{
			break;
		}
	}
	delete[] nextActions;
	delete[] successorHouse;
	*mValue = alpha; 
	*action = maxvAction;
}
void minValue(int house[], int depth, int depthMAX, int *mValue, int *action,const int N, int alpha, int beta)
{
	if (depth >= depthMAX)
	{
		*mValue = evaluate(house,N);
		return;
	}
	int* nextActions=(int*)malloc(sizeof(int)*(N/2-1));
	int numberOfSuccessor = listSuccessors(house,nextActions,0);
	if (numberOfSuccessor == 0) 
	{
		finalScoring(house,N);
		*mValue = evaluate(house,N);
		return;
	}
	int* successorHouse=(int*)malloc(sizeof(int)*N);
	int sAcction, sMValue, minvAction, minv = MAX_INT;
	for (int i = 0; i < numberOfSuccessor; ++i) 
	{
		for (int j = 0; j < 14; ++j)
			successorHouse[j] = house[j];
		if (relocation(successorHouse, nextActions[i], N))
			minValue(successorHouse, depth + 2, depthMAX, &sMValue, &sAcction, N, alpha, beta);
		else
			maxValue(successorHouse, depth + 1, depthMAX, &sMValue, &sAcction, N, alpha, beta);
		if (minv>sMValue)
		{
			minv = sMValue;
			minvAction = nextActions[i];
		}
		if (beta > sMValue)
		{
			beta = sMValue;
		}
		if (alpha >= beta)
		{
			break;
		}
	}
	delete[] nextActions;
	delete[] successorHouse;
	*mValue = beta; 
	*action = minvAction; 
}
int minimaxDecision(int house[], int depthMAX,const int N,bool IsFast)
{
	int mValue, action;
	int alpha=INT_MIN; int beta=INT_MAX;
	if(IsFast==0)
	{
		maxValue(house, 0, depthMAX, &mValue,&action,N, alpha, beta);
	}
	else
	{
		minValue(house, 0, depthMAX, &mValue,&action,N, alpha, beta);
	}
	return action;
}
int main()
{
	int numberOfTData;
	const int N=14;
	char name1[100],name2[100];
	player* players[2];
	int* house=(int*)malloc(sizeof(int)*N);
	int depthMax;
	int IsComputer,next,final=0;
	printf("玩家1是?(0)人類 (1)電腦:");
	scanf("%d",&IsComputer);
	printf("玩家1名字:");
	scanf("%s",name1);
	players[0]=new player(1,IsComputer,name1);
	//player player0(1,IsComputer,name1);
	printf("玩家2是?(0)人類 (1)電腦:");
	scanf("%d",&IsComputer);
	printf("玩家2名字:");
	scanf("%s",name2);
	players[1]=new player(0,IsComputer,name2);
	//player player1(0,IsComputer,name2);
	
	int playerIndex=0;
	for(int i=0;i<N;i++)
	{
		house[i]=6;
	}
	house[N/2-1]=0;house[N-1]=0;
	print(house,N);
		printf("層數:");
		scanf("%d",&depthMax);
		while(IsFinal(house)==0)
		{
			bool again;
			
			if(players[playerIndex]->get_IsComputer())
			{
				next=minimaxDecision(house, depthMax,N,1);
				printf("%s輸入下一步:%d\n",players[playerIndex]->get_name(),next);
				again=relocation(house,next,N);
			}
			else
			{
				printf("%s輸入下一步:",players[playerIndex]->get_name());
				scanf("%d",&next);
				again=relocation(house,next,N);
			}
			
			print(house,N);
			if(again)
				continue;
			playerIndex+=1;
			if(playerIndex==2)
				playerIndex=0;
		}
	finalScoring(house);
	print(house,N);
	if(evaluate(house,N)<0)
	{
		printf("%s贏了!!!\n",players[0]->get_name());
	}
	else if(evaluate(house,N)>0)
	{
		printf("%s贏了!!!\n",players[1]->get_name());
	}
	else
	{
		printf("平手!!!\n");
	}
	system("pause");
	return 0;
}