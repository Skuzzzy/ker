#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <unistd.h> //STDIN_FILENO
#include <stdlib.h> // int atexit(void (*function)(void)), exit(int)

char buffer[2056] = "hello world I need my money prontohello worldhello worldhello worldhello worldhello world\nYou ain't got cheese like a nacho\nWinner winner checkin dinner pull out my riiidde\n Tat Tat tat";
/*char buffer[2057] = "dan\nhello\nworld";*/

static struct termios origt, newt;

void cleanup_term_settings(void);
void setup_term_settings(void) {
    if(tcgetattr(STDIN_FILENO, &origt) == -1) {
        printf("Failed to get original terminal settings.");
        exit(-1);
    }
    atexit(cleanup_term_settings);
    /*now the settings will be copied*/
    newt = origt;

    /*ICANON normally takes care that one line at a time will be processed
     *     that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON | ECHO); // FIXME ECHO

    /*Those new settings will be set to STDIN
     *     TCSANOW tells tcsetattr to change attributes immediately. */
    if(tcsetattr( STDIN_FILENO, TCSANOW, &newt) == -1) {
        printf("Failed to set up our terminal settings");
    }

}

void cleanup_term_settings(void) {
    if(tcsetattr(STDIN_FILENO, TCSANOW, &origt) == -1) {
        printf("Failed to reset terminal to original settings.");
        exit(-1);
    }
    putchar('\n');
}

void clear_screen() {
    printf("\e[2J");
}

void move_cursor_to(int x, int y) {
    printf("\e[%d;%dH", y, x);
}

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
    update_window_dimensions();

    clear_screen();

    int bufferlen = 0;
    while(buffer[bufferlen++] != '\0') {}
    /*printf("%d", bufferlen);*/

    int buffer_index = 0;
    int break_index = 0;


    while(buffer_index < bufferlen) {
        while(buffer[break_index] != '\n' && buffer[break_index] != '\0') { break_index++; }
        int newline_p = (buffer[break_index] == '\n'); //FIXME Should be bool
        int print_index = ((break_index - buffer_index) > win_dimen.cols) ? buffer_index + win_dimen.cols : break_index;
        //Replace with proper print
        /*printf("%d", buffer_index);*/
        /*printf(" %d", print_index);*/
        /*printf(" %d\n", break_index);*/
        for(int i=buffer_index; i < print_index; i++) {
            putchar(buffer[i]);
        }
        if(newline_p) {
            putchar('\n');
        }

        fflush(stdout);

        buffer_index = break_index = ++print_index;
    }
}

int main(void) {
    int c;

    setup_term_settings();

    /*while((c=getchar()) != 0x1b) {*/
        /*clear_screen();*/
        /*putchar(c);*/
        /*move_cursor_to(c-'a',c-'a');*/
    /*}*/

    /*clear_screen();*/
    update_cursor_pos();
    print_buffer_in_area(buffer, 0, 0, -1, -1);
    getchar();

    return 0;
}
