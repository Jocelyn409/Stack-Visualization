#include <stdio.h>

register void *stack_pointer asm ("esp");

/*
* Prints out the stack and frame pointer, plus the contents of the stack starting from the offset and returning back to the stack pointer.
* @param *message: String message, usually to denote where the function is being called.
* @param offset: Denotes the offset of where to start printing from in the stack.
* Function has a void return value.
*/

void print_stack(char *message, int offset) {
    void *v_pointer;
    
    fprintf(stderr, "\nStack (offset: %d): %s\n", offset, message);
    fprintf(stderr, "Stack pointer: %p\n", stack_pointer);
    fprintf(stderr, "Frame pointer: %p\n", __builtin_frame_address(0));
    
    // Starting at offset, this for loop prints out the contents of the offset number in the form of
    // uchar, uint, and pointer values, decreasing until it reaches back to the stack pointer.
    for(offset; offset >= 0; offset--) {
        v_pointer = (stack_pointer+offset); // We are making v_pointer point to sp+offset so it updates each loop, resulting in the address decreasing each loop back up to the stack pointer.
        
        fprintf(stderr, "%p: uchar(%03u), uint(%010u), pointer(0x%016lx)\n", v_pointer, *(unsigned char*)v_pointer, *(unsigned int*)v_pointer, *(unsigned long*)v_pointer);
    }

}
