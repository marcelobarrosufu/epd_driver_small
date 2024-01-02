'''
MIT License

Copyright (c) 2023- Marcelo Barros de Almeida <marcelobarrosalmeida@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
'''
import sys
from PIL import Image

if len(sys.argv) != 2:
    print('Usage: python3 gen_image.py <image_file>')
    sys.exit(1)

fin_name = sys.argv[1]
fvar_name = fin_name.replace('.', '_')
fout_name = fvar_name + '.h'
img = Image.open(fin_name)

data = []
nbits = 1
bits = 0
width, height = img.size
print(f'IMG {width} x {height}')
for y in range(height):
    for x in range(width):
        r,g,b = img.getpixel((x,y))
        gs = (r+g+b)/3
        vb = 1 if gs < 50 else 0
        bits = bits | (vb << (8-nbits))
        nbits = nbits + 1
        if nbits > 8:
            data.append(bits)
            bits = 0
            nbits = 1

if nbits != 1:
    bits = bits << (8-nbits)
    data.append(bits)

nc = 0
with open(fout_name,'wt') as f:
    f.write(f'static uint8_t {fvar_name}[] = {{\n')
    for d in data:
        f.write('%3d, ' %d)
        nc = nc + 1
        if nc == 16:
            nc = 0
            f.write('\n')
    f.write('\n};\n')


