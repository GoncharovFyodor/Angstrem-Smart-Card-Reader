res:libSmartCardLib SmartCard.o 
	gcc SmartCard.o -L. -lSmartCardLib -o res -lpcsclite -I/usr/include/PCSC/ -lstdc++

libSmartCardLib:SmartCardLib.o
	ar rc libSmartCardLib.a SmartCardLib.o
	ranlib libSmartCardLib.a

SmartCardLib.o:SmartCardLib.cpp
	gcc -c SmartCardLib.cpp -lpcsclite -I/usr/include/PCSC/
	
SmartCard.o:SmartCard.cpp
	gcc -c SmartCard.cpp
	
clean:
	rm -rf *.o *.a res