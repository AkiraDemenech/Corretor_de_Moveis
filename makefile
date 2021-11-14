clean:
	make clear
clear:	
	rm *.o
	rm *.out
	rm t2
	rm t1

opts = -fstack-protector-all -std=c99 -g
comp = gcc


qry:	
	$(comp) $(opts) -c qry.c -o qry.o

svg:
	$(comp) $(opts) -c svg.c -o svg.o

arq:
	$(comp) $(opts) -c arq.c -o arq.o

arv:	
	$(comp) $(opts) -c arv.c -o arv.o

esp:	
	$(comp) $(opts) -c strhash.c -o hash.o

din:	
	$(comp) $(opts) -c din.c -o list.o	

cid:	
	$(comp) $(opts) -c cidade.c -o cidade.o	

qua:	
	$(comp) $(opts) -c quadra.c -o quadra.o	

mapa:
	$(comp) $(opts) -c mapa.c -o cidade.o	
	$(comp) $(opts) -c vias.c -o vias.o

pm:	
	$(comp) $(opts) -c pm.c -o pm.o		
	$(comp) $(opts) -c loc.c -o aluguel.o

c:	
	$(comp) $(opts) t2.c quadra.o cidade.o vias.o arv.o arq.o qry.o hash.o list.o svg.o -o t2.out


oh:
	make arq
	make arv
	make din
	make esp
	make mapa
	make svg
	make qry
	make qua
	

t1:
	make oh
	make pm
	make cid
	$(comp) $(opts) t1.c quadra.o aluguel.o pm.o cidade.o arv.o arq.o qry.o hash.o list.o svg.o -o t1.out
	cp t1.out t1.o
	cp t1.o t1
	echo acabado.

t2:
	make oh
	make c
	cp t2.out t2.o
	cp t2.o t2
	echo terminado!


	
	

