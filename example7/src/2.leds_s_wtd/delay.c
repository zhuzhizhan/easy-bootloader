
void delay(int r0)
{
    volatile int count = r0;

    while (count--)
        ;
}
