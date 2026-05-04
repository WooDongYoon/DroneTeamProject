#include "cdh.h"

typedef struct dronePosition
{
	int x;
	int y;
} DP;

int main()
{
	FILE* fp = fopen("01.txt", "w");
	if (fp == NULL)
	{
		printf("파일 생성 실패\n");
		return 1;
	}

	DP* p;
	p = (DP*)malloc(sizeof(DP) * MAX);

	if (p == NULL)
	{
		printf("메모리 할당 실패\n");
		fclose(fp);
		return 1;
	}
	printf("지휘통제실에서 좌표를 전파합니다!!\n");
	srand((unsigned int)time(NULL));

	int x1 = 0, y1 = 0;
	int count = 0;

	while (count < MAX)
	{
		int x = rand() % 101;
		int y = rand() % 101;
		int samePosition = 0;  // 중복일 경우를 제외하기 위한 변수 samePosition

		if (x == x1 && y == y1)
		{
			samePosition = 1;	// 왜 같은 경우 1이냐 같다면 true 1, 다르다면 false 0 으로 구분하기 위함
		}

		for (int i = 0; i < count; i++)
		{
			if ((p + i)->x == x && (p + i)->y == y)
			{
				samePosition = 1;
				break;
			}
		}

		if (samePosition == 0)		// 다른 경우이므로 이는 저장할 변수임
		{
			(p + count)->x = x;		//
			(p + count)->y = y;
			count++;				// count변수는 횟수가 아닌 중복이 아닌것을 성공적으로 저장한 좌표 개수임!
		}
	}

	printf("%d %d\n", x1, y1);
	fprintf(fp, "%d %d\n", x1, y1);

	for (int i = 0; i < MAX; i++)
	{
		printf("%d %d\n", (p + i)->x, (p + i)->y);
		fprintf(fp, "%d %d\n", (p + i)->x, (p + i)->y);
	}

	free(p);
	fclose(fp);

	system("notepad.exe 01.txt");
	getchar();

	return 0;
}