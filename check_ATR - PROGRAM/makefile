res: ATR.o 
	gcc ATR.o -L. -I/usr/include/PCSC/ -lpcsclite -lstdc++ -o res 

ATR.o: ATR.cpp
	gcc -c ATR.cpp -lpcsclite -I/usr/include/PCSC/ -lstdc++
	
clean:
	rm -rf *.o *.a res