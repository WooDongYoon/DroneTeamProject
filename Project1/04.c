#include "cdh.h"

stacknode* top = NULL;

//스택 초기화 함수
void clearstack()
{
	stacknode* temp;

	while (top != NULL)
	{
		temp = top;
		top = top->link04;
		free(temp);
	}
}

//push 함수 
void push(route data)
{
	stacknode* newnode;

	newnode = (stacknode*)malloc(sizeof(stacknode));

	newnode->data04 = data;
	newnode->link04 = top;
	top = newnode;
}

//pop 함수
int pop(route* out)
{
	stacknode* temp;

	if (top == NULL)
	{
		return 0;
	}

	temp = top;
	*out = temp->data04;
	top = top->link04;

	free(temp);
	return 1;
}

//라인 만드는 함수
void printLine(FILE* fp2)
{
	printf("------------------------------------------------------------------------------------------------------------\n");
	fprintf(fp2, "------------------------------------------------------------------------------------------------------------\n");
}

//위에 표에 헤더를 만드는 함수
void printHeader(FILE* fp2)
{
	printLine(fp2);

	//이 부분은 간격을 예쁘게 맞추기 위해 chat gpt 사용하였음
	printf("%-12s %-10s %-6s %-15s %-15s %-12s %-12s %-15s\n",
		"구간", "거리", "k", "배터리소모량", "누적소모량", "상태", "이벤트", "안전회귀경로");

	//이 부분은 간격을 예쁘게 맞추기 위해 chat gpt 사용하였음
	fprintf(fp2, "%-12s %-10s %-6s %-15s %-15s %-12s %-12s %-15s\n",
		"구간", "거리", "k", "배터리소모량", "누적소모량", "상태", "이벤트", "안전회귀경로");

	printLine(fp2);
}

//가장 중요한 함수 표의 행, 즉 레코드를 만드는 함수
void printRow(FILE* fp2, route data, char backroute[])
{
	char state[TSIZE];
	char event[TSIZE];

	//상태 판별하는 스위치문 0=노말, 1=실패, 2=리커버리
	switch (data.state04)
	{
	case 0:
		strcpy(state, "NORMAL");
		break;
	case 1:
		strcpy(state, "FAILURE");
		break;
	case 2:
		strcpy(state, "RECOVERY");
		break;
	}

	//이벤트 판별하는 스위치문 0=임무수행, 1=임무실패, 2=역추적
	switch (data.event04)
	{
	case 0:
		strcpy(event, "임무수행");
		break;
	case 1:
		strcpy(event, "임무실패");
		break;
	case 2:
		strcpy(event, "역추적");
		break;
	}

	//이 부분은 간격을 예쁘게 맞추기 위해 chat gpt 사용하였음
	printf("%-12s %-10.1lf %-6.1lf %-15.1lf %-15.1lf %-12s %-12s %-15s\n",
		data.section,
		data.distance04,
		data.k04,
		data.battery04,
		data.total04,
		state,
		event,
		backroute);

	//이 부분은 간격을 예쁘게 맞추기 위해 chat gpt 사용하였음
	fprintf(fp2, "%-12s %-10.1lf %-6.1lf %-15.1lf %-15.1lf %-12s %-12s %-15s\n",
		data.section,
		data.distance04,
		data.k04,
		data.battery04,
		data.total04,
		state,
		event,
		backroute);
}

//04.txt 만드는 실행 함수
int run04()
{
	FILE* fp1;
	FILE* fp2;

	route data;
	route backdata;

	double total = 0;
	int failcheck = 0;

	char backname[BACKSIZE];

	clearstack();

	fp1 = fopen("03.txt", "r");
	fp2 = fopen("04.txt", "a+");

	if (fp1 == NULL)
	{
		printf("03.txt 파일을 찾을 수 없습니다.\n");
		return 1;// int main이 아니라 void run()구조 이기 때문에 0를 반환하면 안됌 그래서 return;이렇게 사용하여 프로그램을 종료한다.
	}

	//날짜와 시간 출력
	printf("\n[데이터 추가 날짜] %s %s\n", __DATE__, __TIME__);

	printHeader(fp2);

	while (fscanf(fp1, "%s %lf %lf %lf", data.section, &data.distance04, &data.k04, &data.battery04) == 4)
	{//fscanf==4로 하나 가져올때 scanf는 1을 반환하므로 4로 4개 가져올때까지 반복되는 조건문 =

		if (total + data.battery04 > 500)//다음 경로가 500이 넘을때
		{
			data.total04 = total;//구조체에 total 저장
			data.state04 = 1;//실패
			data.event04 = 1;//임무 실패

			printRow(fp2, data, "이전경로 역추적");

			failcheck = 1;// 조건문을 위해 실패체크에 1를 넣음
			break;
		}
		else// 총 배터리가 500이 안넘을 때
		{
			total = total + data.battery04;//total에 배터리 더함

			data.total04 = total;//구조체에 total 저장
			data.state04 = 0;//노말
			data.event04 = 0;//임무 수행

			push(data);
			printRow(fp2, data, "-");
		}
	}

	if (failcheck == 1)//다음 경로가 500이 넘어 실패 체크가 1이 되었을 떄
	{
		while (pop(&backdata))//pop 성공하면 1반환 즉 모두 pop할때까지 반복
		{
			strcpy(backname, backdata.section);//구조체에서 이름 받아오기
			strcat(backname, "(회귀)");//뒤에 (회귀) 붙이기

			total = total + backdata.battery04;//베터리 더하기

			backdata.total04 = total;//구조체에 다시 베터리 저장
			backdata.state04 = 2;// 리커버리
			backdata.event04 = 2;// 역추적

			strcpy(backdata.section, backname);//  구간(회귀)이렇게 위에서 붙인걸 다시 구조체에 저장

			printRow(fp2, backdata, backname);// 출력
		}
	}

	printLine(fp2);
	printf("계속 진행하려면 04.txt 파일을 닫으십시오 \n");

	fclose(fp1);
	fclose(fp2);

	system("notepad.exe 04.txt");

	return 0;

}