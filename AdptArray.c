#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdptArray.h"


struct AdptArray_ {
    int size; // size of current array.
    PElement *my_arr;
    DEL_FUNC del_f;
    COPY_FUNC copy_f;
    PRINT_FUNC print_f;
};


//DEL_FUNC del_f;
//COPY_FUNC copy_f;
//PRINT_FUNC print_f;

// Initializes an empty PAdptArray.
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray new_arr =(PAdptArray) malloc(sizeof(struct AdptArray_));
    if (new_arr == NULL) {
        printf("ERROR: Unable to allocate.");
        return NULL;
    }
    new_arr->size = 0;
    new_arr->my_arr = NULL;
    new_arr->copy_f = copyFunc;
    new_arr->del_f = delFunc;
    new_arr->print_f = printFunc;
//    copy_f = copyFunc;
//    del_f = delFunc;
//    print_f = printFunc;
    return new_arr;
}

// Releases the array's memory (including its indexed objects).
void DeleteAdptArray(PAdptArray arr) {
    if (arr == NULL) {
        return;
    }
    if (arr->my_arr != NULL) {
        for (int i = 0; i < arr->size; ++i) {
            arr->del_f(arr->my_arr[i]);
//            del_f(arr->my_arr[i]);
        }
    }
    // free the pointer my_arr
    free(arr->my_arr);
    // free the object PAdptArray itself
    free(arr);
}

// Receives an index and an object and saves a copy of the object in the requested index. Frees the old member if Exists.
Result SetAdptArrayAt(PAdptArray arr, int index, PElement obj) {
    if (index < 0) {
        return FAIL;
    }
    if (index >= arr->size) { // Reallocate the PAdptArray with index+1.
        int new_size = index + 1;
        arr->my_arr = realloc(arr->my_arr, new_size * sizeof(void *));
        if (arr->my_arr == NULL) {
            printf("ERROR: Unable to allocate.");
            return FAIL;
        }
        for (int i = arr->size; i < new_size; i++) {
            arr->my_arr[i] = NULL;
        }
        arr->size = new_size;
    } else if (arr->my_arr[index] != NULL) {
        arr->del_f(arr->my_arr[index]);
//        del_f(arr->my_arr[index]);
    }
    arr->my_arr[index] = arr->copy_f(obj);
    if (arr->my_arr[index] == NULL && obj != NULL) {
        return FAIL;
    } else {
        return SUCCESS;
    }
}

// Gets an index and returns a copy of the object at this location.
PElement GetAdptArrayAt(PAdptArray arr, int index) {
    if (index < 0 || index >= arr->size) {
        return NULL;
    } else if (arr->my_arr[index] != NULL) {
        return arr->copy_f(arr->my_arr[index]);
    } else {
        return NULL;
    }
}

// Returns the size of the array (-1 when the array was not initialized successfully).
int GetAdptArraySize(PAdptArray arr) {
    if (arr == NULL) {
        return -1;
    } else {
        return arr->size;
    }
}

// Prints all the objects of the array.
void PrintDB(PAdptArray arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < arr->size; ++i) {
        if (arr->my_arr[i] != NULL) {
            arr->print_f(arr->my_arr[i]);
//            print_f(arr->my_arr[i]);
        }
    }
}
