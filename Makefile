C_FLAGS=-O3
C_LIBS=-lcurl
C_FILES=val.o net.o

val: $(C_FILES)
	gcc -o $@ $(C_FILES) $(C_FLAGS) $(C_LIBS)

*.o: *.c
	gcc -c -o $@ $< $(C_FLAGS)

clean:
	$(RM) *.o val