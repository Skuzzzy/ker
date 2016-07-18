#include "gap.c"

typedef struct {
    gbuff * gbuffer;
    int goal_col;
    int col;
    int row;
    int index;
} gctl;

char gctl_dref(gctl* gcontrol, int index);

gctl* gctl_alloc(char* buffer) {
    gctl* gcontrol = calloc(1, sizeof(gctl));

    gbuff* gbuffer = gbuff_alloc(24);
    gbuff_init(gbuffer, buffer);

    gcontrol->gbuffer = gbuffer;
    gcontrol->row = 0;
    gcontrol->col = 0;
    gcontrol->index = 0;
    gcontrol->goal_col = gcontrol->col;
    return gcontrol;
}

void gctl_free(gctl* gcontrol) {
    gbuff_free(gcontrol->gbuffer);
    free(gcontrol);
}

void gctl_right(gctl* gcontrol) {
    int max_shift = gcontrol->gbuffer->bufsize - gcontrol->gbuffer->gap_end;
    if (max_shift > 0) {
        if(gctl_dref(gcontrol, gcontrol->index) == '\n') {
            gcontrol->row++;
            gcontrol->col = 0;
            gcontrol->goal_col = gcontrol->col;
        }
        else {
            gcontrol->col++;
        }

        gcontrol->goal_col = gcontrol->col;
        gcontrol->index++;
        gbuff_shift(gcontrol->gbuffer, 1);
    }
}

void gctl_discover_col(gctl* gcontrol) {
    int new_col = 1;
    while(gcontrol->index - new_col > 0 && gctl_dref(gcontrol, gcontrol->index - new_col) != '\n') {
        new_col++;
    }
    gcontrol->col = new_col - 1;
}

void gctl_left(gctl* gcontrol) {
    int max_shift = gcontrol->gbuffer->gap_start;
    if (max_shift > 0) {
        gcontrol->index--;
        if(gctl_dref(gcontrol, gcontrol->index) == '\n') {
            gcontrol->row--;
            gctl_discover_col(gcontrol);
        }
        else {
            gcontrol->col--;
        }

        gcontrol->goal_col = gcontrol->col;
        gbuff_shift(gcontrol->gbuffer, -1);
    }
}

void gctl_up(gctl* gcontrol) {
    int orig_row = gcontrol->row;
    int goal_col = gcontrol->goal_col;
    while (gcontrol->row != 0 && gcontrol->row == orig_row) {
        gctl_left(gcontrol);
    }

    // Now attempt to move cursor to goal_col
    while(goal_col < gcontrol->col) {
        gctl_left(gcontrol);
    }
    gcontrol->goal_col = goal_col;
}

void gctl_down(gctl* gcontrol) {
    int orig_row = gcontrol->row;
    int goal_col = gcontrol->goal_col;
    while (1 && gcontrol->row == orig_row) { // FIXME Check to make sure we do not go down on the last row
        gctl_right(gcontrol);
    }

    // Now attempt to move cursor to goal_col
    // FIXME there should be a cur_col_len field
    while(gctl_dref(gcontrol, gcontrol->index) != '\n' && gcontrol->col < goal_col) {
        gctl_right(gcontrol);
    }
    gcontrol->goal_col = goal_col;
}
void gctl_ins(gctl* gcontrol, char c) {
    if (c == '\n') {
        gcontrol->row++;
        gcontrol->col = 0;
    }
    else {
        gcontrol->col++;
    }
    gcontrol->index++;
    gcontrol->goal_col = gcontrol->col;

    gbuff_ins(gcontrol->gbuffer, c);
}

void gctl_del(gctl* gcontrol) {
    if (gcontrol->index == 0) {
        return;
    }

    gcontrol->index--;
    if (gctl_dref(gcontrol, gcontrol->index) == '\n') {
        gcontrol->row--;
        gctl_discover_col(gcontrol);
    }
    else {
        gcontrol->col--;
    }

    gcontrol->goal_col = gcontrol->col;
    gbuff_del(gcontrol->gbuffer);

}

void gctl_write_to_file(gctl* gcontrol, char* filename) {
    gbuff_write_to_file(gcontrol->gbuffer, filename);
}


char gctl_dref(gctl* gcontrol, int index) {
    // linear referencing of gbuffer that skips the gap
    index = index < gcontrol->gbuffer->gap_start ? index : index + gbuff_gap_size(gcontrol->gbuffer);
    return gcontrol->gbuffer->buffer[index];
}

int gctl_size(gctl* gcontrol) {
    // Size of gbuffer without the gap
    return gcontrol->gbuffer->bufsize - gbuff_gap_size(gcontrol->gbuffer);
}

int gctl_gbuff_index(gctl* gcontrol) {
    // Where does the gap start?
    // For cursor drawing purposes
    return gcontrol->gbuffer->gap_start;
}

