# Poori tasveer = upar se neeche dabbe (ChatGPT jaisa stack). Use: python stack.py <spec.json> <out.svg>
# spec: {"title": "...", "boxes": [["Naam", "ek line"], ...], "hi": 2, "foot": "cross-cutting: ...", "note": "..."}
import json, sys
from html import escape as e

W, BW, BH, GAP = 760, 640, 64, 34
spec = json.load(open(sys.argv[1], encoding='utf-8'))
boxes = spec['boxes']
x = (W - BW) // 2
y = 70
parts = []
for i, (name, line) in enumerate(boxes):
    hi = i == spec.get('hi', -1)
    fill = '#10243a' if hi else '#161b22'
    stroke = '#3b6ea8' if hi else '#30363d'
    fg, sub = ('#ffffff', '#c9d6e3') if hi else ('#e6edf3', '#9da7b3')
    parts.append(f'<rect x="{x}" y="{y}" width="{BW}" height="{BH}" rx="10" fill="{fill}" stroke="{stroke}"/>')
    parts.append(f'<text x="{W//2}" y="{y+27}" text-anchor="middle" font-size="16" font-weight="700" fill="{fg}">{e(name)}</text>')
    parts.append(f'<text x="{W//2}" y="{y+48}" text-anchor="middle" font-size="13" fill="{sub}">{e(line)}</text>')
    y += BH
    if i < len(boxes) - 1:
        parts.append(f'<text x="{W//2}" y="{y+23}" text-anchor="middle" font-size="16" fill="#9da7b3">&#8595;</text>')
        y += GAP
y += 24
for key in ('foot', 'note'):
    if spec.get(key):
        parts.append(f'<text x="{W//2}" y="{y}" text-anchor="middle" font-size="12.5" fill="#9da7b3">{e(spec[key])}</text>')
        y += 22
H = y + 10
svg = (f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
       f'font-family="Segoe UI, Helvetica, Arial, sans-serif">'
       f'<rect x="0.5" y="0.5" width="{W-1}" height="{H-1}" rx="14" fill="#0d1117" stroke="#30363d"/>'
       f'<text x="{W//2}" y="40" text-anchor="middle" font-size="18" font-weight="700" fill="#e6edf3">{e(spec["title"])}</text>'
       + ''.join(parts) + '</svg>')
open(sys.argv[2], 'w', encoding='utf-8').write(svg)
print('ok', H)
