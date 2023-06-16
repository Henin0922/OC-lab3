#include <stdlib.h>
#include <stdio.h>
#include "Threadstatus.h"
#include <Windows.h>

#pragma warning(disable:4996)

int* array;
int arraySize;
enum threadStatus* threadStatuses;//线程状态
int threadCount;//线程数

CRITICAL_SECTION section;

void WINAPI marker(LPVOID params);

int CheckAllThreads()//检查所有线程
{
	for (int i = 0; i < threadCount; ++i)
	{
		if (threadStatuses[i] == active)//线程状态 == 积极
		{
			return 0;
		}
	}

	return 1;
}

int main()
{
	HANDLE* threads;
	InitializeCriticalSection(&section);//初始化关键部分

	printf("写入数组大小 Запись в размер массива\n");
	scanf("%d", &arraySize);

	array = (int*)calloc(arraySize, sizeof(int));

	for (int i = 0; i < arraySize; ++i)
	{
		array[i] = 0;
	}

	printf("记录流的数量 Количество потоков записи\n");
	scanf("%d", &threadCount);

	threads = (HANDLE*)calloc(threadCount, sizeof(HANDLE));
	threadStatuses = (enum threadStatus*)calloc(threadCount, sizeof(threadStatuses));

	for (int i = 0; i < threadCount; ++i)//creating threads创建线程
	{
		int a = i + 1;
		threadStatuses[i] = paused;//暂停
		threads[i] = CreateThread(
			NULL,
			0,
			marker,
			(void*)a,
			0,
			NULL);
	}

	for (int i = 0; i < threadCount; ++i)
	{
		threadStatuses[i] = active;
	}

	while (2 + 2 == 4)
	{
		while (CheckAllThreads() != 1) {}

		printf("排列: Массив: ");
		for (int i = 0; i < arraySize; ++i)
		{
			printf("%d ", array[i]);
		}
		printf("\n输入标记数字，完成其工作 Введите номера маркеров для завершения работы\n");
		int index = 0;
		scanf("%i", &index);

		threadStatuses[index - 1] = finishing;//结束
		while (threadStatuses[index - 1] != finished) {}
		printf("阵列已被改变：Массив изменен: ");
		for (int i = 0; i < arraySize; ++i)
		{
			printf("%d ", array[i]);
		}

		for (int i = 0; i < threadCount; ++i)
		{
			if (threadStatuses[i] == paused) {
				threadStatuses[i] = active;
			}
		}

		int allFinished = 1;
		for (int i = 0; i < threadCount; ++i)
		{
			if (threadStatuses[i] != finished)
			{
				allFinished = 0;
				break;
			}
		}


		if (allFinished)
		{
			break;
		}

	}
	DeleteCriticalSection(&section);//删除关键部分

}