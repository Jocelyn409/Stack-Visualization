#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

struct frame_node {
    void *begin, *end;
    void *curr_function, *caller;
    void *frame_pointer;
    void **initial, **final;
    size_t size;
    clock_t start_time, finish_time;
    double total_time;
    struct frame_node *next;
};

struct frame_node *head_node = NULL;
struct frame_node *current_node = NULL;
struct frame_node *new_node = NULL;

/*
* Creates a new node, assigns values to it, then adds it to the end of the LL. 
* @param *this_fn: Takes the *this_fn value from the __cyg_profile_func_enter() function to assign to new node.
* @param *call_site: Takes the *call_site value from the __cyg_profile_func_enter() function to assign to new node.
*/
void add_frame_node(void *this_fn, void *call_site) {

    new_node = (struct frame_node*)malloc(sizeof(struct frame_node));

    new_node->start_time = clock();
    new_node->begin = __builtin_frame_address(1) + (2*sizeof(void *));
    new_node->end = __builtin_frame_address(0) + (4*sizeof(void *));
    new_node->frame_pointer = __builtin_frame_address(1);
    new_node->curr_function = this_fn;
    new_node->caller = call_site;
    new_node->size = (new_node->begin-new_node->end);
    new_node->initial = malloc((new_node->size)*sizeof(void *));
    new_node->final = malloc((new_node->size)*sizeof(void *));
    memcpy(new_node->initial, (new_node->end), new_node->size*sizeof(void *));
    
    if(head_node == NULL) {
        head_node = new_node; // If LL is empty, the new node will be the head node.
    }
    else {
        current_node = head_node; // Assign current_node to start of LL (head_node).
        while(current_node->next != NULL) {
            current_node = current_node->next; // Bring current_node to be at the very end of the linked list.
        }
        current_node->next = new_node; // Add new_node to end of LL.
        new_node->next = NULL; // Ensures "next" value is NULL (this can cause issues if it isn't specified).
    }

}

/*
* Finds the corresponding node created in add_frame_node(), then assigns values to it.
* @param *call_site: Takes the *call_site value from the __cyg_profile_func_exit() function to find the corresponding node with.
*/
void find_frame_node(void *call_site) {

    struct frame_node *node_pointer = NULL;

    current_node = head_node; // Assign current_node to the start of the LL.

    // This while loop finds the most recently created node with matching call_site values that hasn't already been written to.
    // The result is that it finds the correct corresponding node, inspite of the fact that there can be multiple nodes with the same call_site values.
    while(current_node != NULL) {

        if(current_node->caller == call_site && current_node->finish_time <= 0) {
            node_pointer = current_node; // A node that matches call_site values and hasn't been writen to was found.
        }

        if(current_node->next == NULL) {
            break; // If at end of LL, break.
        }
        else {
            current_node = current_node->next; // Else continue along the LL.
        }

    }

    node_pointer->finish_time = clock();
    node_pointer->total_time = (double) ((node_pointer->finish_time)-(node_pointer->start_time)) / CLOCKS_PER_SEC;
    memcpy(node_pointer->final, (node_pointer->end), node_pointer->size*sizeof(void *));
}

void dump_linked_list() {

    current_node = head_node; // Assign current_node to the start of the LL. 
    size_t write;
    FILE *input;

    // Open file with "wb", where "w" deletes all contents in the file, allowing the entire linked list to be printed again without duplicate values.
    input = fopen("stack.bin", "wb");
    if (input == NULL) {
        fprintf(stderr, "Could not open file: stack.bin");
        exit(1);
    }

    // Write entire LL node by node with each node's respective initial and final values, too.
    while(current_node != NULL) {
        write = fwrite(current_node, sizeof(struct frame_node), 1, input);
        write = fwrite(current_node->initial, current_node->size*sizeof(void *), 1, input);
        write = fwrite(current_node->final, current_node->size*sizeof(void *), 1, input);
        current_node = current_node->next; // Move to next value in LL.
    }
    fclose(input);
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) {
    add_frame_node(this_fn, call_site);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
    find_frame_node(call_site);
    dump_linked_list();
}
