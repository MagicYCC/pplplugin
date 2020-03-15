#include <windows.h>
#include <stdio.h>
#include <plugin\yinjection.h>
#include <utils\yprocess.h>
#include "yinjection_test.h"

using namespace YPlugIn;
using namespace YUtils;
using namespace std;

void YRemoteThdInjcTest()
{
    string  hostName    = "stub_host.exe";
    string  modeleName  = "stub_module.dll";

    DWORD hostId;
    ProcessIdByBaseName(hostName, hostId);
    if(!hostId)
    {
        printf("Can not find stub_host!\n");
        return;
    }

    YRemoteThreadInjection inj;
    inj.Set(hostId, modeleName);

    int ret;

    ret = inj.Attach();
    if(ret < YRemoteThreadInjection::Succ)
    {
        printf("Attach error!\n");
        printf("ret = %d\n", ret);
        return;
    }
    printf("Attach Success!\n");

    system("pause");

    ret = inj.Detach();
    if(ret < YRemoteThreadInjection::Succ)
    {
        printf("Detach error!\n");
        printf("ret = %d\n", ret);
        return;
    }
    printf("Detach Success!\n");
}
