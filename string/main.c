#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum {
    VAL_UNDEFINED,
    VAL_NULL,
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_OBJECT,
} ValueType;

typedef struct {
    ValueType type;
    union {
        double number;
        char* string;
        bool boolean;
        void* object;
    }as;
}Value;


typedef struct {
    Value* elements;
    int length;
    int capacity;
} Array;

typedef Value(*MapCalback)(Value item, int index, Array* array);
typedef bool(*FilterCallback)(Value item, int index, Array* array);
typedef void(*ForEachCallback)(Value item, int index, Array* array);
typedef Value(*ReduceCallback)(Value accumulator, Value item, int index, Array* array);
typedef int(*sortCallback)(Value a, Value b);


Value create_number(double num){
    Value val = {VAL_NUMBER, {.number=num}};
    return val;
}

Value create_string(const char* str){
    Value val= {VAL_STRING};
    val.as.string = malloc(strlen(str) +1);
    strcpy(val.as.string,str);
    return val;
}


Value create_boolean(bool b){
    Value val = {VAL_BOOLEAN, {.boolean=b}};
    return val;
}

Value create_undefined(){
    Value val = {VAL_UNDEFINED};
    return val;
}

Value create_null(){
    Value val = {VAL_NULL};
    return val;
}

void free_value(Value* val){
    if(val->type == VAL_STRING && val->as.string){
        free(val->as.string);
    }
    free(val);
}

Value copy_value(Value val) {
    if(val.type == VAL_STRING){
        return create_string(val.as.string);
    }
    return val;

}

void print_value(Value val){
    switch(val.type){
        case VAL_NUMBER:
            printf("%.2f\n",val.as.number);
            break;
        case VAL_STRING:
            printf("\"%s\"",val.as.string);
            break;
        case VAL_BOOLEAN:
            printf("%s",(val.as.boolean)? "true": "false");
            break;
        case VAL_UNDEFINED:
            printf("undefined");
            break;
        case VAL_NULL:
            printf("null");
            break;
    }
}

Array* create_array(){
    Array* array = malloc(sizeof(Array));
    array->elements = malloc(sizeof(Value)*4);
    array->length = 0;
    array->capacity = 4;
    return array;
}


void free_array(Array* arr){
    for(int i=0; i<arr->length; i++){
        free_value(&arr->elements[i]);
    }
    free(arr->elements);
    free(arr);
}

void resize_array(Array* arr, int new_capacity){
    arr->elements = realloc(arr, sizeof(Value)* new_capacity);
    arr->capacity = new_capacity;
}

int push(Array* arr, Value val){
    if(arr->length >= arr->capacity){
        resize_array(arr, arr->capacity * 2);
    }

    arr->elements[arr->length] = copy_value(val);
    arr->length++;
    return arr->length;
}

Value pop(Array* arr){
    if(arr->length == 0){
        return create_undefined();
    }

    arr->length--;
    Value result = arr->elements[arr->length];

    if(arr->length >0 && arr->length <= arr->capacity /4){
        resize_array(arr, arr->capacity/2);
    }
    
    return result;
}



int main(){
    return 0;
}