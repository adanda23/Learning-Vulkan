CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(VULKAN_SDK)/include
LDFLAGS = -L$(VULKAN_SDK)/lib
LDLIBS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = VulkanTest

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: clean debug

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

debug: CXXFLAGS += -g
debug: $(EXECUTABLE)

release: CXXFLAGS += -O2
release: $(EXECUTABLE)