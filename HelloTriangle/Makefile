# Define variables
CXX = g++
CXXFLAGS = -Wall -g 
CFLAGS = -std=c++17 -O2
LDFLAGS =
LDLIBS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SOURCES = main.cpp 
OBJECTS = $(SOURCES:.cpp=.o) # Automatically generate object file names
EXECUTABLE = VulkanTest

# Rule to build the final executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Rule to compile .cpp files into .o files (uses built-in rules if named correctly)
# This uses the implicit rule: $(CXX) $(CXXFLAGS) -c $< -o $@

# Phony target for cleaning up generated files
.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

debug:
	$(MAKE) CXXFLAGS="-Wall -g" LDFLAGS="-g" $(EXECUTABLE)
