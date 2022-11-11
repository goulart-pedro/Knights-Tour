default: clean
	gcc -o knight main.c
	gcc -o verificar verificar.c 

localtest: clean
	./knight

test: clean
	./verificar

clean:
	rm -f saida.txt
	clear