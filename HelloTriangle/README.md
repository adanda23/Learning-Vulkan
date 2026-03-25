# Process

1. initialize the window
2. create a vulkan instance
    - set the proper extensions (need glfw ones)
    - turn on any validation layers
3. find the physical devices available and pick the correct one
    - check which physical devices are available (and filter based off features or properties we want)
    - check if they have the queues we want (specifically graphics queue)
4. create a logical device based off that physical device
    - pick which queues we want
    - set the priority of those queues
    - set the proper extensions 
    - turn on any validation layers