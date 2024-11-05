// demo 1   文件输入：文件操作的读取
#define _CRT_SECURE_NO_WARNINGS /// 使用类scanf printf这样的函数需要在第一行包含该语句
#define BUFLEN 256
#define MAX_GRAM 10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;
struct Grammar
{
	char nonTerminals[BUFLEN]; // 非终结符列表
	char terminals[BUFLEN];    // 终结符列表
	char productions[BUFLEN];  // 推导规则
	char startSymbol;          // 起始符号
};
struct Gram
{
	char str[BUFLEN]; // 产生式字符串
	string lstr;
	string rstr;
	int ll;     // 左边长度
	int rl;     // 右边长度
};
int main()
{
	char lineBuf[BUFLEN];
	FILE* source;
	const char* pgm = "case1.txt"; /// test.txt文件需要和该代码源文件在同一个目录，注意查看是否隐藏了文件类型，避免文件为test.txt.txt
	source = fopen(pgm, "r");      ////获得文件指针，"r"表示只读方式打开

	struct Grammar grammar; // 用于存储当前的文法信息
	int lineCount = 0;      // 行计数器，用于判断读取到哪部分
	Gram gram[BUFLEN];
	int i = 0;              // 产生式计数

	while (1)
	{

		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			/// 逐行读入文件内容，每次读入的内容都存放在lineBuf中

			lineBuf[strcspn(lineBuf, "\r\n")] = 0;
			printf("%s\n", lineBuf);
			// 如果遇到空行，跳过
			if (strcmp(lineBuf, "") == 0)
			{
				lineCount = 0; // 每个文法结束，重置计数器
				i = 0;
				continue;
			}
			// 根据行数来处理不同部分
			switch (lineCount % 4)
			{
			case 0: // 第一行：非终结符列表
				strcpy(grammar.nonTerminals, lineBuf);
				break;
			case 1: // 第二行：终结符列表
				strcpy(grammar.terminals, lineBuf);
				break;
			case 2: // 第三行：推导规则
				strcpy(grammar.productions, lineBuf);

				break;
			case 3: // 第四行：起始符号
				grammar.startSymbol = lineBuf[0];
				// 输出当前文法信息，作为验证
				printf("非终结符: %s\n", grammar.nonTerminals);
				printf("终结符: %s\n", grammar.terminals);
				printf("推导规则: %s\n", grammar.productions);
				printf("起始符号: %c\n\n", grammar.startSymbol);
				break;
			}
			lineCount++;
			//文法类型判断
			if (lineCount == 4)
			{
				char* ptr = NULL;
				int i = 0; //用于计有效的产生式数量
				char* token = strtok_s(grammar.productions, ",", &ptr);//把所有产生式分解
				while (token != NULL && i < MAX_GRAM) //进一步分解含|的
				{
					char* ptr1 = NULL;
					//获取产生式的左侧非终结符部分
					char lSide[BUFLEN];
					strncpy(lSide, token, strchr(token, '-') - token);//复制左侧
					lSide[strchr(token, '-') - token] = '\0'; //添加字符串结束符
					//处理右侧部分，分割|
					char* rSide = strchr(token, '-') + 2; // 跳过 "A->" 部分
					char* token1 = strtok_s(rSide, "|", &ptr1);

					while (token1 != NULL && i < MAX_GRAM) 
					{
						sprintf(gram[i].str, "%s->%s", lSide, token1);//组合左右侧
						i++; // 计数器加一
						token1 = strtok_s(NULL, "|", &ptr1); //继续分割下一个
					}
					token = strtok_s(NULL, ",", &ptr); // 继续分割下一个产生式
				}
				int num = i; // 有效的产生式数量
				int flag = 0, flag1 = 1, flag2 = 1, flagl = 1, flagr = 1;
				int j;
				for (i = 0; i < num; i++)
				{
					int len = strlen(gram[i].str);
					// 找到‘-’的位置
					for (j = 0; j < len; j++)
					{
						if (gram[i].str[j] == '-')

							break;
					}
					gram[i].ll = j;
					gram[i].rl = len - j - 2;
				}
				//1型文法判断
				for (i = 0; i < num; i++)
				{
					if (gram[i].ll > gram[i].rl)
					{
						flag = 1;
						cout << "该文法为0型文法" << endl;
						break;
					}
				}
				//2型文法判断
				if (flag == 0) 
				{
					for (i = 0; i < num; i++)
					{
						if (((gram[i].ll == 1) && (gram[i].str[0] >= 'A') && (gram[i].str[0] <= 'Z')))
						{
							flag1 = 0;
						}
						else
						{
							flag1 = 1;
							cout << "该文法为1型文法" << endl;
							break;
						}
					}
				}
				//
				if (flag1 == 0)
				{
					for (i = 0; i < num; i++)
					{
						if (gram[i].rl == 1 && gram[i].str[3] >= 'a' && gram[i].str[3] <= 'z')
						{
							flag2 = 0;

						}
						else
						{
							flag2 = 1;
							break;
						}
					}
					if (flag2 == 0)
						cout << "该文法为3型文法" << endl;
					else
					{
						for (i = 0; i < num; i++)
						{
							if (gram[i].rl == 2 && gram[i].str[3] >= 'a' && gram[i].str[3] <= 'z' && gram[i].str[4] >= 'A' && gram[i].str[4] <= 'Z' )
							{
								flagr = 0;
							}
							else
							{
								flagr = 1;
								break;
							}
						}

						if (flagr == 0)
							cout << "该文法为3型右线性文法" << endl;
						else
						{
							for (i = 0; i < num; i++)
							{
								if (gram[i].rl == 2 && gram[i].str[4] >= 'a' && gram[i].str[4] <= 'z' && gram[i].str[3] >= 'A' && gram[i].str[3] <= 'Z' )
								{
									flagl = 0;
								}
								else
								{
									flagl = 1;
									break;
								}
							}
							if (flagl == 0)
								cout << "该文法为3型左线性文法" << endl;
							else
								cout << "该文法为2型文法" << endl;
						}
					}
				}
			}
		}
		else
		{ ////已经到了文件末尾，可以直接退出
			////end of file
			break;
		}
	}
	fclose(source); /// 用完后要关闭文件指针
	return 0;
}
