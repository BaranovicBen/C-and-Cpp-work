CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17
TARGET = tableTest
SRC = hw06.cpp
OUT = out_6235971.bin
REF = ref_6235971.bin
DIFF = diff.txt

.PHONY: all run diff clean hex

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET) > $(OUT)

diff: run
	diff -u $(REF) $(OUT) > $(DIFF) || true
	@echo "✅ Porovnanie ukončené. Výsledok diff je uložený v '$(DIFF)'"

hex:
	@echo "🔍 Zobrazenie rozdielov v hexadecimálnom formáte:"
	@xxd $(REF) > ref.hex
	@xxd $(OUT) > out.hex
	@diff -u ref.hex out.hex || true

clean:
	rm -f $(TARGET) $(OUT) $(DIFF) ref.hex out.hex
