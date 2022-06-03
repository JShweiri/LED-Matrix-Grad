
# Removes alpha bytes
from PIL import Image
import sys

if(len(sys.argv) > 1):
  filename = sys.argv[1]
else:
  filename = input('Enter filename:')
#add folder option?

red_image = Image.open(filename)

name = filename.split("\\")[-1].split(".")[0]

red_image_rgb = red_image.convert("RGB")

rgb = []

result = "unsigned char " + name + "[32][32][3] = {\n"

for i in range(1024):
  for item in red_image_rgb.getpixel((i%32,i/32)):
    result = result + "0x" + hex(item)[2:].zfill(2) + ", "
  if i%32 == 31:
    result += "\n"

result = result + "};"

f = open(name + ".h", "w")
f.write(result)
f.close()

print("file created!")
