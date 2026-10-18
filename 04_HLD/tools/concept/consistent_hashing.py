"""Consistent hashing ring (10_distributed_cache card 2). Chalao: python consistent_hashing.py
Output: tools/concept/out/consistent_hashing.svg (render_diagrams.py design ke neeche jodta)
Ring 0..360 (file ke TASVEER jaisa): A=100, B=200, D=250 (nayi), C=300. user:42 hash = 230.
"""
import math, os
W, H, CX, CY, R = 960, 640, 300, 300, 210
ROT = -70   # ghuma ke dikhao: 250 neeche aaye, wahan jagah
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'out', 'consistent_hashing.svg')

def pt(deg, r=R):
    a = math.radians(deg + ROT)
    return CX + r * math.sin(a), CY - r * math.cos(a)

def arc(d1, d2, r):
    x1, y1 = pt(d1, r); x2, y2 = pt(d2, r)
    large = 1 if (d2 - d1) % 360 > 180 else 0
    return f'M{x1:.1f},{y1:.1f} A{r},{r} 0 {large} 1 {x2:.1f},{y2:.1f}'

o = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
     'font-family="Arial, Helvetica, sans-serif">',
     '<defs><marker id="ar" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">'
     '<path d="M0,0 L10,5 L0,10 z" fill="#333"/></marker>'
     '<marker id="arr" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">'
     '<path d="M0,0 L10,5 L0,10 z" fill="#d32f2f"/></marker></defs>',
     f'<rect width="{W}" height="{H}" fill="white"/>',
     f'<text x="{CX}" y="40" text-anchor="middle" font-size="22" font-weight="bold">'
     'Consistent hashing: ring (gol ghera) 0..360</text>',
     f'<circle cx="{CX}" cy="{CY}" r="{R}" fill="none" stroke="#999" stroke-width="3"/>']
for d, lab in [(0, '0 / 360')]:
    x, y = pt(d, R + 22)
    o.append(f'<text x="{x:.0f}" y="{y+5:.0f}" text-anchor="middle" font-size="13" fill="#888">{lab}</text>')

# key 230 -> clockwise to C(300) before D; after D -> D(250)
o.append(f'<path d="{arc(232, 296, R-28)}" fill="none" stroke="#333" stroke-width="2.5" marker-end="url(#ar)"/>')
x, y = pt(262, R - 70)
o.append(f'<text x="{x:.0f}" y="{y:.0f}" text-anchor="middle" font-size="14">pehle: aage pehli = C</text>')
o.append(f'<path d="{arc(234, 247, R+40)}" fill="none" stroke="#d32f2f" stroke-width="3" '
         'stroke-dasharray="7,5" marker-end="url(#arr)"/>')
x, y = pt(244, R + 66)
o.append(f'<text x="{x:.0f}" y="{y:.0f}" text-anchor="middle" font-size="14" fill="#d32f2f">D judi: ab D pe</text>')

machines = [(100, 'A', '#1976d2'), (200, 'B', '#7b1fa2'), (300, 'C', '#00897b')]
for d, n, c in machines:
    x, y = pt(d)
    o.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="26" fill="{c}"/>')
    o.append(f'<text x="{x:.1f}" y="{y+7:.1f}" text-anchor="middle" font-size="20" fill="white" '
             f'font-weight="bold">{n}</text>')
    lx, ly = pt(d, R - 50)
    o.append(f'<text x="{lx:.0f}" y="{ly+5:.0f}" text-anchor="middle" font-size="13" fill="#555">{d}</text>')
x, y = pt(250)
o.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="26" fill="white" stroke="#2e7d32" stroke-width="3" '
         'stroke-dasharray="6,4"/>')
o.append(f'<text x="{x:.1f}" y="{y+7:.1f}" text-anchor="middle" font-size="20" fill="#2e7d32" '
         'font-weight="bold">D</text>')
lx, ly = pt(250, R - 50)
o.append(f'<text x="{lx:.0f}" y="{ly+5:.0f}" text-anchor="middle" font-size="13" fill="#2e7d32">250 (nayi)</text>')
x, y = pt(230)
o.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="11" fill="#f9a825" stroke="#333"/>')
lx, ly = pt(214, R + 46)
o.append(f'<text x="{lx:.0f}" y="{ly+5:.0f}" text-anchor="middle" font-size="15" font-weight="bold">'
         'user:42 (230)</text>')

lines = [('NIYAM', True), ('key ring pe aage chale,', False), ('pehli machine uski', False), ('', False),
         ('D (250) JUDI', True), ('sirf 201..250 wali key', False), ('C se D pe gayi (user:42 bhi)', False),
         ('baaki saari key apni jagah', False), ('', False),
         ('C GIRI', True), ('C ki key aage wali A pe,', False), ('sirf unki ek baar miss', False), ('', False),
         ('KAUN KARTA?', True), ('App ki library (client) khud', False), ('hash karke machine chunti', False)]
y = 110
for t, b in lines:
    if t:
        o.append(f'<text x="600" y="{y}" font-size="{17 if b else 16}" '
                 f'{"font-weight=\"bold\" fill=\"#d32f2f\"" if b else "fill=\"#222\""}>{t}</text>')
    y += 26
o.append('</svg>')
os.makedirs(os.path.dirname(OUT), exist_ok=True)
open(OUT, 'w', encoding='utf-8').write('\n'.join(o))
print('wrote', os.path.abspath(OUT))
