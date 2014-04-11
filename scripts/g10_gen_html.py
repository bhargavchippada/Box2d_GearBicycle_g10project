#! /usr/bin/env python3.3
import sys
texf=open("./doc/cs296_report_10.tex")
outputf=open("./doc/g10_project_report.html",'w')
sys.stdout=outputf
texflines=[(i.split('\n'))[0] for i in texf.readlines()]
sectioncount=0

#\\
def slashfunc(strg):
	ind=strg.find("\\")
	if(ind != -1):
		return "<br>"
	else:
		return ""
		
#\title
def titlefunc(strg):
	ind=strg.find('\\title')
	if(ind != -1):
		return '<h1 align="center"><b>'+strg[1+strg.find('{'):strg.find('}')]+"</b></h1>"
	else:
		return ""
#\section
def sectionfunc(strg):
	global sectioncount
	ind=strg.find('\section')
	if(ind != -1):
		sectioncount+=1
		return "<h1><b>"+strg[1+strg.find('{'):strg.find('}')]+"</b></h1>"
	else:
		return ""

#\subsection
def subsectionfunc(strg):
	ind=strg.find('\subsection')
	if(ind != -1):
		return "<h1><b>"+strg[1+strg.find('{'):strg.find('}')]+"</b></h1>"
	else:
		return ""
#\textbf
def textbffunc(strg):
	ind=strg.find('\\textbf')
	if(ind != -1):
		return "<h3><b>"+strg[1+strg.find('{'):strg.find('}')]+"</b></h3>"
	else:
		return ""

#\includegraphics
def graphfunc(strg):
	ind=strg.find('\includegraphics')
	if(ind != -1):
		return '<p  align="center"><img src="../plots/g10_project_plot'+(strg[1+strg.find('{'):strg.find('}')])[16:18]+'.png"></p>'
	else:
		return ""
#\texttt
def textttfunc(strg):
	ind=strg.find('\\texttt')
	if(ind != -1):
		return "<p>"+strg[1+strg.find('{'):strg.find('}')]+"</p>"
	else:
		return ""
#\author
aufor=-1
def authorfunc(strg):
	global aufor
	ind=strg.find('\\author')
	if(ind != -1):
		aufor=1
		return '<p>'+strg[1+strg.find('{'):len(strg)-2]+"</p>"
	else:
		return ""

def nobracfunc(strg):
	ind1=strg.find('{')
	ind2=strg.find('}')
	ind3=strg.find('\\')
	if(ind1 == -1 and ind2 == -1):
		if(ind3==-1):
			ind3=len(strg)
			str1=strg.lstrip(' ').rstrip(' ')
			if(len(str1) != 0):
				return "<p>"+str1+"</p>"
			else:
				return ""
		else:
			str2=strg[0:ind3].lstrip(' ').rstrip(' ')
			if(len(str2) != 0):
				return "<p>"+str2+"</p>"
			else:
				return ""
	else:
		return ""
		
print("<html>")
print("<head>")
print("<title>g10_gen_html.py</title>")
print("</head>")
print("<body>")
for hline in texflines:
	lineformat=""
	if(sectionfunc(hline)!=""):
		lineformat=sectionfunc(hline)
	elif(textbffunc(hline)!=""):
		lineformat=textbffunc(hline)
	elif(subsectionfunc(hline)!=""):
		lineformat=subsectionfunc(hline)
	elif(graphfunc(hline)!=""):
		lineformat=graphfunc(hline)
	elif(textttfunc(hline)!=""):
		lineformat=textttfunc(hline)
	elif(authorfunc(hline)!=""):
		lineformat=authorfunc(hline)
	elif(titlefunc(hline)!=""):
		lineformat=titlefunc(hline)
	elif(nobracfunc(hline)!=""):
		lineformat=nobracfunc(hline)
	else:
		lineformat=""		
	if(len(lineformat)==0):
		continue
	else:
		if(aufor<=9 and aufor>0):
			lineformat='<p align="center"><b>'+lineformat.lstrip('<p>').rstrip('</p>')+"</b></p>"
			aufor+=1;
		if(sectioncount==6 or sectioncount==0):
			print(lineformat)
print("</body>")
print("</html>")
outputf.close()
