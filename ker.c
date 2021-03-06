#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <unistd.h> //STDIN_FILENO
#include <stdlib.h> // int atexit(void (*function)(void)), exit(int)
#include "drawinfo.c"

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
    newt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    newt.c_iflag &= ~(IXON);

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

void print_gcontrol_area(gctl* gcontrol, int offset, int s_x, int s_y, int e_x, int e_y) {

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

// http://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c
long slurp(char const* path, char **buf)
{
    FILE  *fp;
    size_t fsz;
    long   off_end;
    int    rc;

    /* Open the file */
    fp = fopen(path, "rb");
    if( NULL == fp ) {
        return -1L;
    }

    /* Seek to the end of the file */
    rc = fseek(fp, 0L, SEEK_END);
    if( 0 != rc ) {
        return -1L;
    }

    /* Byte offset to the end of the file (size) */
    if( 0 > (off_end = ftell(fp)) ) {
        return -1L;
    }
    fsz = (size_t)off_end;

    /* Allocate a buffer to hold the whole file */
    *buf = malloc( fsz );
    if( NULL == *buf ) {
        return -1L;
    }

    /* Rewind file pointer to start of file */
    rewind(fp);

    /* Slurp file into buffer */
    if( fsz != fread(*buf, 1, fsz, fp) ) {
        free(*buf);
        return -1L;
    }

    /* Close the file */
    if( EOF == fclose(fp) ) {
        free(*buf);
        return -1L;
    }

    /* Return the file size */
    return (long)fsz;
}

int main(int argc, char** argv) {
    char c;

    setup_term_settings();
    update_window_dimensions();


    /*c = getchar();*/
    /*printf("%d\n", (int)c);*/
    /*fflush(stdout);*/

    long  file_size;
    char* buf;

    if(argc < 2) {
        printf("WE NEED THEM ARGUMENTS BRUH");
        exit(-1);
    }

    file_size = slurp(argv[1], &buf);

    if(file_size == -1L) {
        buf = calloc(1, sizeof(char));
        buf[0] = '\0';
    }

    /*gbuff * text = gbuff_alloc(25);*/
    /*gbuff_init(text, buf);*/
    gctl* gcontrol = gctl_alloc(buf);
    drinfo* dr_left = drinfo_alloc();
    drinfo* dr_right = drinfo_alloc();
    /*drinfo_shift_draw_line(dr_right, gcontrol, win_dimen.rows);*/
    /*gbuff_shift(text, 10);*/
    /*buf = buffer;*/

    /*clear_screen();*/
    /*update_cursor_pos();*/
    /*printf("%d %d", win_dimen.cols, win_dimen.rows);*/
    /*print_buffer_in_area(buf, 1, 1, win_dimen.cols, win_dimen.rows);*/

    clear_screen();
    print_gcontrol_area(gcontrol, dr_right->draw_offset, 82, 1, win_dimen.cols+1, win_dimen.rows);
    print_gcontrol_area(gcontrol, dr_left->draw_offset, 1, 1, 80, win_dimen.rows);

    int cur_pos = 0;
    while((c = getchar()) != '`') {

        /*if (0) {*/
        if(c == 'J') {
            int actual = drinfo_shift_draw_line(dr_left, gcontrol, 1);
            drinfo_shift_draw_line(dr_right, gcontrol, actual);
        } else if (c == 'K') {
            int actual = drinfo_shift_draw_line(dr_left, gcontrol, -1);
            drinfo_shift_draw_line(dr_right, gcontrol, actual);
        } else if (c == 3 || c == 24) { // CTRL C / CTRL X
            break;
        } else if (c == 19) { // CTRL S
            gctl_write_to_file(gcontrol, argv[1]);
        } else if (c == 127) {
            gctl_del(gcontrol);
            if(gcontrol->index < dr_left->draw_offset) { dr_left->draw_offset--; }
            if(gcontrol->index < dr_right->draw_offset) { dr_right->draw_offset--; }
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
            if(gcontrol->index < dr_left->draw_offset) { dr_left->draw_offset++; }
            if(gcontrol->index < dr_right->draw_offset) { dr_right->draw_offset++; }

            // If the cursor position is before one of the craw cursors, they much be shifted
            /*[>continue;<]*/
        }


        /*[>char * buffoffset = buf+cur_pos;<]*/
        /*[>print_buffer_in_area(buffoffset, 1, 1, win_dimen.cols, win_dimen.rows);<]*/
        /*print_gapbuffer_area(text, cur_pos, 1, 1, win_dimen.cols, win_dimen.rows);*/
        clear_screen();
        print_gcontrol_area(gcontrol, dr_right->draw_offset, 82, 1, win_dimen.cols+1, win_dimen.rows);
        print_gcontrol_area(gcontrol, dr_left->draw_offset, 1, 1, 80, win_dimen.rows);

        /*printf("%d, %d", dr_right->draw_offset, gcontrol->index);*/
        /*[>printf("%d", c);<]*/
    }

    gctl_free(gcontrol);

    return 0;
}
