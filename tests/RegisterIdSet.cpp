#include "BasicBlock.hpp"

RegisterId const r1(1, 0);
RegisterId const r2(2, 0);
RegisterId const r31(31, 0);
RegisterId const r32(32, 0);
RegisterId const r127(127, 0);
RegisterId const r510(510, 0);
RegisterId const r511(511, 0);
RegisterId const r1023(1023, 0);

void basic() {
    // Unit test for RegisterIdSet 
    RegisterIdSet s1(128);
    assert(s1.bits()==128);

    s1.set(r1);
    assert(s1.has(r1));
    assert(!s1.has(r31));
    assert(!s1.has(r32));
    assert(s1.count()==1);

    s1.set(r31);
    assert(s1.has(r1));
    assert(s1.has(r31));
    assert(!s1.has(r32));
    assert(s1.count()==2);

    s1.set(r32);
    assert(s1.has(r1));
    assert(s1.has(r31));
    assert(s1.has(r32));
    assert(s1.count()==3);

    s1.set(r127);
    assert(s1.has(r127));
    assert(s1.count()==4);

    RegisterIdSet s2(128);
    s2.set(r1);
    s2.set(r2);
    s2.set(r127);
    assert(s2.count()==3);

    RegisterIdSet s3 = s1 & s2;
    assert(s3.has(r1));
    assert(!s3.has(r31));
    assert(!s3.has(r32));
    assert(s3.has(r127));
    assert(s3.count()==2);

    RegisterIdSet s4 = s1 | s2;
    assert(s4.has(r1));
    assert(s4.has(r2));
    assert(s4.has(r31));
    assert(s4.has(r32));
    assert(s4.has(r127));
    assert(s4.count()==5);

    assert(s4.next(0) == 1);
    assert(s4.next(1) == 2);
    assert(s4.next(2) == 31);
    assert(s4.next(31) == 32);
    assert(s4.next(32) == 127);
    assert(s4.next(127) == -1);
}

void copy() {
    RegisterIdSet s1(128);
    
    s1.set(r1);
    s1.set(r2);
    s1.set(r31);
    s1.set(r127);

    RegisterIdSet s2(s1);
    assert(s2.has(r1));
    assert(s2.has(r2));
    assert(s2.has(r31));
    assert(s2.has(r127));

    RegisterIdSet s3 = s1;
    assert(s3.has(r1));
    assert(s3.has(r2));
    assert(s3.has(r31));
    assert(s3.has(r127));

    assert(s2 == s1);
    assert(!(s2 != s1));
    assert(s3 == s1);
    assert(!(s3 != s1));
    assert(s2 == s3);
    assert(!(s2 != s3));

    RegisterIdSet s4(128);
    assert(s4 != s1);

}

void large() {
    RegisterIdSet s1(512);
    s1.set(r1);
    s1.set(r127);
    s1.set(r511);
    assert(s1.has(r1));
    assert(s1.has(r127));
    assert(s1.has(r511));
    assert(s1.count()==3);

    RegisterIdSet s2(1024);
    s2.set(r2);
    s2.set(r127); 
    s2.set(r1023);
    assert(s2.has(r2));
    assert(s2.has(r127));
    assert(s2.has(r1023));
    assert(s2.count()==3);

    RegisterIdSet s3 = s1 & s2;
    assert(!s3.has(r1));
    assert(!s3.has(r2));
    assert(s3.has(r127));
    assert(!s3.has(r511));
    assert(!s3.has(r1023));
        
    RegisterIdSet s4 = s1 | s2;
    assert(s4.has(r1));
    assert(s4.has(r2));
    assert(s4.has(r127));
    assert(s4.has(r511));
    assert(s4.has(r1023));

    assert(s1 != s2);
    assert(!(s1 == s2));
    assert(s2 != s3);
    assert(!(s2 == s3));
    assert(s3 != s4);
    assert(!(s3 == s4));

    RegisterIdSet s5(s4);
    RegisterIdSet s6 = s4;
    assert(s5 == s4);
    assert(!(s5 != s4));
    assert(s6 == s4);
    assert(!(s6 != s4));
    assert(s5.has(r1));
    assert(s5.has(r2));
    assert(s5.has(r127));
    assert(s5.has(r511));
    assert(s5.has(r1023));
    assert(s6.has(r1));
    assert(s6.has(r2));
    assert(s6.has(r127));
    assert(s6.has(r511));
    assert(s6.has(r1023));
    assert(s6.count()==5);

    s6.set(RegisterId(0, 0));
    assert(s6.bit(0));
    assert(s6.next(-1) == 0);
    assert(s6.next(0) == 1);
    assert(s6.next(1) == 2);
    assert(s6.next(2) == 127);
    assert(s6.next(127) == 511);
    assert(s6.next(511) == 1023);
} 

void assign() {
    RegisterIdSet s1(1024);
    s1.set(r1);
    s1.set(r127);
    s1.set(r511);

    RegisterIdSet s2(1024);
    s2.set(r1023);
    s2 |= s1; 

    assert(s2.has(r1));
    assert(s2.has(r127));
    assert(s2.has(r511));
    assert(s2.has(r1023));
}

int main() {
    basic();
    copy();
    large();
    assign();
   
    return 0;
}
