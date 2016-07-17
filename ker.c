#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <unistd.h> //STDIN_FILENO
#include <stdlib.h> // int atexit(void (*function)(void)), exit(int)
/*#include "gap.c"*/
#include "gctl.c"

/*char buffer[2057] = "dan\n\nhello\n\nworld\ntest\ntest";*/
/*char asd[2056] = "hello world I need my money prontohello worldhello! \nYou ain't got cheese like a nacho\nWinner winner checkin dinner pull out my riiidde how much for one of those onwes! They look great how do I get one of those this line needs to be super long in order to forcing a wrap. DONE~!\n Tat Tat tat";*/

static struct termios origt, newt;

typedef struct {
    int cols;
    int rows;
} dimen;

dimen win_dimen;

typedef struct {
    int x;
    int y;
} coords;

coords current_pos;

void cleanup_term_settings(void);
void clear_screen(void);
void setup_term_settings(void) {
    if(tcgetattr(STDIN_FILENO, &origt) == -1) {
        printf("Failed to get original terminal settings.");
        exit(-1);
    }
    atexit(cleanup_term_settings);

    newt = origt;
    newt.c_lflag &= ~(ICANON | ECHO);

    if(tcsetattr( STDIN_FILENO, TCSANOW, &newt) == -1) {
        printf("Failed to set up our terminal settings");
    }

    clear_screen();
}

void cleanup_term_settings(void) {
    if(tcsetattr(STDIN_FILENO, TCSANOW, &origt) == -1) {
        printf("Failed to reset terminal to original settings.");
        exit(-1);
    }
    putchar('\n');
}

void clear_screen(void) {
    printf("\e[2J");

}

void move_cursor_to(int x, int y) {
    printf("\e[%d;%dH", y, x);
}

void push_cursor_pos(void) {
    printf("\e[s");
}

void pop_cursor_pos(void) {
    printf("\e[u");
}

void update_cursor_pos(void) {
    fseek(stdin,0,SEEK_END); // Clobber stdin so that we can retrieve x and y values
    printf("\e[6n");
    int x, y;
    getchar();
    getchar();
    scanf("%d", &x);
    getchar();
    scanf("%d", &y);
    getchar();
    current_pos.x = x;
    current_pos.y = y;
    /*printf("{y:%d x:%d}", current_pos.x, current_pos.y);*/
}

void update_window_dimensions(void) {
    struct winsize dimens;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimens);
    win_dimen.cols = dimens.ws_col;
    win_dimen.rows = dimens.ws_row;
    /*printf ("lines %d\n", dimens.ws_row);*/
    /*printf ("columns %d\n", dimens.ws_col);*/
}

/*void print_buffer_in_area(char * buffer, int s_x, int s_y, int e_x, int e_y) {*/

    /*clear_screen();*/
    /*move_cursor_to(s_x,s_y);*/

    /*int current_y = s_y;*/

    /*int width = e_x - s_x;*/
    /*[>printf("%d", width);<]*/
    /*update_window_dimensions();*/


    /*int bufferlen = 0;*/
    /*while(buffer[bufferlen++] != '\0') {}*/
    /*[>printf("%d", bufferlen);<]*/

    /*int buffer_index = 0;*/
    /*int break_index = 0;*/

    /*int temptemp = temp_lino;*/

    /*while(buffer_index < bufferlen && current_y <= e_y) {*/
        /*while(buffer[break_index] != '\n' && buffer[break_index] != '\0') { break_index++; }*/
        /*int newline_p = (buffer[break_index] == '\n');*/
        /*int print_index = ((break_index - buffer_index) > width) ? buffer_index + width : break_index;*/
        /*[>Replace with proper print<]*/
        /*[>printf("%d", buffer_index);<]*/
        /*[>printf(" %d", print_index);<]*/
        /*[>printf(" %d\n", break_index);<]*/
        /*[>printf("%d, %d\n", buffer_index, print_index);<]*/

        /*printf("%d|", temptemp++);*/

        /*for(int i=buffer_index; i < print_index; i++) {*/
            /*putchar(buffer[i]);*/
        /*}*/
        /*[>if(newline_p) {<]*/
            /*[>putchar('\n');<]*/
        /*[>}<]*/
        /*[>getchar();<]*/

        /*move_cursor_to(s_x, ++current_y);*/
        /*buffer_index = ++break_index;*/
    /*}*/

    /*[>printf("%d, %d", current_y, e_y);<]*/

    /*fflush(stdout);*/
/*}*/

/*void print_gapbuffer_in_area(gbuff* gbuffer, int offset, int s_x, int s_y, int e_x, int e_y) {*/

    /*clear_screen();*/
    /*move_cursor_to(s_x,s_y);*/

    /*int current_y = s_y;*/

    /*int width = e_x - s_x;*/
    /*[>printf("%d", width);<]*/
    /*update_window_dimensions();*/


    /*[>int bufferlen = 0;<]*/
    /*[>while(buffer[bufferlen++] != '\0') {}<]*/
    /*[>printf("%d", bufferlen);<]*/

    /*int buffer_index = 0;*/
    /*int break_index = 0;*/

    /*int temptemp = temp_lino;*/

    /*while(buffer_index < gbuffer->bufsize && current_y <= e_y) {*/
        /*while((gbuffer->buffer[break_index+offset] != '\n' && gbuffer->buffer[break_index+offset] != '\0') || gbuff_within_gap(gbuffer, break_index+offset)) { break_index++; }*/
        /*[>int newline_p = (buffer[break_index] == '\n');<]*/
        /*[>Replace with proper print<]*/
        /*[>printf("%d", buffer_index);<]*/
        /*[>printf(" %d", print_index);<]*/
        /*[>printf(" %d\n", break_index);<]*/
        /*[>printf("%d, %d\n", buffer_index, print_index);<]*/

        /*int expendable_chars = width;*/

        /*printf("%d|", temptemp++);*/
        /*expendable_chars -= 4;*/

        /*for(int i=buffer_index; i < break_index; i++) {*/
            /*if(i==gbuffer->gap_end) {*/
                /*push_cursor_pos();*/
            /*}*/
            /*if(gbuff_within_gap(gbuffer, i+offset)) { putchar('_'); continue; }*/
            /*[>if(gbuff_within_gap(gbuffer, i+offset)) { continue; }<]*/
            /*if(expendable_chars <= 0) { break; }*/
            /*putchar(gbuffer->buffer[i+offset]);*/
            /*expendable_chars--;*/
        /*}*/
        /*if(break_index==gbuffer->gap_end) {*/
            /*push_cursor_pos();*/
        /*}*/
        /*[>if(newline_p) {<]*/
            /*[>putchar('\n');<]*/
        /*[>}<]*/
        /*[>getchar();<]*/

        /*move_cursor_to(s_x, ++current_y);*/
        /*buffer_index = ++break_index;*/
    /*}*/

    /*[>printf("%d, %d", current_y, e_y);<]*/

    /*pop_cursor_pos();*/

    /*fflush(stdout);*/
/*}*/

// FIXME IMPROVED
/*void print_gapbuffer_area(gbuff* gbuffer, int offset, int s_x, int s_y, int e_x, int e_y) {*/

    /*clear_screen();*/
    /*move_cursor_to(s_x,s_y);*/

    /*int current_y = s_y;*/

    /*int width = e_x - s_x;*/
    /*update_window_dimensions();*/

    /*int buffer_index = 0;*/
    /*int break_index = 0;*/

    /*while(buffer_index < gbuffer->bufsize && current_y <= e_y) {*/
        /*while((gbuffer->buffer[break_index+offset] != '\n' && gbuffer->buffer[break_index+offset] != '\0') ||*/
               /*gbuff_within_gap(gbuffer, break_index+offset)) { break_index++; }*/

        /*int expendable_chars = width;*/

        /*for(int i=buffer_index; i < break_index; i++) {*/
            /*if(i==gbuffer->gap_end) {*/
                /*push_cursor_pos();*/
            /*}*/
            /*[>if(gbuff_within_gap(gbuffer, i+offset)) { putchar('_'); continue; }<]*/
            /*if(gbuff_within_gap(gbuffer, i+offset)) { continue; }*/
            /*if(expendable_chars <= 0) { break; }*/
            /*putchar(gbuffer->buffer[i+offset]);*/
            /*expendable_chars--;*/
        /*}*/
        /*if(break_index==gbuffer->gap_end) {*/
            /*push_cursor_pos();*/
        /*}*/

        /*move_cursor_to(s_x, ++current_y);*/
        /*buffer_index = ++break_index;*/
    /*}*/

    /*pop_cursor_pos();*/

    /*fflush(stdout);*/
/*}*/

void print_gcontrol_area(gctl* gcontrol, int offset, int s_x, int s_y, int e_x, int e_y) {

    clear_screen();
    move_cursor_to(s_x,s_y);

    int current_y = s_y;

    int width = e_x - s_x;
    update_window_dimensions(); // FIXME do we use this?

    int buffer_index = offset;
    int break_index = buffer_index;

    while(buffer_index < gctl_size(gcontrol) && current_y <= e_y) {
        while(gctl_dref(gcontrol, break_index)  != '\n' && gctl_dref(gcontrol, break_index) != '\0') { break_index++; }

        int expendable_chars = width;

        while(buffer_index < break_index) {
            if(buffer_index == gcontrol->index) {
                push_cursor_pos();
            }
            if(expendable_chars <= 0) { break; }
            putchar(gctl_dref(gcontrol, buffer_index));
            expendable_chars--;
            buffer_index++;
        }
        if(buffer_index == gcontrol->index) {
            push_cursor_pos();
        }


        move_cursor_to(s_x, ++current_y);
        buffer_index = ++break_index;
    }

    pop_cursor_pos();

    fflush(stdout);
}


long slurp(char const* path, char **buf, int add_nul)
{
    FILE  *fp; size_t fsz; long   off_end; int    rc; /* Open the file */ fp = fopen(path, "rb"); if( NULL == fp ) { return -1L; } /* Seek to the end of the file */ rc = fseek(fp, 0L, SEEK_END); if( 0 != rc ) { return -1L; } /* Byte offset to the end of the file (size) */ if( 0 > (off_end = ftell(fp)) ) { return -1L; } fsz = (size_t)off_end; /* Allocate a buffer to hold the whole file */ *buf = malloc( fsz+(int)add_nul ); if( NULL == *buf ) { return -1L; } /* Rewind file pointer to start of file */ rewind(fp); /* Slurp file into buffer */ if( fsz != fread(*buf, 1, fsz, fp) ) { free(*buf); return -1L; } /* Close the file */ if( EOF == fclose(fp) ) { free(*buf); return -1L; } if( add_nul ) { /* Make sure the buffer is NUL-terminated, just in case */ buf[fsz] = '\0'; } /* Return the file size */ return (long)fsz;
}




int main(void) {
    char c;

    setup_term_settings();
    update_window_dimensions();


    /*c = getchar();*/
    /*printf("%d\n", (int)c);*/
    /*fflush(stdout);*/
    /*c = getchar();*/
    /*printf("%d\n", (int)c);*/
    /*fflush(stdout);*/
    /*c = getchar();*/
    /*printf("%d\n", (int)c);*/
    /*fflush(stdout);*/
    /*c = getchar();*/
    /*printf("%d\n", (int)c);*/
    /*fflush(stdout);*/

    /*while((c=getchar()) != 0x1b) {*/
        /*clear_screen();*/
        /*putchar(c);*/
        /*move_cursor_to(c-'a',c-'a');*/
    /*}*/
    long  file_size;
    char *buf;

    file_size = slurp("ker.c", &buf, 0);

    /*gbuff * text = gbuff_alloc(25);*/
    /*gbuff_init(text, buf);*/
    gctl* gcontrol = gctl_alloc(buf);
    /*gbuff_shift(text, 10);*/
    /*buf = buffer;*/

    /*clear_screen();*/
    /*update_cursor_pos();*/
    /*printf("%d %d", win_dimen.cols, win_dimen.rows);*/
    /*print_buffer_in_area(buf, 1, 1, win_dimen.cols, win_dimen.rows);*/

    print_gcontrol_area(gcontrol, 0, 1, 1, win_dimen.cols, win_dimen.rows);

    int cur_pos = 0;
    while((c = getchar()) != '`') {

        if (0) {
        /*if(c == 'J') {*/
            /*while((text->buffer[cur_pos] != '\n' && text->buffer[cur_pos] != '\0') || gbuff_within_gap(text, cur_pos)) {*/
                /*cur_pos++;*/
            /*}*/
            /*cur_pos++;*/
            /*temp_lino++;*/
        /*} else if (c == 'K') {*/
            /*cur_pos--;*/
            /*cur_pos--;*/
            /*[>[>while(buf[cur_pos] != '\n' && buf[cur_pos] != '\0') {<]<]*/
            /*while((text->buffer[cur_pos] != '\n' && text->buffer[cur_pos] != '\0') || gbuff_within_gap(text, cur_pos)) {*/
                /*cur_pos--;*/
            /*}*/
            /*cur_pos++;*/
            /*temp_lino--;*/
        } else if (c == 127) {
            gctl_del(gcontrol);
        } else if (c == 27) {
            c = getchar();
            if(c == 27) {
                break;
            } else if (c== 91) {
                c = getchar();
                if(c == 68) {
                    gctl_left(gcontrol);
                }
                else if(c == 67) {
                    gctl_right(gcontrol);
                }
                else if(c == 66) {
                    gctl_down(gcontrol);
                }
                else if(c == 65) {
                    gctl_up(gcontrol);
                }
            } else {
                continue;
            }

            // 68 67
        } else {
            /*[>printf("%d", c);<]*/
            gctl_ins(gcontrol, c);
            /*[>continue;<]*/
        }


        /*[>char * buffoffset = buf+cur_pos;<]*/
        /*[>print_buffer_in_area(buffoffset, 1, 1, win_dimen.cols, win_dimen.rows);<]*/
        /*print_gapbuffer_area(text, cur_pos, 1, 1, win_dimen.cols, win_dimen.rows);*/
        print_gcontrol_area(gcontrol, cur_pos, 1, 1, win_dimen.cols, win_dimen.rows);
        /*[>printf("%d", c);<]*/
    }


    return 0;
}
