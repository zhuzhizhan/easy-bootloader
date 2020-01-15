#include <stdio.h>
#include <string.h>
 
struct _s_application_init {
    int(*function)(void);
};
 
struct _s_application_init _init_start;//段".application_init"的起始地址，在*.lds文件中定义
struct _s_application_init _init_end;//段".application_init"的末尾地址，在*.lds文件中定义
#define __app_init_section __attribute__((section(".application_init")))
#define __application_init(function) \
    struct _s_application_init _s_a_init_##function  __app_init_section = {function}
 
 
static int application_init_a(void)
{
    printf("execute funtion : %s\n", __FUNCTION__);
    return 0;
}
__application_init(application_init_a);
 
static int application_init_b(void)
{
    printf("execute funtion : %s\n", __FUNCTION__);
    return 0;
}
__application_init(application_init_b);
 
static int application_init_c(void)
{
    printf("execute funtion : %s\n", __FUNCTION__);
    return 0;
}
__application_init(application_init_c);
 
int main(int argc, char **argv)
{
    /*
     * 从段的起始地址开始获取数据，直到末尾地址
     */
    struct _s_application_init *pf_init = &_init_start;
    do {
        printf("Load init function from address %p\n", pf_init);
        pf_init->function();
        ++pf_init;
    } while (pf_init < &_init_end);
    return 0;
}
