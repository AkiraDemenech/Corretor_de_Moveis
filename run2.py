import os, sys, time 
'''sys.stdout = open('console.log', 'w')

arquivos = {}
geo = []

for dp,d,a in os.walk('./BED'):
	arquivos[dp] = []
	for arq in a:
		if '.geo' in a:
			geo.append(a)
		elif not '.qry'	in a:
			continue
		arquivos[dp].append(a)	

#	'''



bed = 'BED'	#str(input("Informe o BED (sem barra no final): "))
bsd = 'BSD'	#str(input("Informe o BSD (sem barra no final): "))
src = 'src'	#str(input("Informe o scr (sem a barra no final): "))
res = 'res'
dot = 'DOT'

progr = 't2'

geos = list()

e = c = 0

falhas = None
falhou_em = []
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
			linha = f'{src}/{progr} -e {bed} -q {nomeGEO[0]}/{qry} -o {bsd} -f {geo} -v {nomeGEO[0]}-v.via'
			print(c,'\t',geo,'\t',qry,file=sys.stderr)
			
			s = os.system(linha)
			print('\n',linha,'\nreturn',s)
			e += s != 0
			if s != 0:
				if falhas == None:
					falhas = open('falhas.txt','w')
				print(linha,'\n\treturn',s,'\n',file=sys.stderr)	
				print(linha.__repr__(),'\t',c,'\t','return',s,file=falhas)
				falhou_em.append((linha,s))			
			c += 1	
			
except KeyboardInterrupt:
	pass
print(time.time() - ti,'para %d testes [' %c, str(e) * (e != len(falhou_em)),len(falhou_em),'falhas ]',file=sys.stderr)		
for f_e in falhou_em:	print(*f_e, file=sys.stderr)

c = 0
for dirpath, dirnames, filenames in os.walk(bsd):
		for filename in filenames:
			if '.dot' in filename:
				s = os.system('dot -T svg %s/%s -o %s/%s/%s.svg ' %(bsd,filename,bsd,dot,filename))
				print(filename,'\t',s,file=sys.stderr)				
				c += not s
		break
print(c,'árvores convertidas para SVG',file=sys.stderr)		