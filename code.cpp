#include <cstdio>
#include <cstdlib>

// 创建全局变量，方便进行调用
// num用来存储输入的四个数字
unsigned short num[4];
// IsUsed用来判定该数字是否被使用
bool IsUsed[4] = {false};
// 结构体用来存储结果
// _A_#_B_=_C_
// First即代表A，symbol即代表#，last即代表B，result即代表C
struct process
{
    short first;
    unsigned short symbol;
    short last, result;
} p[3];

// 这个函数用来打印结果，也就是将结构体里的数值转换为看得懂的字符
void print()
{
    for (unsigned short i = 0; i < 3; i++)
    {
        printf("%d", p[i].first);
        switch (p[i].symbol)
        {
        case 0:
            printf("+");
            break;
        case 1:
            printf("-");
            break;
        case 2:
            printf("*");
            break;
        case 3:
            printf("/");
            break;
        }
        printf("%d=%d\n", p[i].last, p[i].result);
    }
    printf("\n");
}

// 这个函数用来判定是否是整数
bool IsInteger(double num)
{
    if ((int)(num) % 10 != 0 && (int)(num * 100) % 100 == 0)
        return true;
    else
        return false;
}

// 这个函数用来将两项直接相加并记录到结构体中（在(a+b)*(c+d)环境中使用）
unsigned short DoublePlus(unsigned short layer, unsigned short a, unsigned short b)
{
    p[layer].first = a;
    p[layer].last = b;
    p[layer].symbol = 0;
    unsigned short result = a + b;
    p[layer].result = result;
    return result;
}

// 这个函数用来将两项直接相减并记录到结构体中（在(a+b)*(c-d)环境中使用），其中a为被减数，b为减数
short DoubleMinus(unsigned short layer, unsigned short a, unsigned short b)
{
    p[layer].first = a;
    p[layer].last = b;
    p[layer].symbol = 1;
    short result = a - b;
    p[layer].result = result;
    return result;
}

// 这个函数用来搜索每一种情况，其中layer为确认已经搜索了几个数字,cNum为当前需要处理的新数字，result为之前操作得到的结果
void search(unsigned short layer, unsigned short cNum, short result)
{
    p[layer].first = result;
    p[layer].last = cNum;
    for (unsigned short j = 0; j < 6; j++)
    {
        short temp1, temp3;
        double temp2;
        // j=0为加，j=1为减，j=2为乘，j=3为除，j=4为多乘，j=5为多除
        switch (j)
        {
        // 相加
        case 0:
        {
            p[layer].symbol = 0;
            temp1 = result + cNum;
            p[layer].result = temp1;
            if (layer == 2)
            {
                if (temp1 == 24)
                    print();
            }
            else
            {
                for (unsigned short i = 0; i < 4; i++)
                {
                    if (!IsUsed[i])
                    {
                        IsUsed[i] = true;
                        search(layer + 1, num[i], temp1);
                        IsUsed[i] = false;
                    }
                }
            }
            break;
        }
        // 相减
        case 1:
        {
            p[layer].symbol = 1;
            temp1 = result - cNum;
            p[layer].result = temp1;
            if (layer == 2)
            {
                if (temp1 == 24)
                    print();
            }
            else
            {
                for (unsigned short i = 0; i < 4; i++)
                {
                    if (!IsUsed[i])
                    {
                        IsUsed[i] = true;
                        search(layer + 1, num[i], temp1);
                        IsUsed[i] = false;
                    }
                }
            }
            break;
        }
        // 相乘
        case 2:
        {
            p[layer].symbol = 2;
            temp1 = result * cNum;
            p[layer].result = temp1;
            if (layer == 2)
            {
                if (temp1 == 24)
                    print();
            }
            else
            {
                for (unsigned short i = 0; i < 4; i++)
                {
                    if (!IsUsed[i])
                    {
                        IsUsed[i] = true;
                        search(layer + 1, num[i], temp1);
                        IsUsed[i] = false;
                    }
                }
            }
            break;
        }
        // 相除
        case 3:
        {
            p[layer].symbol = 3;
            // 这里是确保相除后得到的数为整数
            temp2 = (double)result / cNum;
            if (IsInteger(temp2))
            {
                p[layer].result = temp2;
                if (layer == 2)
                {
                    if (temp2 == 24)
                        print();
                }
                else
                {
                    for (unsigned short i = 0; i < 4; i++)
                    {
                        if (!IsUsed[i])
                        {
                            IsUsed[i] = true;
                            search(layer + 1, num[i], temp2);
                            IsUsed[i] = false;
                        }
                    }
                }
            }
            break;
        }
        // 两个数先进行加减法操作，再与result进行乘法操作，即a*(b+c)等
        case 4:
        {
            if (layer == 1)
            {
                // 先找到两个还没使用的数
                short unused = 0;
                for (unsigned short i = 0; i < 4; i++)
                    if (!IsUsed[i])
                    {
                        IsUsed[i] = true;
                        unused = i;
                        break;
                    }
                // 两个数先加再乘
                temp3 = DoublePlus(layer, num[unused], cNum);
                layer++;
                p[layer].first = result;
                p[layer].last = temp3;
                p[layer].symbol = 2;
                temp1 = result * temp3;
                p[layer].result = temp1;
                if (temp1 == 24)
                    print();
                layer--;
                // 两个数先减再乘
                temp3 = DoubleMinus(layer, num[unused], cNum);
                layer++;
                p[layer].first = result;
                p[layer].last = temp3;
                p[layer].symbol = 2;
                temp1 = result * temp3;
                p[layer].result = temp1;
                if (temp1 == 24)
                    print();
                layer--;
                IsUsed[unused] = false;
            }
            break;
        }
        // 两个数先进行加减法操作，再与result进行除法操作，即a*(b+c)等
        case 5:
        {
            if (layer == 1)
            {
                // 先找到两个还没使用的数
                short unused = 0;
                for (unsigned short i = 0; i < 4; i++)
                    if (!IsUsed[i])
                    {
                        IsUsed[i] = true;
                        unused = i;
                        break;
                    }
                // 两个数先加再除
                temp3 = DoublePlus(layer, num[unused], cNum);
                layer++;
                p[layer].first = result;
                p[layer].last = temp3;
                p[layer].symbol = 3;
                temp2 = (double)result / temp3;
                if (IsInteger(temp2))
                {
                    p[layer].result = temp2;
                    if (temp2 == 24)
                        print();
                }
                layer--;
                // 两个数先减再除
                temp3 = DoubleMinus(layer, num[unused], cNum);
                layer++;
                p[layer].first = result;
                p[layer].last = temp3;
                p[layer].symbol = 3;
                temp2 = (double)result / temp3;
                if (IsInteger(temp2))
                {
                    p[layer].result = temp2;
                    if (temp2 == 24)
                        print();
                }
                layer--;
                IsUsed[unused] = false;
            }
            break;
        }
        }
    }
}

int main()
{
    // 这里开始读入四个数字
    for (unsigned short i = 0; i < 4; i++)
        scanf("%hd", &num[i]);
    for (unsigned short j = 0; j < 4; j++)
        for (unsigned short i = 0; i < 4; i++)
            if (j != i)
            {
                IsUsed[i] = IsUsed[j] = true;
                search(0, num[j], num[i]);
                IsUsed[i] = IsUsed[j] = false;
            }
    system("pause");
    return 0;
}