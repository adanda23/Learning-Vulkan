## Process

The Vulkan setup and rendering process involves several interconnected components. Here's a detailed breakdown:

1.  **Initialize the window (GLFW):** Set up the application window where Vulkan will render.
2.  **Create a Vulkan Instance:** Initialize the Vulkan library.
    *   Specify necessary extensions (like those for GLFW window integration).
    *   Enable validation layers for debugging.
3.  **Find and Select Physical Device:** Identify a suitable GPU.
    *   Query available physical devices.
    *   Filter based on desired features and properties.
    *   Check for required **Queue Families** (e.g., graphics, present).
4.  **Create a Logical Device:** Interface with the selected physical device.
    *   Specify which queue families to use.
    *   Set queue priorities.
    *   Enable device-specific extensions (e.g., swap chain extension).
    *   Enable validation layers.
5.  **Create a Surface:** An OS-specific abstraction representing the renderable area of the window. GLFW handles much of this.

### Key Concepts and Their Connections

**Swap Chain:**
The swap chain (`VkSwapchainKHR`) is essentially a queue of images (typically 2 or 3) that are waiting to be presented to the screen. While one image is displayed, the GPU renders to another. This prevents "tearing" and provides smooth animation.
*   **Connection:** The swap chain is created based on the chosen physical device, surface, and window properties. It provides the `VkImage` objects that the GPU will draw into.

**Image Views:**
A `VkImageView` is a way to look at a `VkImage` (from the swap chain). It describes how to access the image and what parts of it are accessible (e.g., as a 2D texture, an array texture, etc.).
*   **Connection:** For every image in the swap chain, an image view is created. These image views are then used to construct framebuffers.

**Framebuffers:**
A `VkFramebuffer` represents a collection of concrete image resources that a `VkRenderPass` will use as attachments. In our case, each framebuffer corresponds to one of the swap chain image views.
*   **Connection:** Each framebuffer is associated with a specific `VkRenderPass` and contains a `VkImageView` from the swap chain, telling the render pass which actual image to render into.

**Render Pass:**
A `VkRenderPass` defines the sequence of rendering operations (like clearing the screen, drawing geometry) and how attachments (like color buffers or depth buffers) are used. It ensures that the order of operations is well-defined.
*   **Connection:** The render pass needs to know the format of the swap chain images. It also dictates how the framebuffers will be used during rendering.

**Graphics Pipeline & Fixed Functions:**
The graphics pipeline is the sequence of operations that take vertex data and convert it into pixels on the screen.
*   **Shader Modules:** Programmable stages (vertex and fragment shaders) are compiled into SPIR-V format and loaded as `VkShaderModule`s.
*   **Fixed Functions:** These are non-programmable, configurable stages of the pipeline. In your code, you set up:
    *   **Input Assembly:** How vertices are interpreted (e.g., triangle list).
    *   **Viewport & Scissor:** Defines the region of the framebuffer that the output will be rendered to.
    *   **Rasterization:** How triangles are converted into fragments (pixels), including polygon mode, culling, and line width.
    *   **Multisampling:** Used for anti-aliasing.
    *   **Color Blending:** How the new fragment colors are combined with existing colors in the framebuffer.
*   **Connection:** All these stages and their configurations are bundled together into a `VkPipeline` object. This object is then "bound" to a command buffer before drawing.

**Command Pool & Command Buffers:**
*   **Command Pool (`VkCommandPool`):** A command pool manages the memory for command buffers. It's an efficient way to allocate and deallocate command buffers.
*   **Command Buffers (`VkCommandBuffer`):** These are essentially lists of commands that are submitted to a queue on the GPU. Instead of sending individual commands, we record all drawing commands for a frame into a command buffer and submit it once. This greatly reduces CPU overhead.
*   **Connection:** Command buffers are allocated from a command pool. Once allocated, you record all render pass commands, pipeline binding, and draw calls into them.

**Queues:**
A Vulkan physical device (GPU) exposes one or more queue families, each capable of specific operations (e.g., graphics, compute, transfer, present).
*   **Graphics Queue:** Used for submitting graphics commands (e.g., drawing).
*   **Present Queue:** Used for submitting commands to present rendered images to the screen.
*   **Connection:** Command buffers are submitted to queues. Synchronization objects (semaphores and fences) coordinate work between different queues or between the CPU and GPU.

**Synchronization: Semaphores and Fences:**
These objects are crucial for coordinating operations between the CPU and GPU, and between different GPU operations.

*   **Semaphores (`VkSemaphore` - GPU-GPU Synchronization):**
    *   Used to synchronize operations within the GPU itself, or between different queues.
    *   One operation *signals* a semaphore when it's done, and another operation *waits* for that semaphore before starting.
    *   **Example in `drawFrame`:**
        *   `imageAvailableSemaphore`: Signaled by `vkAcquireNextImageKHR` when an image from the swap chain is ready for rendering. The graphics queue waits for this semaphore before starting to draw.
        *   `renderFinishedSemaphore`: Signaled by the graphics queue after all drawing commands for a frame are complete. The present queue waits for this semaphore before presenting the image to the screen.
    *   **Analogy:** A "Go/No-Go" flag for GPU tasks.

*   **Fences (`VkFence` - CPU-GPU Synchronization):**
    *   Used to synchronize between the CPU and the GPU. The CPU can wait on a fence to know when the GPU has completed a submitted command buffer.
    *   **Example in `drawFrame`:**
        *   `inFlightFence`: Signaled by the graphics queue after a command buffer has finished execution. The CPU waits for this fence (`vkWaitForFences`) at the beginning of the next frame to ensure the previous frame's work is complete before recording new commands. This prevents the CPU from overwriting resources the GPU is still using.
    *   **Analogy:** A "task completion" flag for the CPU.

### Frame Rendering Flow (Simplified ASCII Art)

```
+------------------+     +------------------------+     +-------------------+     +---------------------+
|                  |     |                        |     |                   |     |                     |
|    CPU (Main)    | --- |  vkWaitForFences()     | --- |  vkAcquireNextImg | --- |  Record Command Buf |
|                  |     |   (Wait for GPU)       |     |   (Image Ready:   |     | (Clear, Bind Pipe,  |
|                  |     |                        |     |   Signal Sema A)  |     |  Draw, End Render)  |
+------------------+     +------------------------+     +-------------------+     +---------------------+
                                     |                             |                             |
                                     V                             V                             V
+-------------------------------------------------------------------------------------------------------+
|                                           GPU                                                         |
|                                                                                                       |
|  [Graphics Queue] <--- vkQueueSubmit() <--- (Wait: Sema A, Signal: Sema B, Signal: Fence) -----------+
|      (Drawing)                                                                                        |
|                                                                                                       |
|  [Present Queue]  <--- vkQueuePresentKHR() <--- (Wait: Sema B)                                        |
|      (Display)                                                                                        |
+-------------------------------------------------------------------------------------------------------+
```

---

```
1. Initialize the window
2. Create a vulkan instance
    - Set the proper extensions (need glfw ones)
    - Turn on any validation layers
3. Find the physical devices available and pick the correct one
    - Check which physical devices are available (and filter based off features or properties we want)
    - Check if they have the queues we want (specifically graphics queue)
4. Create a logical device based off that physical device
    - Pick which queues we want
    - Set the priority of those queues
    - Set the proper extensions 
    - Turn on any validation layers
5. Create a surface to present on and attach to a window (GLFW handles most of it, but can be platform dependent)
6. Create a swap chain, which is essentiallty a queue of images waiting to be presented on the screen
    - Swap chains need Image Views to be able to view a image. They describe what aprt of the image they can access and how to access it.
7. Create the GPU pipeline
    - Create shader modules for the vertex and fragment shaders
    - Compile the shaders into a spv format and read them as buffers, finally pass these into the shader modules
8. Setup fixed functions like the viewport, rasterizer, multisampling, depth testing, and color blending
9. Create render pass objects to define how framebuffer attachements will be used during rendering
10. Create framebuffers that represents of thte ImageView obejcts in the attachment.
11. Create a command pool which we draw command buffers from. Then we can use this to begin a render pass, which includes clearing the image and how buffers are saved
12. Draw frames and synchronize using Semaphores and Fences. Then record the command buffer and tae care of subpasses in render pass.


## Build

```bash
cd RefactoredHelloTriangle
mkdir -p build
cd build
cmake ..
cmake --build .
```

Run the app: `./build/HelloTriangle`

## Project

- Dependencies: Vulkan SDK, GLFW3
- Shader source in `shaders/` directory (`.vert`, `.frag` + compiled `.spv`)
- No tests, no linting