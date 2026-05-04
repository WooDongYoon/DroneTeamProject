#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct ListNode 
{
	char pointname[3];
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
void insertLastNode(linkedList_h* L, double distance, double k, double battery_used, char* pointname);
