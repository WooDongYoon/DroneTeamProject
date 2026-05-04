#pragma once
// 단순 연결 리스트의 노드 구조를 구조체로 정의
typedef struct ListNode 
{
	double distance;
	double k;
	double battery_used;
	struct ListNode* link;
} listNode;

// 리스트의 시작을 나타내는 head 노드를 구조체로 정의
typedef struct 
{
	listNode* head;
} linkedList_h;

linkedList_h* createLinkedList_h(void);
void freeLinkedList_h(linkedList_h* L);
void printList(linkedList_h* L, FILE* fp);
void insertLastNode(linkedList_h* L, double distnce, double k, double battery_used);
