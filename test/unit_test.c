
#include <stdio.h>


extern void test_list (void);
extern void test_clist (void);
extern void test_macro_args (void);

int main ()
{
    printf("\n");
    test_list();
    printf("\n");
    test_clist();
    printf("\n");
    test_macro_args();
    printf("\n");

    return 0;
}
