# Názov programu
PROGRAM = hw0

# Kompilačný príkaz
CC = gcc
CFLAGS = -Wall

# Testovacia zložka
TEST_DIR = sample
INPUT_FILES = $(wildcard $(TEST_DIR)/*_in.txt)
OUTPUT_DIR = output

# Výstupný súbor
PROGRAM_OUT = $(PROGRAM).out

# Kompilácia programu
all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).c
	$(CC) $(CFLAGS) -o $(PROGRAM) $(PROGRAM).c

# Testovanie
test: $(PROGRAM) $(INPUT_FILES)
	mkdir -p $(OUTPUT_DIR)
	@for infile in $(INPUT_FILES); do \
		base=$$(basename $$infile _in.txt); \
		echo "Running test $$base..."; \
		./$(PROGRAM) < $$infile > $(OUTPUT_DIR)/$$base.out; \
		if diff -q $(OUTPUT_DIR)/$$base.out $(TEST_DIR)/$$base\_out.txt; then \
			echo "Test $$base passed."; \
		else \
			echo "Test $$base failed."; \
			echo "Expected output:"; \
			cat $(TEST_DIR)/$$base\_out.txt; \
			echo "Your output:"; \
			cat $(OUTPUT_DIR)/$$base.out; \
		fi; \
	done

# Vyčistenie
clean:
	rm -f $(PROGRAM) $(OUTPUT_DIR)/*.out
