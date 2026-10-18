CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic
OBJECTS = src/main.o src/LanguageModel.o src/TextGenerator.o

.PHONY: all clean test
all: slm

slm: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

src/main.o: src/main.cpp src/LanguageModel.h src/TextGenerator.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/LanguageModel.o: src/LanguageModel.cpp src/LanguageModel.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/TextGenerator.o: src/TextGenerator.cpp src/TextGenerator.h src/LanguageModel.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTS) slm
