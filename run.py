import os, sys, time 
'''out = sys.stdout
sys.stdout = open('console.log', 'w')

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

	'''



bed = 'BED'	#str(input("Informe o BED (sem barra no final): "))
bsd = 'BSD'	#str(input("Informe o BSD (sem barra no final): "))
src = 'src'	#str(input("Informe o scr (sem a barra no final): "))

progr = 'progr'

geos = list()

e = c = 0

ti = time.time()
try:
	for(dirpath, dirnames, filenames) in os.walk(bed):
		geos.extend(filenames)
		break
	
	for geo in geos:
		nomeGEO = geo.split('.')	
		for(dirpath, dirnames, filenames) in os.walk(bed + '/' + nomeGEO[0]):
			qrys = list()
			qrys.extend(filenames)
			break
			
		for qry in qrys:
			linha = f'{src}/{progr} -e {bed} -q {nomeGEO[0]}/{qry} -o {bsd} -f {geo}'
			print('\t',geo,'\t',qry,file=sys.stderr)
			
			s = os.system(linha)
			print('\n',linha,'\nreturn',s)
			e += s != 0
			if s != 0:
				print('\n\treturn',s,'\n',file=sys.stderr)	
			#	e += 1
			c += 1	
except KeyboardInterrupt:
	pass
print(time.time() - ti,'para %d testes [' %c,e,'falhas ]',file=sys.stderr)		