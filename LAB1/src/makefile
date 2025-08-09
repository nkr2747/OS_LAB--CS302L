# Makefile for Windows (MinGW / g++)
# Usage:
#   make build-sharpen
#   make run-sharpen INPUT=input_file OUTPUT=output_file
#   make clean

TARGET = a.exe
CXX = g++
CXXFLAGS = -g
SRC = image_sharpener.cpp libppm.cpp

build-sharpen: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run-sharpen: $(TARGET)
	$(TARGET) ..\images\$(INPUT).ppm ..\images\$(OUTPUT).ppm

clean:
	del /Q $(TARGET)