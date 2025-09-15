#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

void fun(int *ptr){
    int b = 10;
    ptr = &b;
    cout<<*ptr<<endl;
    return;
}

int main()
{
    int a = 20;
    int *ptr = &a;
    cout<<*ptr<<endl;
    fun(ptr);
    cout<<*ptr<<endl;
    return 0;
}