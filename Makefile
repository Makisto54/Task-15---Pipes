CC := gcc
CFLAGS := -c
LDFLAGS := 

TASK_1 := pipe_chat
TASK_2 := bash_interpretator

.PHONY: all $(TASK_1) $(TASK_2) clean

all: $(TASK_1) $(TASK_2)

debug: CFLAGS += -g 
debug: all

$(TASK_1): $(TASK_1).c
	$(CC) $(CFLAGS) $(TASK_1).c -o  $(TASK_1) $(LDFLAGS) 

$(TASK_2): $(TASK_2).c
	$(CC) $(CFLAGS) $(TASK_2).c -o $(TASK_2) $(LDFLAGS)

clean:
	rm -rf $(TASK_1) $(TASK_2)
