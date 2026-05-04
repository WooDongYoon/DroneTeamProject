#include"LinkedList.h"

// 공백 연결 리스트를 생성하는 연산
linkedList_h* createLinkedList_h(void) 
{
	linkedList_h* L;
	L = (linkedList_h*)malloc(sizeof(linkedList_h));
	L->head = NULL;		// 공백 리스트이므로 NULL로 설정
	return L;
}

// 연결 리스트의 전체 메모리를 해제하는 연산
void freeLinkedList_h(linkedList_h* L)
{
	listNode* p;
	while (L->head != NULL) {
		p = L->head;
		L->head = L->head->link;
		free(p);
		p = NULL;
	}
}

// 연결 리스트를 순서대로 출력하는 연산
void printList(linkedList_h* L, FILE* fp)
{
	listNode* p;
	double total_distance=0;	//지점들의 거리의 합
	double total_battery=0;
	p = L->head;
	while (p != NULL) 
	{
		printf("%s\t\t %.1f\t %.1f\t %.1f\n",p->pointname, p->distance, p->k, p-> battery_used);
		fprintf(fp,"%s\t\t %.1f\t %.1f\t %.1f\n",p->pointname, p->distance, p->k, p->battery_used);
		total_distance += p->distance;
		total_battery += p->battery_used;
		p = p->link;
	}
	printf("TOTAL\t\t %.1f\t -\t %.1f", total_distance, total_battery);
}

// 마지막 노드로 삽입하는 연산 
void insertLastNode(linkedList_h* L, double distance, double k, double battery_used, char* pointname) 
{
	listNode* newNode;
	listNode* temp;
	newNode = (listNode*)malloc(sizeof(listNode));
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
