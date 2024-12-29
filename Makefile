# Kompilator g++
CXX = g++

# Flagi kompilatora
CXXFLAGS = -Wall -std=c++11

# Pliki obiektowe
OBJECTS = main.o

# Pliki nagłówkowe
HFILES = RedBlackTree.h

# Nazwa pliku wynikowego
TARGET = main.out

# Domyślna reguła kompilacji plików .cpp na pliki .o
%.o : %.cpp $(HFILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Reguła do budowania programu
$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Reguła do uruchamiania programu
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Cel do czyszczenia plików wynikowych
.PHONY: clean
clean:
	del /Q $(TARGET) *.o