
#include <stdio.h>


extern void test_list (void);
extern void test_clist (void);

int main ()
{
    printf("\n");
    test_list();
    printf("\n");
    test_clist();
    printf("\n");

    return 0;
}
