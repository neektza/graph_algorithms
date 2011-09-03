CC = g++

LAB1_SRC = lab1/src/main.cpp 
LAB1_BIN = lab1/bin/main
LAB1_LNK = l1

LAB2_SRC = lab2/src/main.cpp 
LAB2_BIN = lab2/bin/main
LAB2_LNK = l2

all:
	$(CC) $(LAB1_SRC) -o $(LAB1_BIN)
	$(CC) $(LAB2_SRC) -o $(LAB2_BIN)
	ln -s $(LAB1_BIN) $(LAB1_LNK)
	ln -s $(LAB2_BIN) $(LAB2_LNK)

lab1:
	$(CC) $(LAB1_SRC) -o $(LAB1_BIN)
	ln -s $(LAB1_BIN) $(LAB1_LNK)

lab2:
	$(CC) $(LAB2_SRC) -o $(LAB2_BIN)
	ln -s $(LAB2_BIN) $(LAB2_LNK)

clean:
	rm **/bin/* l1 l2
