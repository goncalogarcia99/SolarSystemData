CC = gcc
CFLAGS = -Wall
LDFLAGS = -lcurl -ljansson
TARGET = solar
OBJECT_FILES_WEB = web/url_transfer.o web/json_manipulation.o
OBJECT_FILES_DS = data_structures/list.o data_structures/hash_table.o
OBJECT_FILES_UI = user_interface/user_interface.o

# This built-in target with no dependencies
# causes intermediate files not to be removed.
.SECONDARY:

all: $(TARGET)

# Link the target object file to the shared library.
$(TARGET): $(TARGET).o
	$(CC) $< -o $@ -L. -lsolar

# Preprocess, compile and assemble the target c file.
$(TARGET).o: $(TARGET).c libsolar.so
	$(CC) $(CFLAGS) -c $< -o $@

# Create the shared library.
libsolar.so: $(OBJECT_FILES_WEB) $(OBJECT_FILES_DS) $(OBJECT_FILES_UI)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

# Preprocess, compile and assemble the json_manipulation c file.
json_manipulation.o: %.c %.h url_transfer.h
	$(CC) $(CFLAGS) -fpic -c $< -o $@

# Preprocess, compile and assemble the hash_table c file.
hash_table.o: %.c %.h list.h
	$(CC) $(CFLAGS) -fpic -c $< -o $@

# Preprocess, compile and assemble the other files.
%.o: %.c %.h
	$(CC) $(CFLAGS) -fpic -c $< -o $@

clean:
	rm -f *.i *.s *.o *.so $(TARGET)
	rm -f web/*.i web/*.s web/*.o
	rm -f data_structures/*.i data_structures/*.s data_structures/*.o
	rm -f user_interface/*.i user_interface/*.s user_interface/*.o
