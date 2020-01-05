#include "bigint.h"
#include <iostream>
using namespace std;
int main() {
    Bigint a("3"), b("22");
    for (Bigint i = 1; i <= a; a = a + 1) {
        cout << "aaa" << endl;
    }

    return 0;
}