#ifndef WILDFRUIT_CAT
#define WILDFRUIT_CAT

#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT 64
#define DELETE 127

typedef struct Opts {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
  bool err;
} Opts;

void parse_opts(int argc, char **argv, Opts *options);
void print_files(int argc, char **argv, Opts options);
void process_file(FILE *FILE, Opts options);
void s_flag(char cur_char, char previous_char, FILE *file);
void e_flag(char cur_char);
void move_cntrl(char cur_char, Opts options);
void move_tab(char cur_char, Opts options);
void number_line(char cur_char, char previous_char, Opts options, int *count);
void print_char(char cur_char, Opts options);
void move_meta(char cur_char, Opts options);
void check_priority(Opts *options);

#endif