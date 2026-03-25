# Process

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