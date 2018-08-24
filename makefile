all: server client
.PHONY: all
objectser = myserver.o servjudge.o my_judge.o servftp.o error.o my_unp.o
objectcli = myclient.o clijudge.o my_judge.o cliftp.o error.o my_unp.o
server:$(objectser)
	gcc -o server $(objectser)
client:$(objectcli)
	gcc -o client $(objectcli)
	rm -rf *.o

# error.o: error.h
# network.o: network.h
# myclient.o myftpserver.o: network.h ftp.h error.h
# servftp.o cliftp.o: error.h network.h ftp.h

myserver.o servjudge.o my_judge.o myclient.o clijudge.o cliftp.o: my_judge.h ftp.h my_unp.h
servftp.o: ftp.h my_unp.h error.h
error.o cliftp.o: error.h
my_unp.o: error.h my_unp.h

.PHONY : clean
clean:
	rm -rf *.o server client
