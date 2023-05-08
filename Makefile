CC = gcc
CFLAGS = -g
LIBRARIES = -ly -lfl
YACC = yacc
YACCFLAGS = -dv -Wcounterexamples
LEX = lex
LEXFLAGS = -l

free_will: free_will.tab.o lex.yy.o matrix.o tree.o semantic.o hashmap.o interpreter.o utils.o
	$(CC) $(CFLAGS) -o free_will free_will.tab.o lex.yy.o matrix.o tree.o semantic.o hashmap.o interpreter.o utils.o $(LIBRARIES)

matrix.o: structures/matrix.c
	$(CC) $(CFLAGS) -c structures/matrix.c
tree.o: structures/tree.c
	$(CC) $(CFLAGS) -c structures/tree.c
hashmap.o: structures/hashmap.c
	$(CC) $(CFLAGS) -c structures/hashmap.c

semantic.o: semantic.c
	$(CC) $(CFLAGS) -c semantic.c
interpreter.o: interpreter.c
	$(CC) $(CFLAGS) -c interpreter.c
utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

free_will.tab.o: free_will.tab.c
	$(CC) $(CFLAGS) -c free_will.tab.c
free_will.tab.c: free_will.y
	$(YACC) $(YACCFLAGS) -b free_will free_will.y

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c
lex.yy.c: free_will.l
	$(LEX) $(LEXFLAGS) free_will.l

clean:
	rm -Rf free_will *.o *.gch structures/*.o structures/*.gch lex.yy.* free_will.tab.* *.output
