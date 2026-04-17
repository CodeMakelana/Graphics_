# ─── Source files ─────────────────────────────────────────────────────────────
# #add all the files that you need to the files makefile variable below as a space seperated list
files = Vec3.cpp Mat4.cpp Shape.cpp Rectangle.cpp Circle.cpp Triangle.cpp Scene.cpp

# ─── Student number ─────────────────────────────────────────────────────────────
STUDENT_NUMBER = U23588579

# ─── Compiler & flags ─────────────────────────────────────────────────────────
CXX      = g++
CXXFLAGS = -std=c++11 -g -Wall -Wextra

# ─── Link libraries ───────────────────────────────────────────────────────────
LIBS = -lglfw3 -pthread -lGLEW -ldl -lGL

# ─── Targets ──────────────────────────────────────────────────────────────────
main: main.cpp glad.c Shader.cpp $(files)
	$(CXX) $(CXXFLAGS) Shader.cpp $(files) main.cpp glad.c $(LIBS) -o main

clean:
	rm -f *.o main

run:
	./main

all:
	$(MAKE) clean
	$(MAKE)
	$(MAKE) run

# ─── Zip for submission ────────────────────────────────────────────────────────
# Creates U23588579.zip
# Usage: make zip
zip:
	zip u23588579$(STUDENT).zip \
		*.cpp *.h *.hpp *.glsl *.c \
		makefile README.md
	@echo "Created P2_$(STUDENT).zip — ready to upload to ClickUp"
 