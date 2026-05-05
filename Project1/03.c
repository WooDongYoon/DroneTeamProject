#include "cdh.h"

linkedList_h03* createlinkedList_h03(void)
{
	linkedList_h03* L;
	L = (linkedList_h03*)malloc(sizeof(linkedList_h03));
	L->head = NULL;		// 공백 리스트이므로 NULL로 설정
	return L;
}

// 연결 리스트의 전체 메모리를 해제하는 연산
void freeLinkedList_h03(linkedList_h03* L)
{
	listNode03* p;
	while (L->head != NULL) {
		p = L->head;
		L->head = L->head->link;
		free(p);
		p = NULL;
	}
}

// 연결 리스트를 순서대로 출력하는 연산
void printList03(linkedList_h03* L, FILE* fp)
{
	listNode03* p;
	double total_distance = 0;	//지점들의 거리의 합
	double total_battery = 0;
	p = L->head;
	while (p != NULL)
	{
		printf("%-8s %-10.1f %-6.1f %-10.1f\n", p->pointname, p->distance, p->k, p->battery_used);
		fprintf(fp, "%-8s %-10.1f %-6.1f %-10.1f\n\n", p->pointname, p->distance, p->k, p->battery_used);
		total_distance += p->distance;
		total_battery += p->battery_used;
		p = p->link;
	}
	printf("%-8s %-10.1f %-6s %-10.1f\n", "TOTAL", total_distance, "-", total_battery);
}

// 마지막 노드로 삽입하는 연산 
void insertLastNode03(linkedList_h03* L, double distance, double k, double battery_used, char* pointname)
{
	listNode03* newNode;
	listNode03* temp;
	newNode = (listNode03*)malloc(sizeof(listNode03));
	newNode->distance = distance;
	newNode->k = k;
	newNode->battery_used = battery_used;
	strcpy(newNode->pointname, pointname);
	newNode->link = NULL;
	if (L->head == NULL)
	{		// 현재 리스트가 공백인 경우					
		L->head = newNode;		// 새 노드를 리스트의 시작 노드로 연결
		return;
	}
	// 현재 리스트가 공백이 아닌 경우 	
	temp = L->head;
	while (temp->link != NULL) temp = temp->link;	// 현재 리스트의 마지막 노드를 찾음
	temp->link = newNode;							// 새 노드를 마지막 노드(temp)의 다음 노드로 연결 
}

int run03()
{
	//printf("한글 출력 확인\n");
	linkedList_h03* L = createlinkedList_h03();
	/* 유효성 검사 코드 추가 *조동현* 2026.05.05 15:29 */
	if (L == NULL)
	{
		printf("리스트 생성 실패\n");
		return 1;
	}
	FILE* p1;
	FILE* p2;
	p1 = fopen("02.txt", "r");
	p2 = fopen("03.txt", "w");
	if (p1 == NULL || p2 == NULL)
	{
		printf("파일 열기 실패\n");
		return 1;
	}

	d* points = (d*)malloc(sizeof(d) * PointMAX);
	if (p1 == NULL || p2 == NULL)
	{
		printf("File open failed\n");

		if (p1 != NULL)
			fclose(p1);

		if (p2 != NULL)
			fclose(p2);

		freeLinkedList_h03(L);
		free(L);

		return 1;
	}

	char pointname;	//지점 이름을 읽을 변수
	int x, y;
	double battery_used;	//배터리 소모량
	double k;	//배터리 상수 100m 이하 1.5, 100m초과 1.7 
	double distance;	//거리
	char trash[TrashMAX];

	fgets(trash, 50, p1);	//한줄 건너뛰기
	for (int i = 0; i < PointMAX; i++)
	{
		/* 유효성 검사 코드 추가 *조동현* 2026.05.05 16:17 */
		if (fscanf(p1, " %c %d %d", &pointname, &x, &y) != 3) {
			printf("%d번째 좌표 읽기 실패\n", i + 1);
			freeLinkedList_h03(L);
			free(L);
			fclose(p1);
			fclose(p2);
			free(points);

			return 1;
		}
		points[i].x = x;
		points[i].y = y;
		points[i].pointname = pointname;
	}

	for (int i = 0; i < PointMAX - 1; i++)	//distance, k, battery_used 계산해서 연결리스트에 저장
	{
		int x1 = points[i].x;
		int x2 = points[i + 1].x;
		int y1 = points[i].y;
		int y2 = points[i + 1].y;
		char sum_pointname[SMAX];
		for (int j = 0; j < 2; j++)	//지점의 이름을 합치는 코드
		{
			sum_pointname[j] = points[i + j].pointname;	//A,B 지점이면 합쳐서 AB를 저장
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
		/*
		확인용 프롬포트 출력
		2026.05.05 16:20 수정 *조동현*
		*/
		printf("INSERT 전: %s %.1f %.1f %.1f\n", sum_pointname, distance, k, battery_used);
		insertLastNode03(L, distance, k, battery_used, sum_pointname);
	}
	printf("%-8s %-10s %-6s %-10s\n", "구간", "거리", "k", "배터리");
	printList03(L, p2);
	printf("계속 진행하려면 03.txt 파일을 닫으십시오 \n");

	freeLinkedList_h03(L);
	free(L);
	fclose(p1);
	fclose(p2);
	free(points);
	system("notepad.exe 03.txt");
	return 0;
}