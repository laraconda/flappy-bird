LDFLAGS = -lncurses -lpthread -lm
SRC_DIR = src
OBJ_DIR = build

$(OBJ_DIR)/flappy: $(OBJ_DIR)/obstacles.o $(OBJ_DIR)/bird.o $(OBJ_DIR)/game.o $(OBJ_DIR)/welcome.o
	cc -o $(OBJ_DIR)/flappy $^ $(LDFLAGS)

$(OBJ_DIR)/obstacles.o: $(SRC_DIR)/obstacles.c $(SRC_DIR)/obstacles.h $(SRC_DIR)/nwindows.h
	cc -c $< -o $@

$(OBJ_DIR)/bird.o: $(SRC_DIR)/bird.c $(SRC_DIR)/bird.h
	cc -c $< -o $@

$(OBJ_DIR)/game.o: $(SRC_DIR)/game.c $(SRC_DIR)/externs.h $(SRC_DIR)/bird.h $(SRC_DIR)/obstacles.h $(SRC_DIR)/keys.h $(SRC_DIR)/nwindows.h
	cc -c $< -o $@

$(OBJ_DIR)/welcome.o: $(SRC_DIR)/welcome.c $(SRC_DIR)/externs.h $(SRC_DIR)/game.h $(SRC_DIR)/keys.h
	cc -c $< -o $@

clean:
	rm -r $(OBJ_DIR)

$(shell mkdir -p $(OBJ_DIR))

.PHONY: clean
