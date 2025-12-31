#include <stdio.h>

#include "../memory/MEM.h"
#include "svm.h"

static SVM_Value native_print(SVM_VirtualMachine* svm, SVM_Value* values,int arg_count) {
    SVM_Value v;
    v.ival = 0;
    int stack_index = svm->sp - arg_count;

    if (svm->stack_value_type[stack_index] == SVM_INT) {
        printf("%d\n", values[0].ival);
    }
    else if (svm->stack_value_type[stack_index] == SVM_DOUBLE) {
        printf("%f\n", values[0].dval);
    }
    else {
        printf("print: unkown type\n");
    }
    return v;
}

static SVM_Value native_printb(SVM_VirtualMachine* svm, SVM_Value* values,
                               int arg_count) {
    SVM_Value v;
    v.ival = 1;
    printf("printb\n");
    printf("%d\n", values[0].ival);
    return v;
}

void add_native_functions(SVM_VirtualMachine* svm) {
    svm_add_native_function(svm, native_print, "print", 1);
    svm_add_native_function(svm, native_printb, "printb", 1);
}
