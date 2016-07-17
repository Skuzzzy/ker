
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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
    return gapbuffer;
}

int gbuff_gap_size(gbuff* gbuffer) {
    return gbuffer->gap_end - gbuffer->gap_start;
}

char* gbuff_extract_buffer(gbuff* gbuffer) {
    char* buffer = calloc(gbuffer->bufsize - gbuff_gap_size(gbuffer), sizeof(char));
    // Copy first half in
    memcpy(
            buffer,
            gbuffer->buffer,
            gbuffer->gap_start
            );
    // Copy second half in
    memcpy(
            buffer + (gbuffer->gap_start),
            gbuffer->buffer + (gbuffer->gap_end),
            gbuffer->bufsize - gbuffer->gap_end
            );
    return buffer;
}

// FIXME I should really force files to end with \n
int gbuff_write_to_file(gbuff* gbuffer, char* filename) {
    FILE *fp;
    fp = fopen(filename, "w+");
    char * raw_buffer = gbuff_extract_buffer(gbuffer);
    fputs(raw_buffer, fp);
    free(raw_buffer);
    fclose(fp);
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


void gbuff_expand(gbuff* gbuffer) {
    gbuffer->buffer = realloc(gbuffer->buffer, gbuffer->bufsize + gbuffer->goal_gap_size);
    memmove(
            gbuffer->buffer+gbuffer->gap_end+gbuffer->goal_gap_size,
            gbuffer->buffer+gbuffer->gap_end,
            gbuffer->bufsize - gbuffer->gap_end
            );
    gbuffer->bufsize += gbuffer->goal_gap_size;
    gbuffer->gap_end += gbuffer->goal_gap_size;
}

void gbuff_ins(gbuff* gbuffer, char c) {
    gbuffer->buffer[gbuffer->gap_start++] = c;
    if(gbuffer->gap_start == gbuffer->gap_end) {
        gbuff_expand(gbuffer);
    }
}

void gbuff_del(gbuff* gbuffer) {
    if(gbuffer->gap_start != 0) {
        gbuffer->gap_start--;
    }
}

void gbuff_SL(gbuff* gbuffer, int repetitions) {
    for(int counter=0; counter < repetitions; counter++) {
        if(gbuffer->gap_start != 0) {
            gbuffer->buffer[--gbuffer->gap_end] = gbuffer->buffer[--gbuffer->gap_start];
        }
        else {
            break;
        }
    }
}

void gbuff_SR(gbuff* gbuffer, int repetitions) {
    for(int counter=0; counter < repetitions; counter++) {
        if(gbuffer->gap_end < gbuffer->bufsize) {
            gbuffer->buffer[gbuffer->gap_start++] = gbuffer->buffer[gbuffer->gap_end++];
        }
        else {
            break;
        }
    }
}

void gbuff_free(gbuff* gbuffer) {
    free(gbuffer->buffer);
    free(gbuffer);
}


void gbuff_shift(gbuff* gbuffer, int shift_offset) {
    if(shift_offset == 0) {
        return;
    }
    else if (shift_offset > 0) {
        // FIXME Not sure which is faster
        /*gbuff_SR(gbuffer, shift_offset);*/

        shift_offset = MIN(shift_offset, gbuffer->bufsize - gbuffer->gap_end);
        memmove(
                gbuffer->buffer + gbuffer->gap_start,
                gbuffer->buffer + gbuffer->gap_end,
                shift_offset
                );
        gbuffer->gap_start += shift_offset;
        gbuffer->gap_end += shift_offset;

    }
    else if (shift_offset < 0) {
        // FIXME Not sure which is faster
        /*gbuff_SL(gbuffer, -shift_offset);*/
        shift_offset = MIN(-shift_offset, gbuffer->gap_start);
        memmove(
                gbuffer->buffer + gbuffer->gap_end - shift_offset,
                gbuffer->buffer + gbuffer->gap_start - shift_offset,
                shift_offset
                );
        gbuffer->gap_start -= shift_offset;
        gbuffer->gap_end -= shift_offset;
    }
}

/*int main() {*/
    /*char buffer[1024] = "Hello";*/
    /*gbuff * text = gbuff_alloc(25);*/
    /*gbuff_init(text, buffer);*/

    /*char c;*/
    /*gbuff_print(text);*/
    /*while((c=getchar()) != '\e') {*/
        /*if(c=='\'') {*/
            /*gbuff_del(text);*/
        /*} else if (c == '<') {*/
            /*[>gbuff_SL(text,3);<]*/
            /*gbuff_shift(text,-3);*/
        /*} else if (c == '>') {*/
            /*[>gbuff_SR(text,3);<]*/
            /*gbuff_shift(text,5);*/
        /*} else if (c == '1') {*/
            /*printf("YOLO");*/
            /*gbuff_write_to_file(text, "/tmp/test.txt");*/
        /*} else if (c == '\n') {*/
        /*} else {*/
            /*gbuff_ins(text, c);*/
        /*}*/

        /*gbuff_print(text);*/
    /*}*/

    /*gbuff_free(text);*/


/*}*/
