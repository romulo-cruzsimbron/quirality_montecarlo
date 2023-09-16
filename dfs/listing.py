import os
import sys
import glob
import os.path
import re

##remove Mass and list of molecules
list_of_files = glob.glob('list.txt') 
for file_name in list_of_files:
	try:
		f = open(file_name)
		os.remove(file_name)
	except IOError:
	    print("File not accessible")
lista_general = {}	    
with open('list.txt','a') as the_file:
	os.chdir("Imagenes/")
	for file in glob.glob("*.csv"):  
		#the_file.write("Imagenes/"+file[:-4]+"\n")
		#print("Imagenes/"+file[:-4])						
		namestr0=re.split(r'_',file)
		l=int(namestr0[1])
		k=int(namestr0[2])
		m=namestr0[3][:-4]
		namestr1=re.split(r'-',m)
		o = int(namestr1[0])
		#print(o)
		lista_general[o] = "Imagenes/"+file[:-4] 
		#print(lista_general[o])
	for i in sorted (lista_general.keys()) :  
		  the_file.write(lista_general[i]+"\n")
		  print(lista_general[i])
