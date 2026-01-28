# Memory Management Report for Exercice 4

## Objective
The goal of this exercise was to analyze, identify, and fix memory leaks in a C program that allocates, initializes, prints, and duplicates an array.

## Initial Analysis
The initial version of the program contained memory leaks due to the following issues:
1. Memory allocated in the `allocate_array` function was not freed.
2. Memory allocated in the `duplicate_array` function was not freed.

### Valgrind Output (Before Fix)
The Valgrind tool was used to detect memory leaks. The output showed:
- **40 bytes in 2 blocks** were definitely lost.
- Memory leaks occurred in:
  - `allocate_array` function.
  - `duplicate_array` function.

#### Example Output:
```
==1677607== HEAP SUMMARY:
==1677607==     in use at exit: 40 bytes in 2 blocks
==1677607==   total heap usage: 3 allocs, 1 frees, 1,064 bytes allocated
==1677607== 
==1677607== 20 bytes in 1 blocks are definitely lost in loss record 1 of 2
==1677607==    at 0x4C39185: malloc (vg_replace_malloc.c:442)
==1677607==    by 0x400771: allocate_array (code_with_memo_leaks.c:8)
==1677607==    by 0x40090D: main (code_with_memo_leaks.c:48)
==1677607== 
==1677607== 20 bytes in 1 blocks are definitely lost in loss record 2 of 2
==1677607==    at 0x4C39185: malloc (vg_replace_malloc.c:442)
==1677607==    by 0x400897: duplicate_array (code_with_memo_leaks.c:34)
==1677607==    by 0x400944: main (code_with_memo_leaks.c:52)
```

## Fixes Implemented
1. **`free_memory` Function**:
   - Implemented to free dynamically allocated memory.
   - Ensures that memory is released when no longer needed.

2. **Freeing Memory in `main`**:
   - Added calls to `free_memory` for both `array` and `array_copy`.

### Updated Code
The updated code ensures all allocated memory is properly freed:
```c
void free_memory(int *arr) {
    if (arr) {
        free(arr);
    }
}

int main() {
    int *array = allocate_array(SIZE);
    initialize_array(array, SIZE);
    print_array(array, SIZE);

    int *array_copy = duplicate_array(array, SIZE);
    print_array(array_copy, SIZE);

    free_memory(array);
    free_memory(array_copy);

    return 0;
}
```

## Verification
After applying the fixes, Valgrind was used again to verify the program. The output showed:
- **No memory leaks**.
- All heap blocks were freed.

### Valgrind Output (After Fix)
```
==1727319== HEAP SUMMARY:
==1727319==     in use at exit: 0 bytes in 0 blocks
==1727319==   total heap usage: 3 allocs, 3 frees, 1,064 bytes allocated
==1727319== 
==1727319== All heap blocks were freed -- no leaks are possible
==1727319== 
==1727319== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Conclusion
The memory leaks in the program were successfully identified and fixed. The use of Valgrind was instrumental in detecting and verifying the fixes. The program now adheres to proper memory management practices, ensuring no memory is leaked during execution.