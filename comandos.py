import os, time 

bed = 'BED'	#str(input("Informe o BED (sem barra no final): "))
bsd = 'BSD'	#str(input("Informe o BSD (sem barra no final): "))
src = 'src'	#str(input("Informe o scr (sem a barra no final): "))
res = 'res'
dot = 'DOT'

progr = 't1'


comandos = {}
for c in 'del m? dm? mud oloc oloc? loc loc? dloc hom mul dmpt catac'.split():	
	com = open('com-%s.sh' %(c.replace('?','_')),'w',encoding='utf8')	
	comandos[c.lower()] = com
	print('#!/bin/bash\necho',c,file=com) 	
	
	

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
			linha = f'{src}/{progr} -e {bed} -q {nomeGEO[0]}/{qry} -o {bsd} -f {geo} -pm {nomeGEO[0]}.pm'
			print(c,'\t',geo,'\t',qry)
			
			q = set()
			for com in open(bed + '/' + nomeGEO[0] + '/' + qry, 'r').read().split():
				if com in comandos:
					if not com in q:
						print(com)
						print(linha,file=comandos[com])	
					q.add(com)
			
			
			
		#	s = os.system(linha)
			print(linha,'\n')															
			c += 1	
			
except KeyboardInterrupt:
	print('interrompido')
print(time.time() - ti, 'para', c, 'testes')		
for c in comandos:
	print('\necho',comandos[c].name,c,'concluído',file=comandos[c])
	comandos[c].close() 	
	print(os.system('chmod +x ' + comandos[c].name), comandos[c].name, c)