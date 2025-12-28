#! /bin/sh

flex flex.l
bison bison.y -d
gcc -o parser parser.c lex.yy.c bison.tab.c jsonlib.c
