# LED-Matrix-Grad
This project contains a basic Pi Pico driver I wrote for a 32x32 RGB Matrix, aswell as a python tool to convert pngs/gifs into .h files containing RGB byte values. It should also work on any microcontroller supported by the Arduino IDE (given it is fast enough).

To use the python tool you can either provide a file/folder by command line argument, or if you dont provide one, it will prompt you to drag and drop one. If no argument is given at the prompt it defaults to processing the entire ImagesToConvert folder.. (creating the .h files in ConvertedImages and updating the graphics.h file)

There are 2 Fonts provided, a 4x6 size and a 10x12 size.. depending on how much space is available you can include a larger size or just double the 10x12
font sources: ..

These are the settings I used: <br>
<img src="https://github.com/JShweiri/LED-Matrix-Grad/blob/master/settings.png?raw=true">
