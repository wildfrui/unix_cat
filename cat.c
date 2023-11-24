#include "cat.h"

int main(int argc, char **argv) {
  Opts options = {false};
  parse_opts(argc, argv, &options);
  print_files(argc, argv, options);
  return EXIT_SUCCESS;
}

void parse_opts(int argc, char **argv, Opts *options) {
  int rez = 0;
  static const struct option LONG_FLAGS[] = {{"number-nonblank", 0, NULL, 'b'},
                                             {"number", 0, NULL, 'n'},
                                             {NULL, 0, NULL, 0}};
  while ((rez = getopt_long(argc, argv, "+beEnstTv", LONG_FLAGS, NULL)) != -1) {
    switch (rez) {
      case 'b':
        options->b = true;
        break;
      case 'e':
        options->e = true;
        options->v = true;
        break;
      case 'n':
        options->n = true;
        break;
      case 's':
        options->s = true;
        break;
      case 't':
        options->t = true;
        options->v = true;
        break;
      case 'T':
        options->t = true;
        break;
      case 'E':
        options->e = true;
        break;
      case 'v':
        options->v = true;
        break;
      case '?':
        options->err = true;
        break;
    }
  }
  check_priority(options);
}

void print_files(int argc, char **argv, Opts options) {
  if (options.err != 1) {
    while (optind < argc) {
      FILE *cur_file = fopen(argv[optind], "r");
      if (cur_file != NULL) {
        process_file(cur_file, options);
        optind++;
      } else {
        fprintf(stderr, "cat: %s: %s", argv[optind],
                "No such file or directory\n");
        optind++;
      }
      fclose(cur_file);
    }
  } else {
    fputs("usage: cat [-benst] [file ...]", stderr);
  }
}

void process_file(FILE *file, Opts options) {
  int count = 1;
  char previous_char = '\n';
  char cur_char = fgetc(file);
  while (cur_char != EOF) {
    if (options.n || options.b) {
      number_line(cur_char, previous_char, options, &count);
    }
    if (options.e) {
      e_flag(cur_char);
    }
    if (options.s) {
      s_flag(cur_char, previous_char, file);
    }
    print_char(cur_char, options);
    previous_char = cur_char;
    cur_char = fgetc(file);
  }
  count = 1;
}

void e_flag(char cur_char) {
  if (cur_char == '\n') {
    fputc('$', stdout);
  }
}

void s_flag(char cur_char, char previous_char, FILE *file) {
  if (cur_char == '\n' && previous_char == '\n') {
    while (cur_char == '\n') {
      cur_char = fgetc(file);
    }
    ungetc(cur_char, file);
  }
}

void move_tab(char cur_char, Opts options) {
  if (options.t) {
    fputc('^', stdout);
    fputc(SHIFT + cur_char, stdout);
  } else {
    fputc(cur_char, stdout);
  }
}

void move_cntrl(char cur_char, Opts options) {
  if (options.v) {
    if (iscntrl(cur_char)) {
      fputc('^', stdout);
      if (cur_char == DELETE) {
        fputc(cur_char - SHIFT, stdout);
      } else {
        fputc(cur_char + SHIFT, stdout);
      }
    }
  } else {
    fputc(cur_char, stdout);
  }
}

void print_char(char cur_char, Opts options) {
  if (isprint(cur_char)) {
    fputc(cur_char, stdout);
  } else if (cur_char == '\t') {
    move_tab(cur_char, options);
  } else if (cur_char == '\n') {
    fputc(cur_char, stdout);
  } else if (iscntrl(cur_char)) {
    move_cntrl(cur_char, options);
  } else {
    move_meta(cur_char, options);
  }
}

void move_meta(char cur_char, Opts options) {
  if (options.v) {
    char m_char = (signed char)cur_char + DELETE + 1;
    if (isprint(m_char)) {
      fputc(m_char, stdout);
    } else {
      move_cntrl(m_char, options);
    }
  } else {
    fputc(cur_char, stdout);
  }
}

void number_line(char cur_char, char previous_char, Opts options, int *count) {
  if (options.b && previous_char == '\n' && cur_char != '\n') {
    printf("%6d\t", (*count)++);
  } else if (options.n && previous_char == '\n') {
    printf("%6d\t", (*count)++);
  }
}

void check_priority(Opts *options) {
  if (options->b && options->n) {
    options->n = false;
  }
}