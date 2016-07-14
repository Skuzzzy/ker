#include <stdio.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <unistd.h> //STDIN_FILENO
#include <stdlib.h> // int atexit(void (*function)(void)), exit(int)

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
}

void clear_screen() {
    printf("\e[2J");
}

void move_cursor_to(int x, int y) {
    printf("\e[%d;%dH", y, x);
}

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
    current_pos.x = x;
    current_pos.y = y;
    printf("{y:%d x:%d}", current_pos.x, current_pos.y);
}

int main(void){
    int c;

    setup_term_settings();

    while((c=getchar()) != 0x1b) {
        clear_screen();
        putchar(c);
        move_cursor_to(c-'a',c-'a');
    }

    /*clear_screen();*/
    /*c = getchar();*/
    update_cursor_pos();

    return 0;
}
