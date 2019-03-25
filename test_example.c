#include "testing_suite.h"

void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);
void test6(void);

int main(void){
   //creando un test_suite
    create_suite("Testing the Framework");

    add_test(test1);
    add_test(test2);
    add_test(test3);
    add_test(test4);
    add_test(test5);
    add_test(test6);

    run_suite();
    
    clear_suite();
}

//cambiar los 1eros 2 tests

void test1(){
    assert_true(1 == 1);
}

void test2(){
    assert_false(1 == 0);
}

void test3(){
    assert_true('a' == 'a');
}

void test4(){
    assert_false(1 == 2);
}

void test5(){
    int a = 1;
    int b = 1;
    assert_equals(&a, &b, sizeof(int));
}

void test6(){
    int a = 1;
    int b = 2;
    assert_not_equals(&a,&b, sizeof(int));
}

//FALTA TESTEAR