TARGET = pathtracer
LIBS = -lm
CXX = g++
CXXFLAGS = -Wall -O3 -std=c++17 -march=native -fopenmp

.PHONY: default all clean run

default: $(TARGET)
all: default

BINDIR = bin
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=$(BINDIR)/%.o)
HEADERS = $(wildcard src/*.h)

$(BINDIR)/%.o: src/%.cpp $(HEADERS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) $(CXXFLAGS) -o $@

clean:
	-rm -f $(BINDIR)/*.o
	-rm -f $(TARGET)

run: $(TARGET)
	-./$(TARGET)
