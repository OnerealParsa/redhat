CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

TARGET = hoodquest
SRCS = main.cpp graph.cpp hash_table.cpp bst.cpp max_heap.cpp game_map.cpp game.cpp user_manager.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)