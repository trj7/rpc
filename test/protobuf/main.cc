#include<iostream>

#include"test.pb.h"

using namespace fixbug;
using namespace std;
int main()
{
    loginRequest req;
    req.set_name("1111");
    req.set_pwd("123");
    string str;
    if(req.SerializeToString(&str))
    {
        cout<<str.c_str()<<endl;
    }
    
    return 0;
}