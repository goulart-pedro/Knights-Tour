compile: clean
	gcc -o knight main.c -lm
	gcc -o verificar verificar.c -lm

localtest: clean
	./knight

test: clean
	./verificar

clean:
	clear
	rm -f saida.txt