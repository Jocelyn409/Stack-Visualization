#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

struct frame_node *new_node = NULL;

int main(int argc, char *argv[]) {
    
    size_t read;
    char *check;
    FILE *output;

    if(argc != 2) {
        fprintf(stderr, "Enter only one argument, that being a .bin file.\n");
        return 1;
    }
    else {
        check = strstr(argv[1], ".bin"); // Checks to see if argument provided is a .bin file.
        if(check != NULL) {

            output = fopen(argv[1], "rb"); // Open file.

            if (output == NULL) {
                fprintf(stderr, "Could not open file: %s\n", argv[1]);
                return 1;
            }
            
            while(!feof(output)) {

                new_node = (struct frame_node*)malloc(sizeof(struct frame_node));

                read = fread(new_node, sizeof(struct frame_node), 1, output);
                if(read == 0) {
                    break; // If no values were read, break out of loop. This is needed or else an empty node is printed.
                }

                // Malloc initial and final contents, then read them.
                void **initial = malloc((new_node->size)*sizeof(void *));
                read = fread(initial, new_node->size*sizeof(void *), 1, output);
                void **final = malloc((new_node->size)*sizeof(void *));
                read = fread(final, new_node->size*sizeof(void *), 1, output);

                printf("function: %p, caller: %p, frame pointer: %p\nstack frame: %p-%p, time: %f (%ld-%ld)\n", 
                new_node->curr_function, new_node->caller, new_node->frame_pointer,
                new_node->begin, new_node->end, new_node->total_time, new_node->start_time, new_node->finish_time);
                printf("address range                initial    final\n");

                // needs work
                for(void * addr = new_node->begin; addr >= new_node->end; addr-=4) {
                    printf("%p-%p: %ld | %ld\n", *initial, *final, 
                    (long int)initial, (long int)*final);
                }

                printf("\n");            
            }

            fclose(output);
        }
        else {
            fprintf(stderr, "Incorrect argument. A .bin file is expected.\n");
        }

    }
    
    return 0;
}
