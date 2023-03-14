// Created by Shalev Ben David.
#include "AdptArray.h"
#include <stdlib.h>

// Redefining the struct.
struct AdptArray_ {
    DEL_FUNC delF; // Pointer to the delete function.
    COPY_FUNC copyF; // Pointer to the copy function.
    PRINT_FUNC printF; // Pointer to the print function.
    PElement* items; // Pointer to the adaptive array.
    int size; // The size of the array.
};

// Initializes an empty array and returns a pointer to it.
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray arr = (PAdptArray)malloc(sizeof(struct AdptArray_));
    if(!arr) return NULL;
    // Allocate memory for the array.
    arr -> items = (PElement*) calloc(1, sizeof (PElement));
    // If allocation failed.
    if (!arr -> items) {
        free(arr);
        return NULL;
    }
    // No items in the array.
    arr -> size = 0;
    // Assigning the pointers we got as parameters to the fields.
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
            // Delete the item with the appropriate function only if it's not NULL.
            if (arr -> items[i])
                arr -> delF(arr -> items[i]);
        }
        // Free the allocation of the array.
        free(arr -> items);
        // Free the array object.
        free(arr);
    }
}

// Method that copies the old array to a new array of size "newSize" and returns a pointer to it.
PElement* extendArray (PAdptArray arr, int newSize) {
    PElement* newArray = (PElement*) calloc(newSize, sizeof (PElement));
    // If calloc method failed.
    if (!newArray) return NULL;
    // Copy the items from the old array to the new array.
    for(int i = 0; i < arr -> size; i++) {
        // Only if it's not NULL we will copy it to the new array and delete it from the old array.
        if (arr -> items[i]) {
            newArray[i] = arr -> copyF(arr ->items[i]);
            arr -> delF(arr -> items[i]);
        }
    }
    // Free the allocation of the old array.
    free(arr -> items);
    // Return the pointer to the new array.
    return newArray;
}

// Receives the array, an index and an item. Saves a copy of the item in that index.
Result SetAdptArrayAt(PAdptArray arr, int index, PElement item) {
    // If the array wasn't initialized properly.
    if (!arr) return FAIL;
    // If the index exists in the current array, there's no need to increase its size.
    if (index <= arr -> size - 1) {
        // If theres an item in index, delete it.
        if (arr -> items[index])
            arr -> delF(arr -> items[index]);
        // Copy the item to a temporary object (if not NULL).
        if (item) {
            PElement temp = arr -> copyF(item);
            // Insert the copy to the array at index.
            arr -> items[index] = temp;
        }
        else {
            arr -> items[index] = NULL;
        }
        return SUCCESS;
    }
    // If the index doesn't exist in the current array, we need to increase the size.
    else {
        PElement* tempArray;
        if ((tempArray = extendArray(arr, index + 1)) == NULL){
            return FAIL;
        }
        else {
            // Update the size of the array.
            arr -> size = index + 1;
            // Make arr point to the new memory address.
            arr -> items = tempArray;
            // Copy the item to a temporary object (if not NULL).
            if (item) {
                PElement temp = arr -> copyF(item);
                // Insert the copy to the array at index.
                arr -> items[index] = temp;
            }
            else {
                arr -> items[index] = NULL;
            }
            return SUCCESS;
        }
    }
}

// Receives index and returns a copy of the item in that index.
PElement GetAdptArrayAt(PAdptArray arr, int index) {
    // If the array wasn't initialized properly or index doesn't exist.
    if (!arr || index > arr -> size - 1) return NULL;
    // If the index exists in the current array, return the copy at index (if not NULL).
    else {
        if (arr -> items[index])
            return arr -> copyF(arr -> items[index]);
        return NULL;
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
    // Check first that arr is initialized (if not NULL).
    if (arr) {
        for (int i = 0; i < arr -> size; i++) {
            // Call the appropriate function to print each item (if not NULL).
            if (arr -> items[i]) {
                arr -> printF(arr -> items[i]);
            }
        }
    }
}