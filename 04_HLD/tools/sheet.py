"""Ek card ke scene ek ke neeche ek jodo (khud dekhne ke liye). python sheet.py <img_dir> <cN> <out.png>"""
import sys, glob
from PIL import Image
d, c, out = sys.argv[1:4]
fs = sorted(glob.glob(f'{d}/{c}_[0-9].png'))
s = Image.new('RGB', (900, 405 * len(fs)), 'white')
for i, f in enumerate(fs): s.paste(Image.open(f), (0, i * 405))
s.save(out); print(len(fs), 'scene')
