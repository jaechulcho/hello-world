#include <cstdio>
#include <memory>

using namespace std;

void f()
{
    unique_ptr<int> a(new int(3));
    printf("%d\n", *a.get());
}

int main()
{
    f();
    
    return 0;
}
