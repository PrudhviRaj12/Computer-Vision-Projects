
all: CImg.h food.cpp Classifier.h HaarClassifier.h NearestNeighbor.h SVM.h SVM2.h
	mkdir -p  n
	\
	g++ food.cpp  -o food -lX11 -lpthread `pkg-config opencv --cflags` `pkg-config opencv --libs` -I. -O3 -Isiftpp siftpp/sift.cpp
overfeat:
	tar xvf overfeat.tgz
	./overfeat/download_weights.py

clean:
	rm food
