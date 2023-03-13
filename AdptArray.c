// Created by Shalev Ben David.
#include "AdptArray.h"
#include <stdlib.h>

// Redefining the struct.
struct AdptArray_ {
    DEL_FUNC delF; // Pointer to the delete function.
    COPY_FUNC copyF; // Pointer to the copy function.
    PRINT_FUNC printF; // Pointer to the print function.
    PElement* items; // Item in the adaptive array.
    int size; // The size of the array.
};

// Initializes an empty array and returns a pointer to it.
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray arr = (PAdptArray)malloc(sizeof(PAdptArray));
    if(!arr) return NULL;
    // No items in the array.
    arr -> items = (PElement*) calloc(1, sizeof (PElement));
    arr -> size = 0;
    // Assigning the pointers we got as parameters to the object.
    arr -> delF = delFunc;
    arr -> copyF = copyFunc;
    arr -> printF = printFunc;
    // Returning the array pointer.
    return arr;
}

// Deletes the array and frees it from the memory.
void DeleteAdptArray(PAdptArray arr) {
    // Only if there is something to free (arr != NULL).
    if (arr) {
        // Free all items in the array.
        for (int i = 0; i < arr -> size; i++) {
            // First delete the item with the appropriate function.
            arr -> delF(arr -> items[i]);
            // Now free its allocated memory.
            free(arr -> items[i]);
        }
        // Free the array object.
        free(arr);
    }
}

// Receives the array, an index and an item. Sav
Result SetAdptArrayAt(PAdptArray arr, int index, PElement item) {
    // If the array wasn't initialized properly.
    if (arr == NULL) {
        return FAIL;
    }
    // If the index exists in the current array, there's no need to increase its size.
    if (index <= arr -> size - 1) {
        // Copy the item to a temporary object.
        PElement temp = arr -> copyF(item);
        // Insert the copy to the array at index.
        arr -> items[index] = temp;
        return SUCCESS;
    }
    // If the index doesn't exist in the current array, we need to increase the size.
    else {
        PElement* tempRealloc;
        if ((tempRealloc = realloc(arr -> items, (index + 1) * sizeof (PElement)) == NULL)){
            DeleteAdptArray(arr);
            return FAIL;
        }
        else {
            // Update the size of the array.
            arr -> size = index + 1;
            // Make arr point to the new memory address.
            arr -> items = tempRealloc;
            // Copy the item to a temporary object.
            PElement temp = arr -> copyF(item);
            // Insert the copy to the array at index.
            arr -> items[index] = temp;
            return SUCCESS;
        }
    }
}

// Receives index and returns ac oopy of the item in that index.
PElement GetAdptArrayAt(PAdptArray arr, int index) {
    // If the array wasn't initialized properly or index doesn't exist.
    if (arr == NULL || index > arr -> size -1) {
        return NULL;
    }
    // If the index exists in the current array, return the copy in index.
    else {
        return arr -> copyF(arr -> items[index]);
    }
}


// Returns the size of the array, or -1 if it wasn't initialized properly.
int GetAdptArraySize(PAdptArray arr) {
    // If the array was initialized properly (arr != NULL)
    if (arr) {
       return arr -> size;
    }
    // If the array wasn't initialized properly (arr == NULL)
    else {
        return -1;
    }
}


// Print the array items.
void PrintDB(PAdptArray arr) {
    for (int i = 0; i < arr -> size; i++) {
        // Call the appropriate function to print each item.
        arr -> printF(arr -> items[i]);
    }
}