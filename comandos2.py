

import os, time 

bed = 'BED'	#str(input("Informe o BED (sem barra no final): "))
bsd = 'BSD'	#str(input("Informe o BSD (sem barra no final): "))
src = 'src'	#str(input("Informe o scr (sem a barra no final): "))
res = 'res'
dot = 'DOT'

progr = 't2'


comandos = {}
for c in 'p? @o? cx rv del catac'.split():	
	com = open('1com-%s.sh' %(c.replace('?','_')),'w',encoding='utf8')	
	comandos[c.lower()] = com
	print('#!/bin/bash\necho',c,file=com) 	
	print('\n',file=com) 	
	
def coms (comb, ln, arquivos = comandos):	
	while len(comb) > 1:	
		if not comb in arquivos:
			a = '%dcoms' %len(comb)
			for c in comb:
				a += '-' + c.replace('?','_')
			arquivos[comb] = open(a + '.sh','w',encoding='utf8')
		print(ln,file=arquivos[comb])	
		for a in range(len(comb) - 1):
			coms(comb[:a] + comb[a + 1:], ln, arquivos)
		comb = comb[:-1] 	


geos = list()
c = 0
#

ti = time.time()
try:
	for(dirpath, dirnames, filenames) in os.walk(bed):
		geos.extend(filenames)
		break
	
	for geo in geos:		
		nomeGEO = geo.split('.')	
		if not 'geo' in nomeGEO[1]:
			continue
		for(dirpath, dirnames, filenames) in os.walk(bed + '/' + nomeGEO[0]):
			qrys = list()
			qrys.extend(filenames)
			break
			
		for qry in qrys:		
			linha = f'{src}/{progr} -e {bed} -q {nomeGEO[0]}/{qry} -o {bsd} -f {geo} -via {nomeGEO[0]}-v.via'
			print(c,'\t',geo,'\t',qry)
			
			q = set()
			for com in open(bed + '/' + nomeGEO[0] + '/' + qry, 'r').read().split():
				if com in comandos:
					if not com in q:
						print(com)
						print(linha,file=comandos[com])	
					q.add(com)
			if len(q) > 1:
				q = list(q)		
				q.sort()
				coms(tuple(q),linha,comandos)
			
			
			
		#	s = os.system(linha)
			print(linha,'\n')															
			c += 1	
			
except KeyboardInterrupt:
	print('interrompido')
print(time.time() - ti, 'para', c, 'testes')		
def xcontido (x,c):
	if x == c or type(c) == str:
		return False
	if type(x) == str:
		return x in c	
	for e in x:	
		if not e in c:
			return False
	return True		
		
for c in comandos:
	print('\necho',comandos[c].name,'concluído:',*c,file=comandos[c])
	for a in comandos:
		if xcontido(a,c):
			print(a,'@',c)
			print('echo', comandos[a].name, 'está nesse', file=comandos[c])
		elif xcontido(c,a):	
			print(c,'@',a)
			print('echo está em', comandos[a].name, file=comandos[c])			
	comandos[c].close() 	
	print(os.system('chmod +x ' + comandos[c].name), '\tgedit', comandos[c].name, '\t', c, '\n')
