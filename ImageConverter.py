import sys
import os
from PIL import Image

def convert_png_to_qgt(png_path):
    if not os.path.exists(png_path):
        print(f"Error: File '{png_path}' does not exists.")
        return

    base_name = os.path.splitext(png_path)[0]
    qgt_path = f"{base_name}.qgt"

    try:
        with Image.open(png_path) as img:
            img = img.convert('RGBA')
            width, height = img.size
            pixels = img.getdata()
            with open(qgt_path, 'w') as f:
                f.write(f"{width} {height}\n")
                for r, g, b, a in pixels:
                    f.write(f"{r} {g} {b} {a}\n")
            print(f"Success! Converted '{png_path}' to '{qgt_path}'! ({width}x{height})")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Using: python ImageConverter.py <file.png>")
    else:
        convert_png_to_qgt(sys.argv[1])
