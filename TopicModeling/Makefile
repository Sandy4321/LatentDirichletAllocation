all: fastLDA
fastLDA: LDA.o SCVB0.o Document.o MiniBatch.o
	g++ -fopenmp -o fastLDA -g LDA.o SCVB0.o Document.o MiniBatch.o

LDA.o: LDA/LDA.cpp LDA/SCVB0.h LDA/Document.h LDA/MiniBatch.h
	g++ -fopenmp -g -c -Wall LDA/LDA.cpp

SCVB0.o: LDA/SCVB0.cpp LDA/SCVB0.h
	g++ -fopenmp -g -c -Wall LDA/SCVB0.cpp
	
MiniBatch.o: LDA/MiniBatch.cpp LDA/MiniBatch.h
	g++ -fopenmp -g -c -Wall LDA/MiniBatch.cpp
	
Document.o: LDA/Document.cpp LDA/Document.h
	g++ -fopenmp -g -c -Wall LDA/Document.cpp

clean:
	rm -f *.o fastLDA
