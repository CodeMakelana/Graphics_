| Step | What to build | Why first |
|---|---|---|
| 1 | Window setup + GLFW/GLEW init + basic clear colour | Prove the OpenGL setup works before adding features. |
| 2 | Custom matrix/vector math | Everything else depends on reliable transforms. |
| 3 | Basic vertex + fragment shader | Needed before any drawing can happen. |
| 4 | Draw one hardcoded rectangle | Prove the rendering pipeline works end-to-end. |
| 5 | Shape class with transform state | Foundation for all scene objects. |
| 6 | All shape types (Rectangle, Triangle, Circle ×2) | Build the full scene. |
| 7 | Lay out the full golf course scene | Assemble and position everything visually (Make it look right). |
| 8 | Selection system + colour highlight | Required before applying object-specific transforms. |
| 9 | All 8 transformations | Adds full interaction control on selected objects. |
| 10 | Wireframe mode with GL_LINES | Add alternative render mode on top of working scene. |
| 11 | Polish + Makefile | Final cleanup and submission readiness. |