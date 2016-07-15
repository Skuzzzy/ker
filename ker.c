#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <unistd.h> //STDIN_FILENO
#include <stdlib.h> // int atexit(void (*function)(void)), exit(int)

int temp_lino = 0;
char buffer[2057] = "dan\n\nhello\n\nworld\ntest\ntest";
char asd[2056] = "hello world I need my money prontohello worldhello! \nYou ain't got cheese like a nacho\nWinner winner checkin dinner pull out my riiidde how much for one of those onwes! They look great how do I get one of those this line needs to be super long in order to forcing a wrap. DONE~!\n Tat Tat tat";

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
    /*now the settings will be copied*/
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

void print_buffer_in_area(char * buffer, int s_x, int s_y, int e_x, int e_y) {

    move_cursor_to(s_x,s_y);

    int current_y = s_y;

    int width = e_x - s_x - 4; // FIXME REMOVE 4, CURRENTLY THERE JUST BECAUSE I DONT ACCOUNT FOR LINE NUMBERS
    /*printf("%d", width);*/
    update_window_dimensions();


    int bufferlen = 0;
    while(buffer[bufferlen++] != '\0') {}
    /*printf("%d", bufferlen);*/

    int buffer_index = 0;
    int break_index = 0;

    int temptemp = temp_lino;

    while(buffer_index < bufferlen && current_y <= e_y) {
        while(buffer[break_index] != '\n' && buffer[break_index] != '\0') { break_index++; }
        int newline_p = (buffer[break_index] == '\n');
        int print_index = ((break_index - buffer_index) > width) ? buffer_index + width : break_index;
        /*Replace with proper print*/
        /*printf("%d", buffer_index);*/
        /*printf(" %d", print_index);*/
        /*printf(" %d\n", break_index);*/
        /*printf("%d, %d\n", buffer_index, print_index);*/

        printf("%d|", temptemp++);

        for(int i=buffer_index; i < print_index; i++) {
            putchar(buffer[i]);
        }
        /*if(newline_p) {*/
            /*putchar('\n');*/
        /*}*/
        /*getchar();*/

        move_cursor_to(s_x, ++current_y);
        buffer_index = ++break_index;
    }

    /*printf("%d, %d", current_y, e_y);*/

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

    /*while((c=getchar()) != 0x1b) {*/
        /*clear_screen();*/
        /*putchar(c);*/
        /*move_cursor_to(c-'a',c-'a');*/
    /*}*/
    long  file_size;
    char *buf;

    file_size = slurp("ker.c", &buf, 0);
    /*buf = buffer;*/

    /*clear_screen();*/
    update_cursor_pos();
    /*printf("%d %d", win_dimen.cols, win_dimen.rows);*/
    print_buffer_in_area(buf, 1, 1, win_dimen.cols, win_dimen.rows);

    int cur_pos = 0;
    int cur_pos2 = 0;
    for(int i=0; i<win_dimen.rows; i++) {
        while(buf[cur_pos2] != '\n' && buf[cur_pos2] != '\0') {
            cur_pos2++;
        }
        cur_pos2++;
    }

    while((c = getchar()) != 0x1b) {

        if(c == 'j') {
            while(buf[cur_pos] != '\n' && buf[cur_pos] != '\0') {
                cur_pos++;
            }
            cur_pos++;

            while(buf[cur_pos2] != '\n' && buf[cur_pos2] != '\0') {
            cur_pos2++;
            }
            cur_pos2++;
            temp_lino++;
        } else if (c == 'k') {
            if(temp_lino <= 0) {continue;}

            cur_pos--;
            cur_pos--;
            while(buf[cur_pos] != '\n' && buf[cur_pos] != '\0') {
                cur_pos--;
            }
            cur_pos++;
            cur_pos2--;
            cur_pos2--;
            while(buf[cur_pos2] != '\n' && buf[cur_pos2] != '\0') {
            cur_pos2--;
            }
            cur_pos2++;

            temp_lino--;
        } else if (c == 'K') {
            if(temp_lino <= 0) {continue;}
            for(int i=0; i<5; i++) {
                cur_pos--;
                cur_pos--;
                while(buf[cur_pos] != '\n' && buf[cur_pos] != '\0') {
                    cur_pos--;
                }
                cur_pos++;
                cur_pos2--;
                cur_pos2--;
                while(buf[cur_pos2] != '\n' && buf[cur_pos2] != '\0') {
                cur_pos2--;
                }
                cur_pos2++;

                temp_lino--;
            }
        }
            else if(c == 'J') {
            for(int i=0; i<5; i++) {
            while(buf[cur_pos] != '\n' && buf[cur_pos] != '\0') {
                cur_pos++;
            }
            cur_pos++;

            while(buf[cur_pos2] != '\n' && buf[cur_pos2] != '\0') {
            cur_pos2++;
            }
            cur_pos2++;
            temp_lino++;
            }
        /*} else if (c == 'c') {*/
            /*clear_screen();*/
            /*getchar();*/
        } else {
            continue;
        }


        char * buffoffset = buf+cur_pos;
        char * buffoffset2 = buf+cur_pos2;
        /*print_buffer_in_area(buffoffset, 1, 1, win_dimen.cols, win_dimen.rows);*/
        clear_screen();
        print_buffer_in_area(buffoffset, 1, 1, 70, win_dimen.rows);
        temp_lino += win_dimen.rows;
        print_buffer_in_area(buffoffset2, 72, 1, win_dimen.cols, win_dimen.rows);
        temp_lino -= win_dimen.rows;
    }

    return 0;
}
