#include "universe.h"

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define TOROIDAL    false
#define GENERATIONS 100
#define DELAY       50000

int main(int argc, char **argv) {

    bool toroidal = TOROIDAL;
    uint32_t rows = 0;
    uint32_t cols = 0;
    uint32_t generations = GENERATIONS;
    char *infile_n = NULL;
    char *outfile_n = NULL;
    FILE *infile = stdin;
    FILE *outfile = stdout;

    bool test_s = true;
    bool test_i = false;
    bool test_o = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;

        case 's': test_s = false; break;

        case 'n': generations = atoi(optarg); break;

        case 'i':
            test_i = true;
            infile_n = optarg;
            break;

        case 'o':
            test_o = true;
            outfile_n = optarg;
            break;
        }
    }

    if (test_i) {
        infile = fopen(infile_n, "r");
        fscanf(infile, "%" PRIu32 "%" PRIu32, &rows, &cols);
    }

    Universe *u_A = uv_create(rows, cols, toroidal);
    Universe *u_B = uv_create(rows, cols, toroidal);

    uv_populate(u_A, infile);

    if (test_s) {
        initscr();
        curs_set(FALSE);

        for (uint32_t i = 0; i < generations; i += 1) {
            clear();
            // Display universe A.
            for (uint32_t row = 0; row < uv_rows(u_A); row += 1) {
                for (uint32_t col = 0; col < uv_cols(u_A); col += 1) {
                    if (uv_get_cell(u_A, row, col)) {
                        mvprintw(row, col, "o");
                    }
                }
            }
            refresh();
            usleep(DELAY);

            // Perform one generation.
            for (uint32_t row = 0; row < uv_rows(u_A); row += 1) {
                for (uint32_t col = 0; col < uv_rows(u_A); col += 1) {
                    uint32_t live_neighbor = uv_census(u_A, row, col);
                    bool survive = uv_get_cell(u_A, row, col);
                    if (survive) {
                        if (live_neighbor == 2 || live_neighbor == 3) {
                            uv_live_cell(u_B, row, col);
                            printf("%" PRIu32 " %" PRIu32 "\n", row, col);
                        } else {
                            uv_dead_cell(u_B, row, col);
                        }
                    } else {
                        if (live_neighbor == 3) {
                            uv_live_cell(u_B, row, col);
                        } else {
                            uv_dead_cell(u_B, row, col);
                        }
                    }
                }
            }

            Universe *t = &(*u_A);
            u_A = &(*u_B);
            u_B = t;
        }
        endwin();
    }

    if (test_o) {
        outfile = fopen(outfile_n, "w");
    }

    uv_print(u_A, outfile);

    uv_delete(u_A);
    uv_delete(u_B);

    fclose(infile);
    fclose(outfile);

    return 0;
}
