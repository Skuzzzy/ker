#include "gctl.c"

typedef struct {
    int draw_offset;
} drinfo;

drinfo* drinfo_alloc(void) {
    drinfo* drawinfo = calloc(1, sizeof(drinfo));
    drawinfo->draw_offset = 0;
    return drawinfo;
}

int drinfo_shift_draw_line(drinfo* drawinfo, gctl* gcontrol, int amount) {
    int actual_shift = 0;
    if(amount > 0) {
        //FIXME check to make sure we do not overrun the bottom
        for(int repetition = 0; repetition < amount; repetition++) {
            while((gctl_dref(gcontrol, drawinfo->draw_offset) != '\n')) {
                drawinfo->draw_offset++;
            }
            drawinfo->draw_offset++;
            actual_shift++;
        }
    }
    else if (amount < 0) {
        amount = -amount;
        for(int repetition = 0; repetition < amount; repetition++) {
            if(drawinfo->draw_offset > 0) {
                drawinfo->draw_offset--; // Move back into the \n
                while(drawinfo->draw_offset > 0 && gctl_dref(gcontrol, drawinfo->draw_offset - 1) != '\n') {
                    drawinfo->draw_offset--;
                }
            }
            else {
                break;
            }

            actual_shift--;
        }
    }

    return actual_shift;

}
