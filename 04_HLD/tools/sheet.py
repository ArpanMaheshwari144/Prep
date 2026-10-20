"""Ek card ke scene ek ke neeche ek jodo (khud dekhne ke liye). python sheet.py <img_dir> <cN> <out.png>"""
import sys, glob
from PIL import Image
d, c, out = sys.argv[1:4]
fs = sorted(glob.glob(f'{d}/{c}_[0-9].png'))
ims = [Image.open(f) for f in fs]   # naya saancha 610 tak lamba, isliye asli uchai
s = Image.new('RGB', (900, sum(im.height + 5 for im in ims)), 'white')
y = 0
for im in ims: s.paste(im, (0, y)); y += im.height + 5
s.save(out); print(len(fs), 'scene')
