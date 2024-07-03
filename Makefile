CC = g++

tANSexample :
	$(CC) -o tANSexample main.cpp tANS.cpp

debug :
	$(CC) -g -o0 -o tANSexample main.cpp tANS.cpp

clean:
	rm tANSexample
