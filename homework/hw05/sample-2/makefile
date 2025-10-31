CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

TARGET = hw05
SOURCES = hw05.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run-tests: $(TARGET)
	@echo "Running tests..."
	@mkdir -p outputs
	@./$(TARGET)

	@diff -q outputs/0out 0ref && echo "Test 0 PASSED" || echo "Test 0 FAILED"
	@diff -q outputs/1out 1ref && echo "Test 1 PASSED" || echo "Test 1 FAILED"
	@diff -q outputs/2out 2ref && echo "Test 2 PASSED" || echo "Test 2 FAILED"
	@diff -q outputs/3out 3ref && echo "Test 3 PASSED" || echo "Test 3 FAILED"

clean:
	rm -f $(TARGET)
	rm -rf outputs
