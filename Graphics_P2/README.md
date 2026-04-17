# COS344 Practical 2 — Compiling Instructions

## Requirements
- g++ with C++11 support
- OpenGL 3.3+
- GLFW3
- GLEW
- On WSL/Linux: run `export LIBGL_ALWAYS_SOFTWARE=1` if no GPU driver is available

## Compile
```
make
```

## Run
```
./main
```

## Compile and Run (clean build)
```
make all
```

## Controls
| Key        | Action                        |
|------------|-------------------------------|
| 1          | Select golf ball              |
| 2          | Select obstacle (triangle)    |
| 3          | Select obstacle (octagon)     |
| 4          | Select golf hole              |
| 0          | Deselect                      |
| W          | Move selected up              |
| S          | Move selected down            |
| A          | Move selected left            |
| D          | Move selected right           |
| Q          | Rotate counter-clockwise      |
| E          | Rotate clockwise              |
| +          | Scale up                      |
| -          | Scale down                    |
| Enter      | Toggle wireframe              |
| Escape     | Exit                          |