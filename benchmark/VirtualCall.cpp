#include <iostream>

class Intf {
public:
    virtual void call()=0;
};

class Obj : public Intf {
public:
    int x;
    Obj() : x(0) {}
    virtual void call() {
        x += 1;
    }
};

Obj obj;

Intf* intf() {
    return &obj;
}

int main() {

    Intf* in = intf();
    int i = 0;
    
    while (i < 100000000) {
        in->call();
        i = i + 1;
    }
    std::cout << obj.x << std::endl;
    return 0;
}
