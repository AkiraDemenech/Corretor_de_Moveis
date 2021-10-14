clean:
	make clear
clear:
	rm t1
	rm *.o
	rm *.out

opts = -fstack-protector-all -std=c99
comp = gcc


qry:	
	$(comp) $(opts) -c qry.c -o qry.o

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

pm:	
	$(comp) $(opts) -c pm.c -o pm.o		
	$(comp) $(opts) -c loc.c -o aluguel.o

c:	
	$(comp) $(opts) t1.c quadra.o aluguel.o pm.o cidade.o arv.o arq.o qry.o hash.o list.o -o t1.out


oh:
	make arq
	make arv
	make din
	make esp
	make cid
	make qry
	make qua
	make pm
	

t1:
	make oh
	make c
	cp t1.out t1.o
	cp t1.o t1
	echo terminado!


	
	

