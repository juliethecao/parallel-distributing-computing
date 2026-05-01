NAME = hw4
#CCOPTS = -Wall -pthread
CCOPTS = -Wall -pthread
CCLIBS = -lm
SRC = main.c util.c multiply.c
BIN = $(NAME).bin


.PHONY: build clean test
build: $(BIN)
$(BIN): $(SRC)
	gcc $(CCOPTS) -o $@ $^ $(CCLIBS) 


clean:
	rm -rf $(BIN) sequential.txt parallel.txt


MAT_SIZE = 16
THR = 4
VERB = 1
test: 
	./$(BIN) $(MAT_SIZE) 1 $(VERB) | sed '$$d' > sequential.txt
	./$(BIN) $(MAT_SIZE) $(THR) $(VERB) | sed '$$d' > parallel.txt
	diff sequential.txt parallel.txt
