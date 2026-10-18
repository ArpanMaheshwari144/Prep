"""LRU cache = HashMap + doubly linked list (10_distributed_cache card 1). Chalao: python lru.py
Output: tools/concept/out/lru.svg (render_diagrams.py design ke neeche jodta)
"""
import os
W, H = 960, 420
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'out', 'lru.svg')
o = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
     'font-family="Arial, Helvetica, sans-serif">',
     '<defs><marker id="la" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">'
     '<path d="M0,0 L10,5 L0,10 z" fill="#555"/></marker></defs>',
     f'<rect width="{W}" height="{H}" fill="white"/>',
     '<text x="30" y="36" font-size="22" font-weight="bold">LRU = HashMap + doubly linked list (dono O(1))</text>']
# HashMap
o.append('<text x="40" y="80" font-size="16" font-weight="bold" fill="#1976d2">HashMap (key -&gt; list ka dabba)</text>')
keys = ['user:9', 'user:3', 'user:7']
for i, k in enumerate(keys):
    y = 100 + i * 52
    o.append(f'<rect x="40" y="{y}" width="120" height="38" fill="#e3f2fd" stroke="#1976d2"/>')
    o.append(f'<text x="100" y="{y+25}" text-anchor="middle" font-size="16">{k}</text>')
# list nodes
nx = [330, 530, 730]
o.append('<text x="330" y="80" font-size="16" font-weight="bold" fill="#00897b">doubly linked list</text>')
for i, k in enumerate(keys):
    x = nx[i]
    fill = '#f9a825' if i == 0 else '#e0f2f1'
    o.append(f'<rect x="{x}" y="160" width="130" height="50" rx="6" fill="{fill}" stroke="#00897b" stroke-width="2"/>')
    o.append(f'<text x="{x+65}" y="191" text-anchor="middle" font-size="17" font-weight="bold">{k}</text>')
    if i < 2:
        o.append(f'<line x1="{x+130}" y1="178" x2="{nx[i+1]-4}" y2="178" stroke="#555" stroke-width="2" marker-end="url(#la)"/>')
        o.append(f'<line x1="{nx[i+1]}" y1="194" x2="{x+134}" y2="194" stroke="#555" stroke-width="2" marker-end="url(#la)"/>')
o.append('<text x="395" y="240" text-anchor="middle" font-size="15" fill="#00897b" font-weight="bold">HEAD = abhi chhua</text>')
o.append('<text x="795" y="240" text-anchor="middle" font-size="15" fill="#d32f2f" font-weight="bold">TAIL = sabse purana</text>')
# map pointers: har key seedha apne list dabbe tak
for i in range(3):
    y = 119 + i * 52
    o.append(f'<line x1="160" y1="{y}" x2="{nx[i]+65}" y2="157" stroke="#1976d2" stroke-width="1.5" '
             'stroke-dasharray="5,4" marker-end="url(#la)"/>')
rows = [('GET user:9', 'map se seedha dabba mila (O(1)) -> use HEAD pe le aao'),
        ('RAM bhar gayi', 'TAIL wala (user:7) bahar, map se bhi hatao (O(1))'),
        ('TTL', 'har key ki umar: khatam to padhte waqt hi bekaar, bahar')]
y = 300
for a, b in rows:
    o.append(f'<text x="40" y="{y}" font-size="16" font-weight="bold" fill="#d32f2f">{a}</text>')
    o.append(f'<text x="200" y="{y}" font-size="16" fill="#222">{b}</text>')
    y += 34
o.append('</svg>')
os.makedirs(os.path.dirname(OUT), exist_ok=True)
open(OUT, 'w', encoding='utf-8').write('\n'.join(o))
print('wrote', OUT)
