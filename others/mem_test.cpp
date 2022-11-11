#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

int main() {

    void *p = nullptr;
    int size = 32*1024*1024; // 32GB
    int ret = posix_memalign(&p, 4096, size);

    if (ret != 0) {
        cout << "Failed to posix_memalign for 8GB spaceof memory" << endl;
    }

    unsigned char *c = reinterpret_cast<unsigned char *> (p);
    c += size;

    cout << "Address of p: " << reinterpret_cast<uintptr_t>(p) << ", end of p: " << reinterpret_cast<uintptr_t>(c) << endl;

    int *x = new int;
    *x = 10;
    cout << "x: " << *x << ", addr: " << reinterpret_cast<uintptr_t>(x) << endl;
    
    int *m = (int *)malloc(sizeof(int)*10);
    memset(m, 0, sizeof(int)*10);
    cout << "m: " << *m << ", addr: " << reinterpret_cast<uintptr_t>(m) << endl;

    return 0;
}


