## unix cat command implementation

cat is a standard Unix utility that reads files sequentially, writing them to standard output. The list of supported flags:

- b (GNU: --number-nonblank), number non-blank output lines
- e implies -v but also display end-of-line characters as $ (GNU only: -E the same, but without implying -v)
- n (GNU: --number), number all output lines
- s (GNU: --squeeze-blank), squeeze multiple adjacent blank lines
- t implies -v, but also display tabs as ^I (GNU: -T the same, but without implying -v)
- v, displays nonprinting characters, except for tabs and the end of line character

###To test:

1. Clone
2. cd unix_cat
3. gcc -o cat cat.c
4. Use it like an original cat command:
##### ./cat -v text.txt
##### ./cat -benst text.txt
