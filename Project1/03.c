#include"LinkedList.h"
#define PointMAX 5	// 지점 개수
#define TrashMAX 50	//필요 없는 줄을 저장 시킬 휴지통 역할 배열 크기

typedef struct Distance	//지점간 거리를 구할 때 사용할 구조체 배열
{
	char pointname;	//지점 이름
	int x;
	int y;
}d;

int main()
{
	linkedList_h* L = createLinkedList_h();
	FILE* p1;
	FILE* p2;
	p1 = fopen("02.txt", "r");
	p2 = fopen("03.txt", "w");
	if (p1 == NULL || p2 == NULL)
	{
		printf("파일 열기 오류\n");
		return 0;
	}
	
	d* points = (d*)malloc(sizeof(d) * PointMAX);

	char pointname;	//지점 이름을 읽을 변수
	int x, y;
	double battery_used;	//배터리 소모량
	double k;	//배터리 상수 100m 이하 1.5, 100m초과 1.7 
	double distance;	//거리
	char trash[TrashMAX];

	fgets(trash, 50, p1);	//한줄 건너뛰기
	for (int i = 0; i < PointMAX; i++)
	{
		fscanf(p1," %c %d %d",&pointname, &x, &y);
		points[i].x = x;
		points[i].y = y;
		points[i].pointname = pointname;
	}

	for (int i = 0; i < PointMAX-1; i++)	//distance, k, battery_used 계산해서 연결리스트에 저장
	{
		int x1 = points[i].x;
		int x2 = points[i + 1].x;
		int y1 = points[i].y;
		int y2 = points[i + 1].y;
		char sum_pointname[3];
		for (int j = 0; j < 2; j++)	//지점의 이름을 합치는 코드
		{
			sum_pointname[j] = points[i+j].pointname;	//A,B 지점이면 합쳐서 AB를 저장
		}
		sum_pointname[2] = '\0';
		distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));	//두 지점 사이의 거리구하는 공식(유클리디안 거리)
		if (distance <= 100)
		{
			k = 1.5;
		}
		else
		{
			k = 1.7;
		}
		battery_used = k * distance;
		insertLastNode(L, distance, k, battery_used, sum_pointname);
	}
	printf("구간\t 거리(m)\t k\t 배터리 소모량\t\n");
	printList(L, p2);

	freeLinkedList_h(L);
	fclose(p1);
	fclose(p2);
	free(points);
	system("notepad.exe 03.txt");
}