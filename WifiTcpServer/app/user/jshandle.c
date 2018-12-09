//#include "stdafx.h"//VS ��Ҫ��ͷ�ļ�������Ҫ����ȥ��
#include <stdio.h>
#include <string.h>
#include "jshandle.h"

//only test
#define TESTJSON "{\
room:3,\
light:{\
	light1:1,\
	light2:0,\
},\
curtain:5,\
}"
void jstest()//this is a test func
{

	char value[32] = "";
	get_value((void *)TESTJSON, "light1", value);
	puts(value);
}

//ʹ���෽����Ҫ�ȴ��������� jstest()
void jsErrHandle1()
{
	puts("json error:found key,but value's format error");
}
void jsErrHandle2()
{
	puts("json error:not found key");
}
void jsStrncpy(char *src, const char *dest, int n)
{
	int i;
	if (src == NULL || dest == NULL || n == 0)
	{
		return;
	}
	for (i = 0; i < n; i++) {
		src[i] = dest[i];
	}
}
void get_value(void *src, char *key, char *value)
{
	char key_temp[64] = "";//ָ��ʵ�ִ���ϸ��ӣ�����ָ��key����+1������
	char *find = NULL;

	memset((void *)value, 0, sizeof(value));//insure value empty
	if (src == NULL || key == NULL || value == NULL)
	{
		return;
	}
	strcpy(key_temp, key);
	strcat(key_temp, ":"); //json format flag
	if ((find = strstr((char *)src, key_temp)) != NULL)
	{
		char *find_value = find + strlen(key_temp);
		char *find_comma = NULL;
		if ((find_comma = strstr(find_value, ",")) != NULL)
		{
			jsStrncpy(value, find_value, (find_comma - find_value));//if value not empty, need add '\0'
			if (strstr(value, "{") != NULL || strstr(value, ":") != NULL) {
				memset(value, 0, sizeof(value));
				jsErrHandle1();
			}
		}
		else
		{
			jsErrHandle1();
		}
	}
	else
	{
		jsErrHandle2();
	}
}
