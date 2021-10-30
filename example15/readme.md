## 内联汇编

1.

```
#include <stdio.h>

unsigned long byte_swap(unsigned long val)
{
	int ch;
	
	asm(
	"eor r3, %1, %1, ror #16\n\t"
	"bic r3, r3, #0x00ff0000\n\t"
	"mov %0, %1, ror #8\n\t"
	: "=r" (val)
	: "r3"
	);
}
```

2.&修饰符

```
int num;
__asm__ __volatile__(
	// mov r3, #123
	"mov %0, %1\n\t"
	: "=r" (num)
	: "r" (123)
);

int num;
__asm__ __volatile__(
	// mov r3, #123
	"mov %0, %1\n\t"
	: "=&r" (num)
	: "r" (123)
);
```

3.使用占位符

```
int a = 100, b = 200;
int result;
__asm__ __volatile__(
	"mov %0, %3\n\t"	\\ mov r3, #123, %0 代表result，%3代表123
	"ldr r0, %1\n\t"	\\ ldr r0, [fp, #12]， %1代表a的地址
	"ldr r1, %2\n\t"	\\ ldr r1, [fp, #-16],%2代表b的地址
	"str r0, %2\n\t"	\\ str r0, [fp, #-16],因为%1和%2是地址，所以只能用ldr,str指令
	"str r1, %1\n\t"	\\ str r1, [fp, #-12]
	: "=r" (result), "+m" (a), "+m" (b)	\\ out1 is %0, out2 is %1 ... outn is %(n - 1)
	: "i" (123)		\\ in1 is %n, in2 is %(n + 1)
);
```

4.指定输出

```
__asm__ __volatile__(
	"asm code"
	:
	: "constraint" (variable / immediate)
);

constraint 定义 variable / immediate 的存放位置
r : 使用任何可用的通用寄存器
m : 使用变量的内存地址
i : 使用立即数
```

5.语法

```
asm volatile(
"asm code" : output : input : changed
);
```



