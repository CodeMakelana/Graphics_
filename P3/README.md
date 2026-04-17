# COS 344 Practical 3 - Build and Run Guide

## 1) Prerequisites (Linux or WSL)

Install required tools and OpenGL development libraries:

```bash
sudo apt update
sudo apt install -y build-essential libglfw3-dev libglew-dev libgl1-mesa-dev
```

## 2) Build and Run 

From the project folder:

```bash
make all
```

This runs:
- clean
- compile
- execute

## 3) Build and Run (Step by Step)

```bash
make clean
make
make run
```

## 4) If Build Fails

- If `GL/glew.h` or `GLFW/glfw3.h` is missing, install the prerequisites in section 1.
- If `main` does not launch in WSL, ensure an X server/display is available.
