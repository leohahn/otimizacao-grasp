#include <iostream>

using namespace std;

class Vector {
public:
    Vector(int size): sz{size}, elem{new int[size]} {}
private:
    int sz;
    int* elem;
};
int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
