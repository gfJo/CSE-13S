#include "universe.h"
#include <inttypes.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) calloc(rows, sizeof(Universe)); // create a universe.
    u->grid = (bool **) calloc(rows, sizeof(uint32_t *));
    for (uint32_t r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    return u;
}

void uv_delete(Universe *u) {
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && r < u->rows) {
        if (c >= 0 && c < u->cols) {
            u->grid[r][c] = true;
        }
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && r < u->rows) {
        if (c >= 0 && c < u->cols) {
            u->grid[r][c] = false;
        }
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && r < u->rows) {
        if (c >= 0 && c < u->cols) {
            return u->grid[r][c];
        }
    }
    return false;
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r, c;
    while (fscanf(infile, "%" PRIu32 "%" PRIu32, &r, &c) != EOF) {
        if (r < 0 || r >= u->rows) {
            return -1;
        }
        if (c < 0 || c >= u->cols) {
            return -1;
        }
        uv_live_cell(u, r, c);
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && r < u->rows) { // Check if the grid exits.
        if (c >= 0 && c < u->cols) {
            uint32_t num = 0;
            if (u->toroidal) { // Check if toroidal or not.
                uint32_t top = (r + u->rows - 1) % u->rows;
                uint32_t bot = (r + u->rows + 1) % u->rows;
                uint32_t left = (c + u->cols - 1) % u->cols;
                uint32_t right = (c + u->cols + 1) % u->cols;

                uint32_t rs[] = { top, r, bot };
                uint32_t cs[] = { left, c, right };

                for (uint32_t i = 0; i < 3; i += 1) {
                    for (uint32_t j = 0; j < 3; j += 1) {
                        if (rs[i] != r || cs[j] != c) {
                            if (u->grid[rs[i]][cs[j]]) {
                                num += 1;
                            }
                        }
                    }
                }
                return num;
            }

            // For flat universe.
            else {
                for (int i = -1; i < 2; i += 1) {
                    for (int j = -1; j < 2; j += 1) {
                        int x = r + i;
                        int y = c + j;
                        if (x >= 0 && x < (int) u->rows && y >= 0 && y < (int) u->cols) {
                            if (x != (int) r || y != (int) c) {
                                if (u->grid[r + i][c + j]) {
                                    num += 1;
                                }
                            }
                        }
                    }
                }

                return num;
            }
        }
    }
    return -1;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; i += 1) {
        for (uint32_t j = 0; j < u->cols; j += 1) {
            if (u->grid[i][j]) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
}
