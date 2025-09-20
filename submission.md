# Project 1: Brush

Please fill this out for Brush only. The project handout can be found [here](https://cs1230.graphics/projects/raster/1).

## Output Comparison
This is how you will demonstrate the functionality of your project.

For each of the rows below, follow the instructions to record a video. Please reference the [Screen Recording Guide](https://cs1230.graphics/docs/screen-recording/) for machine-specific guidance on how to do this.

Once you've recorded everything, navigate to this file in Github, click edit, and either select or drag-and-drop each of your videos into the correct location. This will upload them to GitHub (but not include them in the repo) and automatically embed them into this Markdown file by providing a link. Make sure to double-check that they all show up properly in the preview.

We're **not** looking for your video to **exactly** match the expected output (i.e. draw the exact same shape). Just make sure to follow the instructions and verify that the recording fully demonstrates the functionality of that part of your project.

### Constant Brush
**Instructions:** Draw with the constant brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/241d99b5-947a-407c-b454-410534520aad

#### Your Output


https://github.com/user-attachments/assets/765633bb-2cc0-48f9-8e99-f6cb76b7347f


<!---
Paste your output on top of this comment!
-->



### Linear Brush
**Instructions:** Draw with the linear brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/9706fa04-7710-441f-b292-ab010e04dec6

#### Your Output


https://github.com/user-attachments/assets/28e51709-199f-41f8-8fcc-5a20d1ca3072


<!---
Paste your output on top of this comment!
-->



### Quadratic Brush
**Instructions:** Draw with the quadratic brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/c5df5c09-bfe0-4c05-a56e-14609772d675

#### Your Output


https://github.com/user-attachments/assets/71483ca4-8a73-4fbe-9221-6deb25639822


<!---
Paste your output on top of this comment!
-->



### Smudge Brush
**Instructions:** Draw some colors on the canvas and use the smudge brush to smear them together.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/26440b63-2d1c-43fd-95f2-55b74ad3bbed

#### Your Output


https://github.com/user-attachments/assets/daaef5c2-544c-4fc0-9b90-e5260038f798


<!---
Paste your output on top of this comment!
-->



### Smudge Brush Change in Alpha
**Instructions:** Draw some colors on the canvas. Use the smudge brush with varying alpha levels (use at least three) and demonstrate that the brush still works the same way each time.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/0b49c7d0-47ca-46d0-af72-48b831dfe7ea

#### Your Output

I disallowed changing alpha, or even changing colour of Smudge brush to begin with. Its colour is copmletely determined by the click.
<!---
Paste your output on top of this comment!
-->



### Radius
**Instructions:** Use any brush with at least 3 different values for the radius.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/6f619df6-80cd-4849-8831-6a5aade2a517

#### Your Output


https://github.com/user-attachments/assets/ef8e918a-9190-4e6f-80e0-35d64584fe0b


<!---
Paste your output on top of this comment!
-->



### Color
**Instructions:** Use any brush to draw red (255, 0, 0), green (0, 255, 0), and blue (0, 0, 255).

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/fd9578ca-e0af-433e-ac9e-b27db2ceebc9

#### Your Output


https://github.com/user-attachments/assets/ae62ef0c-143b-451d-b52b-8f8737383d27


<!---
Paste your output on top of this comment!
-->



### Canvas Edge Behavior
**Instructions:** With any brush, click and draw on the canvas in a place where the mask intersects with the edge. Then, start drawing anywhere on the canvas and drag your mouse off of the edge.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/f5344248-fa5f-4c33-b6df-ff0a45011c7a

#### Your Output


https://github.com/user-attachments/assets/40534758-1ae6-46e9-9de5-5936b41a5294


<!---
Paste your output on top of this comment!
-->



### Alpha
**Instructions:** With the constant brush, draw a single dot of red (255, 0, 0) with an alpha of 255. Then, draw over it with a single dot of blue (0, 0, 255) with an alpha of 100. You should get a purpleish color.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/b13d312d-d6d4-4375-aeaa-96174065443b

#### Your Output


https://github.com/user-attachments/assets/7424539c-fe6f-4a2a-91ab-fecc7dce6eb0


<!---
Paste your output on top of this comment!
-->



### Alpha of Zero
**Instructions:** Choose any brush and demonstrate that it will not draw if the alpha value is zero.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/8e48777e-8196-401e-9af6-871abe712146

#### Your Output
(Note that the format is a u32 with alpha-red-green-blue each representing two hex digits)

https://github.com/user-attachments/assets/fd1e07e5-ba93-41dd-8c2f-458f29df33b1


<!---
Paste your output on top of this comment!
-->



## Design Choices
### Windowing System
For purely educational reasons, I used Wayland directly to create my window, handle mouse and keyboard events etc...

I implemented my custom event handler, my custom window class, and allocate my custom rendering buffers. The rendering is entirely on the cpu and no usage of GPU APIs is in this project.

### Colour Representation
Colour is represented here as u32. Each two hex digits (a byte each) rerpesents a channel. Thus, a colour may look like 0xff000000 which has ff for alpha (max alpha) 00 for every other channel.

There are helpers to set and get channels, and they are implemented inline and using bitwise operations. There is also a function to unpack a u32 colour to an array of u8 channels and a function to combine an array of 4 u8 values to a u32 colour. Floats were only used for alpha blending.

### Coommon Classes
#### Canvas
Canvas is actually a struct and has two int values (for width and height) a u32 array of pixels and a second u32 array for alpha fixing. More on this second array later. All members and methods are public since canvas is widely used by almost all other classes.

#### Tool
This class represents all types of tools that can be used by user. It has two members of type i64 for x and y coordinated. It has a pure virutal function for drawing (which takes a canvas pointer) and a pure virutal fucntion to get the type of tool. Otherwise, it has getters and setters for x and y.

There is an associated enum class called ToolType. The only types here are fill and brush. This should be extended
later to include more types and is useful for handling key and mouse events. This helps with having fewer handler augmented by a switch case on the tool type to modify behaviour.

#### Brush
A Tool with a radius, a mask, a boolean to indicate if alpha_fixing should be applied and a boolean to indicate if mask is already filled. The draw function of tool is overriden here and merelt blends the values in the mask with the canvas. Other Brushes inherit from Brush and modify its behaviour.

##### Simple Brush
A parituclar type of brush that has a colour. Among Simple Brushes is the constant Brush (since Simple Brush is not pure virtual, constant Brush is just Simple Brush), Linear Brush, Quadratic Brush, Speed Brush, and Spray Brush. Essentially anything that has a constant colour value (regardless of how this value is later used to fill the mask) is a Simple Brush. Smudge Brush is the only brush implemented here that is not simple, since it has no colour and fills its mask from the current click.

#### Fill
The Fill class inherits from Tool. Its draw function is a flood-fill algorithm implemented iteratively. It has a constant colour that can be set by user and fills the contionus region based on it.

### Additional Classes
Othe Classes and parts of the proejct are not directly graphics related and are related more to handling events, Having a custom cursor, etc... Thus, their desc. is ommited here.

## Collaboration/References

## Known Bugs

## Extra Credit
### Fill
Uses flood-fill iteratively to fill a contionous region. Maintains a boolen array to indicate if a pixel was already proccessed or not.

https://github.com/user-attachments/assets/cd12baed-de82-46f0-9609-48c7274df11f

### Fixing Alpha Blending
The idea is to store the old blend value in an array. On draw, check if the value at current pixel is already filled, and if so, compute old colour (by reversing the belnding formula) and reblend.

https://github.com/user-attachments/assets/c7df49a5-de95-4152-962c-47bd0072cb12

### Speed Brush
Only relies on previous x and y. No fancy speed calculations or anything, just composes two log functions that ensure slow growth while also ensuring no math error results.

https://github.com/user-attachments/assets/dd5b5fb6-eab4-4668-b3e8-4db39a0e0c0f

### Spray Brush
For its mask, it determines whether to fill it based on 1) whether it is in the circle or not, and 2) by calculating a random number between 0 and 100. If the value is less than indicated density fill, otherwise, set it to 0x00000000.

https://github.com/user-attachments/assets/71f6249c-4cf7-4216-af98-31df5c877c97


