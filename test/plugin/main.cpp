#include <windows.h>
#include <stdio.h>
#include "yhook_test.h"
#include "yinjection_test.h"

int main(int argc, char *argv[])
{
    printf("------------YInlineHookTest Ready!\n");
    system("pause");
    printf("------------YInlineHookTest Start!\n");
    YInlineHookTest();
    printf("------------YInlineHookTest End!\n\n");

    printf("------------YRemoteThdInjcTest Ready!\n");
    system("pause");
    printf("------------YRemoteThdInjcTest Start!\n");
    YRemoteThdInjcTest();
    printf("------------YRemoteThdInjcTest End!\n\n");

    system("pause");
    return 0;
}
