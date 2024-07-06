CC = g++

tANSexample : main.cpp tANS.cpp
	$(CC) -o tANSexample main.cpp tANS.cpp

debug : main.cpp tANS.cpp
	$(CC) -g -o0 -o tANSexample main.cpp tANS.cpp

clean:
	rm tANSexample
