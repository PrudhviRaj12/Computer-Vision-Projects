all: CImg.h img.cpp
	g++ img.cpp -o img -lX11 -lpthread -I. -Isiftpp -O3 siftpp/sift.cpp

clean:
	rm img
