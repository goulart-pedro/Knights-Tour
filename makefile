default: clean
	gcc -o knight main.c 
	gcc -o verificar verificar.c 

localtest: clean
	./knight

test: clean
	./verificar

clean:
	clear
	rm -f saida.txt