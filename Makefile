CC = mpicc
CRUN = mpirun
NTHREADS = 1
RUNOPTS = -np $(NTHREADS)
TARGET = corner
CLIB = -lm

.PHONY:
	all

all: compile run

compile: $(TARGET)

$(TARGET): $(TARGET).c $(TARGET).h
	$(CC) $< -o $@ $(CLIB)

run: $(TARGET)
	$(CRUN) $(RUNOPTS) ./$(TARGET)

clean:
	rm *.o $(TARGET)