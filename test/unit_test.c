
#include <stdio.h>


extern void test_list (void);
extern void test_clist (void);
extern void test_macro_args (void);
extern void test_json_field (void);
extern void test_mac_addr (void);

int main ()
{
    printf("\n");
    test_list();
    printf("\n");
    test_clist();
    printf("\n");
    test_macro_args();
    printf("\n");
    test_json_field();
    printf("\n");
    test_mac_addr();
    printf("\n");

    return 0;
}
