# World Generator

## Isometric world made with procedural world generation

This is a small project, that shows what you can do with just a simple Perlin noise.

* Generates a Perlin Noise
* Transforms the black and white pixels to colored ones ( simple 2D map )
* Gives each pixel (Square) a block type and height
* Uses Isometric projection to create a simple world

## Resources
* [Perlin Noise WIKI](https://en.wikipedia.org/wiki/Perlin_noise)
* [Video that explains the isometric project the best](https://www.youtube.com/watch?v=04oQ2jOUjkU)

## Customization
Small ImGui UI where you can change:

### Perlin Noise Settings
- ![image](https://github.com/user-attachments/assets/e5479061-f91e-411b-8993-bc21cdaab39c)
### Block type Heights
- ![image](https://github.com/user-attachments/assets/6a2a5616-c0cd-41a5-ac0e-126403be292b)
### Isometric Projection Settings
- ![image](https://github.com/user-attachments/assets/28ee1e92-c8fa-4ec0-8e36-c9cb87886a09)

## Aditional things
### Waves
- ![image](https://github.com/user-attachments/assets/5dcb7e61-4395-4158-97e2-68b918c706e9)
### Trees
- ![image](https://github.com/user-attachments/assets/f23bf139-a1cd-4a98-9428-7dfd4b8528cd)
### Block outline
- ![image](https://github.com/user-attachments/assets/dc1c1f31-db31-451d-ae41-42e289594c42)

## Screenshots
![image](https://github.com/user-attachments/assets/a89112d3-cae7-4f6b-be3c-48b3ee1af055)
![image](https://github.com/user-attachments/assets/084d2bab-81e4-4ba7-a474-e68ceeb347b0)
![image](https://github.com/user-attachments/assets/e276c1d3-df33-4aba-a624-970394a9f3ac)

## Known Issues
* Trees are drawn last, because of that they can appear on top of mountains even if they are below.
* The block outline is buggy at the top blocks of the isometric projection.
