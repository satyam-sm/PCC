all: calculator

calculator: calculator.l calculator.y
	bison -d calculator.y  # Generates y.tab.c and y.tab.h
	flex calculator.l      # Generates lex.yy.c
	gcc -o calculator y.tab.c lex.yy.c -lm  # Compiles everything into calculator

clean:
	rm -f calculator lex.yy.c y.tab.c y.tab.h

