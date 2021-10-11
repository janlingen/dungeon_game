#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum Direction { RIGHT, LEFT, UP, DOWN } Direction;

typedef struct Player {
    int x;
    int y;
    short dead : 2;
    short win : 2;
} Player;

typedef struct Monster {
    int x;
    int y;
    Direction direction;
} Monster;

typedef struct Level {
    int rows;
    int columns;
    int monsters;
    char **map;
    Player p;
    Monster *m;
} Level;


short options_handling(int *argc, char **argv[], FILE **out, FILE **command);
short open_leveldata(int *argc, char **argv[], FILE **in);
short file_read_error(FILE **file);
void count_all(FILE *in, Level *l);
void map_memory_allocation(Level *l);
void free_close_memory(FILE *in, FILE *out, FILE *command, Level *l);
void build_level(FILE *in, Level *l);
void set_player(Player *p, int x, int y);
void set_monster(Monster *m, int x, int y, Direction dir);
void player_move(Level *l, char key);
void player_move_rules(Level *l, short x_offset, short y_offset);
void monster_rules(Level *l);
char get_direction(Monster *m);
char change_direction(Monster *m);
void game_manager(Level *l, FILE *out, FILE *command);
void play_game(Level *l, FILE *out, FILE *command);
void monster_cleanup(Level *l);
void print_level(Level *l, FILE *print);
