all:
	pdflatex dsg-paper
	bibtex dsg-paper
	pdflatex dsg-paper
	pdflatex dsg-paper
	convert scenegraph.svg scenegraph.png
clean:
	rm -f *.dvi *.log *.toc *.aux *.blg *.bbl *.idx *.ilg *.ind *.lol *.png *~ 

cleanall: clean
	rm -f dsg-paper.pdf dsg-paper.ps