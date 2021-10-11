#include "dungeon.h"

void monster_cleanup(Level *l) {
    for (int i = l->monsters - 1; i >= 0; i--) {
        l->map[l->m[i].x][l->m[i].y] = ' ';
    }
}

void monster_rules(Level *l) {
    int nextColumn = 0;
    int nextRow = 0;
    for (int i = l->monsters - 1; i >= 0; i--) {
        switch (l->m[i].direction) {
            case RIGHT:
                nextColumn = l->m[i].y + 1;
                nextRow = l->m[i].x;
                break;
            case LEFT:
                nextColumn = l->m[i].y - 1;
                nextRow = l->m[i].x;
                break;
            case UP:
                nextColumn = l->m[i].y;
                nextRow = l->m[i].x - 1;
                break;
            case DOWN:
                nextColumn = l->m[i].y;
                nextRow = l->m[i].x + 1;
                break;
        }
        if (l->map[nextRow][nextColumn] == '#' || l->map[nextRow][nextColumn] == 'A') {
            l->map[l->m[i].x][l->m[i].y] = change_direction(&l->m[i]);
            if (l->p.x == l->m[i].x && l->p.y == l->m[i].y) {
                l->p.dead = 1;
            }
        } else {
            if (l->p.x == nextRow && l->p.y == nextColumn) {
                l->p.dead = 1;
            }
            l->m[i].x = nextRow;
            l->m[i].y = nextColumn;
            l->map[nextRow][nextColumn] = get_direction(&l->m[i]);
        }
        nextColumn = 0;
        nextRow = 0;
    }
}

char get_direction(Monster *m) {
    switch (m->direction) {
        case RIGHT:
            return '>';
        case LEFT:
            return '<';
        case UP:
            return '^';
        case DOWN:
            return 'v';
    }
    return '*';
}

char change_direction(Monster *m) {
    switch (m->direction) {
        case RIGHT:
            m->direction = LEFT;
            return '<';
        case LEFT:
            m->direction = RIGHT;
            return '>';
        case UP:
            m->direction = DOWN;
            return 'v';
        case DOWN:
            m->direction = UP;
            return '^';
    }
    return '*';
}

void player_move_rules(Level *l, short x_offset, short y_offset) {
    if (l->map[l->p.x + x_offset][l->p.y + y_offset] == '#') {
        l->p.y += 0;
        l->p.x += 0;
    } else if (l->map[l->p.x + x_offset][l->p.y + y_offset] == 'A') {
        l->map[l->p.x][l->p.y] = ' ';
        l->p.win = 1;
    } else {
        l->map[l->p.x][l->p.y] = ' ';
        l->p.y += y_offset;
        l->p.x += x_offset;
    }
}

void player_move(Level *l, char key) {
    switch (key) {
        case 's':
            player_move_rules(l, 1, 0);
            break;
        case 'w':
            player_move_rules(l, -1, 0);
            break;
        case 'a':
            player_move_rules(l, 0, -1);
            break;
        case 'd':
            player_move_rules(l, 0, 1);
            break;
    }
}

void print_level(Level *l, FILE *print) {
    for (int x = 0; x < l->rows; x++) {
        for (int y = 0; y < l->columns; y++) {
            if (l->p.x == x && l->p.y == y && l->p.win == 0 && l->p.dead == 0) {
                fprintf(print, "S");
            } else if (l->map[x][y] == '>' || l->map[x][y] == '<' || l->map[x][y] == 'v' ||
                       l->map[x][y] == '^' || l->map[x][y] == ' ' || l->map[x][y] == '#' ||
                       l->map[x][y] == 'A' || l->map[x][y] == '\n') {
                fprintf(print, "%c", l->map[x][y]);
            }
        }
    }
}

void set_monster(Monster *m, int x, int y, Direction dir) {
    m->x = x;
    m->y = y;
    m->direction = dir;
}

void set_player(Player *p, int x, int y) {
    p->x = x;
    p->y = y;
}

void map_memory_allocation(Level *l) {
    l->map = calloc(l->rows, sizeof(char *));
    for (int i = 0; i < l->rows; i++) {
        l->map[i] = calloc(l->columns, sizeof(char));
    }
    l->m = calloc(l->monsters, sizeof(Monster));
}

void build_level(FILE *in, Level *l) {
    l->p.dead = 0;
    l->p.win = 0;
    char chr;

    int mCnt = 0;
    for (int x = 0; x < l->rows; x++) {
        for (int y = 0; y < l->columns; y++) {
            if ((chr = fgetc(in)) != EOF) {
                if (chr == 'S') {
                    set_player(&l->p, x, y);
                    l->map[x][y] = chr;
                } else if (chr == '>') {
                    l->map[x][y] = chr;
                    set_monster(&l->m[mCnt], x, y, RIGHT);
                    mCnt++;
                } else if (chr == '<') {
                    l->map[x][y] = chr;
                    set_monster(&l->m[mCnt], x, y, LEFT);
                    mCnt++;
                } else if (chr == 'v') {
                    l->map[x][y] = chr;
                    set_monster(&l->m[mCnt], x, y, DOWN);
                    mCnt++;
                } else if (chr == '^') {
                    l->map[x][y] = chr;
                    set_monster(&l->m[mCnt], x, y, UP);
                    mCnt++;
                } else if (chr == '\n') {
                    break;
                } else {
                    l->map[x][y] = chr;
                }
            }
            l->map[x][l->columns - 1] = '\n';
        }
    }
}

void count_all(FILE *in, Level *l) {
    char chr;
    int column = 0;
    l->columns = 0;
    while ((chr = fgetc(in)) != EOF) {
        if (chr == '\n') {
            column++;
            l->rows++;
            l->columns = l->columns > column ? l->columns : column;
            column = 0;
        } else if (chr == '<') {
            l->monsters++;
            column++;
        } else if (chr == '>') {
            l->monsters++;
            column++;
        } else if (chr == '^') {
            l->monsters++;
            column++;
        } else if (chr == 'v') {
            l->monsters++;
            column++;
        } else {
            column++;
        }
    }
    rewind(in);
}

void free_close_memory(FILE *in, FILE *out, FILE *command, Level *l) {
    if (l != NULL) {
        for (int i = 0; i < l->rows; i++) {
            free(l->map[i]);
        }
        free(l->map);
        free(l->m);
        free(l);
    }

    if (in) {
        fclose(in);
    }
    if (out) {
        fclose(out);
    }
    if (command) {
        fclose(command);
    }
}

void play_game(Level *l, FILE *out, FILE *command) {
    char key;
    int round = 1;

    print_level(l, out);
    monster_cleanup(l);

    while (l->p.win == 0 && l->p.dead == 0) {
        key = fgetc(command);
        if ((key == 'w') || (key == 'a') || (key == 's') || (key == 'd')) {
            player_move(l, key);
            monster_rules(l);
            fprintf(out, "%i %c\n", round++, key);
            print_level(l, out);
            monster_cleanup(l);
            if (l->p.win == 1) {
                fprintf(out, "Gewonnen!\n");
            }
            if (l->p.dead == 1) {
                fprintf(out, "Du wurdest von einem Monster gefressen.\n");
            }
        }
    }
}

void game_manager(Level *l, FILE *out, FILE *command) {
    if (out != NULL && command != NULL) {
        play_game(l, out, command);
    } else if (out != NULL && command == NULL) {
        play_game(l, out, command);
    } else if (out == NULL && command != NULL) {
        play_game(l, out, command);
    } else {
        play_game(l, out, command);
    }
}

short file_read_error(FILE **file) {
    char test;
    if ((test = fgetc(*file)) == EOF) {
        return 1;
    }
    rewind(*file);
    return 0;
}

short options_handling(int *argc, char **argv[], FILE **out, FILE **command) {
    int opt;
    short toManyI = 0;
    short toManyO = 0;

    while ((opt = getopt(*argc, *argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                if (toManyI == 0) { 
                    *command = fopen(optarg, "r");
                    if (!*command) { 
                        fprintf(stderr, "Fehler: Datei nicht geoeffnet!(Befehle)\n");
                        return 1;
                    }
                    if (file_read_error(command) == 1) {
                        fprintf(stderr, "Fehler: Datei nicht gelesen!(Befehle)\n");
                        return 2;
                    }
                    toManyI++;
                } else {
                    fprintf(stderr, "Fehler: Doppelte Option!\n");
                    return 3;
                }
                break;
            case 'o':
                if (toManyO == 0) { 
                    *out = fopen(optarg, "w");
                    if (!*out) {
                        fprintf(stderr, "Fehler: Datei nicht geoeffnet!(Ausgabe)\n");
                        return 1;
                    }
                    toManyO++;
                } else {
                    fprintf(stderr, "Fehler: Doppelte Option!\n");
                    return 3;
                }
                break;
            default:
                fprintf(stderr, "Fehler: Falsche Option!\n");
                return 3;
        }
    }
    *argc -= optind;
    *argv += optind;
    return 0;
}

short open_leveldata(int *argc, char **argv[], FILE **in) {
    if (*argc == 0) {
        *in = fopen("level/1.txt", "r");
    } else {
        if (!(*in = fopen(*argv[0], "r"))) {
            fprintf(stderr, "Fehler: Datei nicht geoeffnet!(Level)\n");
            return 1;
        }
        if (file_read_error(in) == 1) {
            fprintf(stderr, "Fehler: Datei nicht gelesen!(Level)\n");
            return 2;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *in = NULL;
    FILE *out = stdout;
    FILE *command = stdin;
    short error = 0;

    error = options_handling(&argc, &argv, &out, &command);
    if (error == 0) {
        error = open_leveldata(&argc, &argv, &in);
    }
    if (error != 0) {
        free_close_memory(in, out, command, NULL);
        return error;
    }
    if (argc > 1) {
        free_close_memory(in, out, command, NULL);
        fprintf(stderr, "Fehler: Zu viele Parameter\n");
        return 3;
    }

    Level *l = calloc(1, sizeof(Level));

    count_all(in, l);
    map_memory_allocation(l);
    build_level(in, l);
    game_manager(l, out, command);
    free_close_memory(in, out, command, l);

    return 0;
}
