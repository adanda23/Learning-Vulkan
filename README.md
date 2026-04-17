# Notes

## Instance initializiation

After setting up a simple GLFW window, we need to begin by setting up a vulkan instance. A vulkan instance specifies details about the application that a driver might use for optimization, as well as extensions that we might need to use immedietaly. Since this project uses GLFW for windows, we need to set that as a extension. GLFW also has a helper function to get the required extensions, so we can use that. We also need to specify the validation layers that we want to use, and the debug messenger that will be used to print out any validation errors. Validation layers are just layers on top of our program that check for certain errors. In this project, we use the standard vulkan Khronos validation layer.

## Physical and Logical devices

Next we need to setup our physical and logical devices. The physical device is the literal GPU that is going to be used in the program. We need to check which extensions we want and whether or not our physical device supports it. Additionally, we need to create a logical device. A logical device is a way to interact with the physical device. It contains more details like which queues, features, and any extensions we want to use. Note that physical device extensions are focused on hardware specific features while logical device extensions are focused on software features. For example, the swapchain extension is a logical device extension since it is a software feature that allows us to present images to the screen.

## Window surfaces

Next, we need to create our window surface. A window surface is a abstract surface that we can present images to. This goes after vulkan initializiation as physical devices are dependent on the surface. This is because some physical devices might not support certain surface formats, so we need to create the surface before we can select a physical device.

## Swapchain

We now need to check for swap chain support. Aa swapchain is just a  queue of images that get presented onto the screen. As mentioned previously, we need to check if our physical device supports the swapchain extension, and if it does, we need to check for the details of the swapchain support. The swapchain consists of a few settings like Surface format, presentation mode, and swap extent. In other words, the color depth, condition for swapping images, and the resolution of the images in the swapchain. Additionally, to be able to see these Images in the swapchain, we need to use a image view. An image view explains how to access and image, wihch part of it to access, and mipmapping.
Graphics pipeline

After these initial setup steps, we can finally get to the graphics pipeline. The pipeline consists of setting up vertices, doing any transformations to them, rasterizing them, coloring them with a fragment shader, and doing any blending after. Some of these steps are fixed function, such as the rasterizer, while others are programmable. The way we define how to transform the vertices and color the pixels is with vertex and fragment shaders. Its important to compile these shaders down to SPIR-V, which is a binary format that vulkan can read. We can use the glslc tool to do this. Additionally, you need to setup the fixed function stages such as color blending, input assembly, depth tests, viewports and scissors, and rasterizer. Finally we need to setup Render passes and subpasses which specify how many color and depth buffers there will be, how many samples to use for each of them and how their contents should be handled throughout the rendering operations. The framebuffer itself is just a collection of attachments that are used during rendering. It needs to be compatible with the render pass, and it needs to specify the actual image views that will be used as attachments.

## Command buffers

Commands in Vulkan are recorded with command buffers, which are stored in command pools. Command buffers get submitted to the device queues. commands include setting the viewport, set the scissor, using the aforementined pipeline, and actually drawing to the window.

## Other

### Need to further study these topics

1. swapchain vs framebuffer
2. command buffers
3. render passes

### Credits

Much of the setup is being taken from [link to tutorial](https://vulkan-tutorial.com).
