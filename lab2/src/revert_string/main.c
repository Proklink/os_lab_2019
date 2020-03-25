#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "revert_string.h"

int main(int argc, char *argv[])
{
 //printf("%s\n",argv[1]);
 //printf("%s\n",argv[2]);
	if (argc != 2)//если пришло не 2 параметра, то программа завершается с кодом -1
	{
		printf("Usage: %s string_to_revert\n", argv[0]);
		return -1;
	}

	char *reverted_str = malloc(sizeof(char) * (strlen(argv[1]) + 1));//выделяется память под второй параметр
	strcpy(reverted_str, argv[1]);//значение из argvпереносится в собственный массив

	RevertString(reverted_str);//вызывается функция для переворачивания

	printf("Reverted: %s\n", reverted_str);//выводится результат
	free(reverted_str);//освобождается память
	return 0;
}

