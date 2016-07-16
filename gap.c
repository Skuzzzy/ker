
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char * buffer;
    int bufsize;
    int gap_start;
    int gap_end;
    int goal_gap_size;
} gbuff;

gbuff* gbuff_alloc(int gapsize) {
    gbuff * gapbuffer = calloc(1, sizeof(gbuff));
    gapbuffer->goal_gap_size = gapsize;
    /*gbuff.gap_start = 0;*/
    /*gbuff.gap_end = 0;*/
    /*gbuff.size_before_gap = strlen(buffer)+1;*/
    /*gbuff.reserved_size = gbuff.size_before_gap * 2;*/
    /*gbuff.buffer = calloc(gbuff.reserved_size, sizeof(char))*/

    return gapbuffer;
}

void gbuff_init(gbuff* gbuffer, char * buffer) {
    int buf_len = strlen(buffer) + 1;
    int gbuffer_size = buf_len + gbuffer->goal_gap_size;
    char * ginitbuff = calloc(gbuffer_size, sizeof(char));
    gbuffer->gap_start = 0; // init gap buffer at index 0 for now...
    gbuffer->gap_end = gbuffer->goal_gap_size;
    strcpy(ginitbuff+gbuffer->gap_end, buffer);
    gbuffer->buffer = ginitbuff;
    gbuffer->bufsize = gbuffer_size;
}

int gbuff_within_gap(gbuff* gbuffer, int index) {
    return (index >= gbuffer->gap_start && index < gbuffer->gap_end);
}

void gbuff_print(gbuff* gbuffer) {
    int index = 0;

    while(index < gbuffer->bufsize) {
        /*printf("%d", gbuff_within_gap(gbuffer, index));*/
        if(gbuff_within_gap(gbuffer, index)) {
            putchar('_');
            fflush(stdout);
        }
        else {
            putchar(gbuffer->buffer[index]);
        }

        index++;
    }
    putchar('\\');
    putchar('0');
    putchar('\n');
}

void gbuff_ins(gbuff* gbuffer, char c) {
    gbuffer->buffer[gbuffer->gap_start++] = c;
}

void gbuff_del(gbuff* gbuffer) {
    gbuffer->gap_start--;
}

void gbuff_SL(gbuff* gbuffer) {
    gbuffer->buffer[--gbuffer->gap_end] = gbuffer->buffer[--gbuffer->gap_start];
}

void gbuff_SR(gbuff* gbuffer) {
    gbuffer->buffer[gbuffer->gap_start++] = gbuffer->buffer[gbuffer->gap_end++];
}

void gbuff_free(gbuff* gbuffer) {
    free(gbuffer->buffer);
    free(gbuffer);
}


int main() {
    char buffer[1024] = "Hello World this is a buffer of characters";
    gbuff * text = gbuff_alloc(5);
    gbuff_init(text, buffer);
    gbuff_print(text);
    gbuff_SR(text);
    gbuff_SR(text);
    gbuff_SR(text);
    gbuff_SR(text);
    gbuff_print(text);
    gbuff_del(text);
    gbuff_print(text);
    gbuff_del(text);
    gbuff_print(text);
    gbuff_ins(text, '!');
    gbuff_print(text);
    gbuff_ins(text, '?');
    gbuff_print(text);

    /*char c;*/
    /*while((c=getchar()) != 'e') {*/
        /*if(c=='d') {*/
            /*gbuff_backspace(text);*/
        /*}*/

    /*}*/

    gbuff_free(text);


}
