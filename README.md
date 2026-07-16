# PacBoy 🟡👾  

**PacBoy** is a 3D game inspired by the classic **Pac-Man**, developed in **C++17** using **Modern OpenGL (3.3 Core)**.  
The project is educational in nature – the goal is to build a simple 3D game engine with rendering, basic AI, and clean architecture.
⚡ **Currently in progress** – new features are being added step by step.  

---

## 🎮 Features  
- Level loading from text files  
- Rendering of walls, pellets, and energizers using instancing  
- Pac-Man movement with grid-based collision detection  
- Ghost AI with pathfinding algorithms
- Shader-based lighting (GLSL)  
- Scoring system and pellet collection  

---

## 🛠️ Technologies  
- **C++17**  
- **OpenGL 3.3 Core**, GLSL  
- **GLFW** (window & input)  
- **GLAD** (OpenGL loader)  
- **GLM** (math library)  
- **stb_image** (texture loading)  
- **CMake** (build system)  

---

## ▶️ Build & Run  
```bash
# configure and build
cmake -S . -B build
cmake --build build -j

# run
./PacBoy