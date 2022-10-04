#include <stdio.h>
#include <math.h>
#include <string.h>

const short true = 1, false = 0;

typedef struct define {
    short pri, op;  //priority
} define;

typedef struct operation {
    define def;
    double *left, *right;
    double ans;
} operation;

const define null = {-1, -1}, pls = {0, 0}, sub = {0, 1}, mod = {1, 2}, mul = {2, 3}, div = {2, 4}, power = {3, 5};

char raw[255];
double nums[20];
operation ops[20];
int p = 0, p_dec = 0, p_op = 1, ans_index=1;


int get_num_op(char s[255]) {
    //p:point d:decimal
    char ch = ' ';
    int i, j;
    double t = 0;
    short decimal = false, is_num = false, minus = false;

    for (i = 0; i <= strlen(raw); i++) {
        ch = raw[i];
        if (ch >= '0' && ch <= '9') {
            is_num = true;
            if (decimal) {
                p_dec++;
            }
            t = t * 10 + (int) ch - 48;
        } else if (ch == '.') {
            decimal = true;
        } else
            //是符号
        {
            if (is_num) //是数字后面的符号
            {
                //处理数字部分
                if (decimal) //处理小数
                {
                    t = t / pow(10, p_dec);
                    p_dec = 0;
                    decimal = false;
                }
                nums[p] = t;
                if (minus) //处理负号
                {
                    nums[p] = -nums[p];
                    minus = false;
                }
                t = 0;
                p++;
                is_num = false;
                //处理符号部分
                //首先实现+-，然后*/，再%^，最后()

                switch (ch) {
                    case '+':
                        ops[p_op].def = pls;
                        break;
                    case '-':
                        ops[p_op].def = sub;
                        break;
                    case '*':
                        ops[p_op].def = mul;
                        break;
                    case '/':
                        ops[p_op].def = div;
                        break;
                    case '%':
                        ops[p_op].def = mod;
                        break;
                    case '^':
                        ops[p_op].def = power;
                        break;
                    case '(':
                        printf("error");
                        break;
                    case ')':
                        printf("error");
                        break;
                    default:
                        ops[p_op].def = null;
                }

                p_op++;
            } else  //前一个不是数字但这个是符号--这个是正负号
            {
                //判断负号
                if (ch == '-') {
                    minus = true;
                }
                //判断正号算了别判断了没意义
                //else if(ch=='+')
            }
        }
    }
    return 0;
}

double calculate(const double *a, const double *b, short op) {
    double result = 0;
    switch (op) {
        case -1:
            return *b;
            break;
        case 0:
            return *a + *b;
            break;
        case 1:
            return *a - *b;
            break;
        case 2:
            return 0; //存在问题
            break;
        case 3:
            return *a * *b;
            break;
        case 4:
            return *a / *b;
            break;
        case 5:
            return pow(*a, *b);
            break;
        default:
            printf("Unknown Error!");
    }
    return 0;
}

void recursive(int i, int start) {
    if (i <= p_op - 1) {
        if (ops[i].def.pri == ops[i + 1].def.pri) {
            ops[i].right = &ops[i + 1].ans;
            recursive(i + 1, start);
            ops[i].ans = calculate(ops[i].left, ops[i].right, ops[i].def.op);
        } else if (ops[i].def.pri > ops[i + 1].def.pri) {
            ops[i].ans = calculate(ops[i].left, ops[i].right, ops[i].def.op);
            ops[i + 1].left = &ops[start-1].ans;
        } else if (ops[i].def.pri < ops[i + 1].def.pri) { //必须考虑升阶之后的降阶
            int j = i;
            while (j < p_op - 1) {
                j++;
                if (ops[j].def.pri > ops[j + 1].def.pri) {
                    j++;
                    break;
                }
            }
            ops[i].right = &ops[i + 1].ans;
            recursive(i + 1, i + 1);
            ops[i].ans = calculate(ops[i].left, ops[i].right, ops[i].def.op);
            ops[j].left = &ops[start].ans;
            if (j<p_op-1){
                ans_index = j;
                recursive(j, j);  //这两条绝对不能反！！！！！
            }
        }
    }
}

double process_op(void) {
    int i, j;
    operation *t;
    double result;
    //初始化指针数组
    ops[0].def = null;
    ops[0].ans = nums[0];
    ops[p_op - 1].ans = nums[p - 1];
    for (i = 1; i < p_op - 1; i++) {
        ops[i].left = &nums[i - 1];
        ops[i].right = &nums[i];
    }
    /*
    //按照运算优先级排序
    result = calculate(op[1]);
    for (i = 0; i < p_op - 1; i++) {
        for (j = i + 1; i < p_op; j++) {
            if (op_processed[i]->def.pri < op_processed[j]->def.op) {
                t = op_processed[i];
                op_processed[i] = op_processed[j];
                op_processed[j] = t;
            }
        }
    }
     */
    return 0;
}


/*
double execute(void) {
    int i;
    short last_pri;
    double result = nums[0];

    operation *t;
    last_pri = op[0].def.pri;
    for (i = 1; i < p_op; i++) {
        t = op_processed[i];
        if (t->def.pri == last_pri) {
            t->ans = calculate(t->left.ans, t->right.ans, t->def.op);
        } else {
        }
    }
    return result;
}
*/

int main(void) {
    scanf("%s", raw);
    get_num_op(raw);
    process_op();
    recursive(0, 0);
    printf("answer:%0.4f", ops[ans_index].ans);
}
