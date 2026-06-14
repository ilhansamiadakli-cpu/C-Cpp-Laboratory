#? makefile: make yazarak derleme işlemi başlatılır. makefile dosyası, hangi dosyaların hangi sırayla derleneceğini ve
#? hangi dosyaların birbirine bağımlı olduğunu belirtir.
#
#! makefile dosyası, genellikle şu formatta yazılır:
#
#* hedef: bağımlılıklar
#*     komutlar
#
#? HEDEF:
#* derlenecek dosya veya programın adıdır.
#? BAĞIMLILIKLAR:
#* hedefin derlenmesi için gereken dosyalar veya diğer hedeflerdir.
#? KOMUTLAR:
#* hedefin derlenmesi için çalıştırılacak komutlardır. Komutlar, hedefin altında ve bir tab karakteri ile başlar.
#* makefile dosyası, genellikle "makefile" veya "Makefile" adıyla kaydedilir ve aynı dizinde bulunur. make komutu çalıştırıldığında
#* makefile dosyası okunur ve belirtilen hedefler ve bağımlılıklar doğrultusunda derleme işlemi gerçekleştirilir.
#
# TODO: Makefile örnek komutları:
#
#! CLEAN:
#* hedefi, derleme sırasında oluşturulan tüm .o dosyalarını ve "program" adlı hedefi silmek için kullanılır.
#! RUN:
#* hedefi, "program" adlı hedefi çalıştırmak için kullanılır. Bu hedef, "program" hedefine bağımlıdır
#* bu nedenle "program" hedefi derlenmeden önce çalıştırılmaya çalışılırsa, önce "program" hedef derlenir ve ardından çalıştırılır.


program: main.o donusum.o analiz.o 
	g++ main.o donusum.o analiz.o  -o program

main.o: main.cpp donusum.h analiz.h
	g++ -c main.cpp

donusum.o: donusum.cpp donusum.h
	g++ -c donusum.cpp

analiz.o: analiz.cpp analiz.h
	g++ -c analiz.cpp

clean:
	rm -f *o program
run: program
	./program
