"""Card poster banane ka saancha (Docs 11 wala style, niyam pakke).
python 04_HLD/tools/gen_poster.py <cN_poster.json> [...]  -> cN_poster.html (phir node poster2png.js se PNG)

spec (json):
  title, h (height)
  boxes:    [{x, y, w, h, kind: user|server|cache|db|lb|plain, title, lines: [..]}]
  circles:  [{cx, cy, r, label}]   (ring)
  arrows:   [{pts: [[x,y],..], c: blue|green|purple|grey|orange, dash: bool, label, lx, ly, a: start|middle|end}]
  callouts: [{n, type: x|pehle|grey, x, y, w, red, green, green2, mark: [x,y], conn: [[x,y],..]}]
NIYAM: gira = dabbe pe X · raasta toota = teer pe X · pehle / galat tareeka = PEELA, X nahi · doosra kyun nahi = GREY.
"""
import json
import sys
from html import escape

COL = {'blue': '#4aa8ff', 'green': '#3ddc97', 'purple': '#a98bff', 'grey': '#6b7a90', 'orange': '#ffa94d',
       'red': '#ff5a6a', 'yel': '#ffcc4d'}
BOX = {'user': ('#10243f', '#4aa8ff'), 'server': ('#0f2a22', '#3ddc97'), 'cache': ('#2a1a10', '#ffa94d'),
       'db': ('#1d1838', '#a98bff'), 'lb': ('#152033', '#9fb0c8'), 'plain': ('#152033', '#4aa8ff')}
SUB = 'Laal X = yahan gira / toota · Peela dabba = pehle aisa tha (X nahi) · har dabbe me hara = kaise bacha'


def t(x, y, s, size, fill, bold=False, mono=False, anchor='start'):
    return (f'<text x="{x}" y="{y}" font-size="{size}"{" font-weight=\"700\"" if bold else ""} fill="{fill}"'
            f'{" class=\"m\"" if mono else ""}{"" if anchor == "start" else f" text-anchor=\"{anchor}\""}>{escape(s)}</text>')


def xmark(x, y, r=12):
    return (f'<g stroke="#ff5a6a" stroke-width="5" stroke-linecap="round"><line x1="{x-r}" y1="{y-r}" x2="{x+r}" y2="{y+r}"/>'
            f'<line x1="{x+r}" y1="{y-r}" x2="{x-r}" y2="{y+r}"/></g>')


def build(spec):
    W, H = 1500, spec['h']
    o = [f'<svg width="{W}" height="{H}" viewBox="0 0 {W} {H}" xmlns="http://www.w3.org/2000/svg"><defs>']
    for k in ('blue', 'green', 'purple', 'grey', 'orange'):
        o.append(f'<marker id="a{k}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
                 f'<path d="M0,0 L10,5 L0,10 Z" fill="{COL[k]}"/></marker>')
    o.append(f'</defs><rect width="{W}" height="{H}" fill="#0b1220"/>')
    o.append(t(30, 40, spec['title'], 26, '#e6edf3', True))
    o.append(t(30, 68, SUB, 16, '#9fb0c8'))
    for r in spec.get('circles', []):   # ring jaisa gol (dashed)
        o.append(f'<circle cx="{r["cx"]}" cy="{r["cy"]}" r="{r["r"]}" fill="none" stroke="#2a3a5a" stroke-width="3" stroke-dasharray="6 6"/>')
        if r.get('label'):
            o.append(t(r['cx'], r['cy'] + 5, r['label'], 15, '#6b7a90', mono=True, anchor='middle'))
    for b in spec['boxes']:
        fill, st = BOX[b['kind']]
        x, y, w, h = b['x'], b['y'], b['w'], b['h']
        o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="12" fill="{fill}" stroke="{st}" stroke-width="2.5"/>')
        o.append(t(x + w / 2, y + 34, b['title'], 19, '#e6edf3', True, anchor='middle'))
        for i, ln in enumerate(b.get('lines', [])):
            o.append(t(x + w / 2, y + 60 + i * 22, ln, 13, '#c9d6e8', mono=True, anchor='middle'))
    for a in spec['arrows']:
        c = a.get('c', 'blue')
        d = 'M' + ' L'.join(f'{p[0]} {p[1]}' for p in a['pts'])
        o.append(f'<path d="{d}" fill="none" stroke="{COL[c]}" stroke-width="3"'
                 f'{" stroke-dasharray=\"8 6\"" if a.get("dash") else ""} marker-end="url(#a{c})"/>')
        if a.get('label'):
            o.append(t(a['lx'], a['ly'], a['label'], 13, COL[c], mono=True, anchor=a.get('a', 'start')))
    for c in spec['callouts']:
        kind = c.get('type', 'x')
        x, y, w = c['x'], c['y'], c['w']
        h = 86 if c.get('green2') else 62
        if kind == 'grey':
            o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="9" fill="#141c2b" stroke="#6b7a90" stroke-width="2"/>')
            o.append(t(x + 22, y + 27, c['red'], 15, '#c9d6e8', True))
            o.append(t(x + 22, y + 51, c['green'], 15, '#9fb0c8', True))
            continue
        line = COL['red'] if kind == 'x' else COL['yel']
        if c.get('conn'):
            d = 'M' + ' L'.join(f'{p[0]} {p[1]}' for p in c['conn'])
            o.append(f'<path d="{d}" fill="none" stroke="{line}" stroke-width="2"'
                     f'{" stroke-dasharray=\"6 5\"" if kind == "pehle" else ""}/>')
        if kind == 'x' and c.get('mark'):
            m = c['mark']
            for pt in (m if isinstance(m[0], list) else [m]):   # ek ya kai X
                o.append(xmark(*pt))
        bg = '#1a0f14' if kind == 'x' else '#221c08'
        o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="9" fill="{bg}" stroke="{line}" stroke-width="2"/>')
        o.append(f'<circle cx="{x+22}" cy="{y+22}" r="13" fill="{line}"/>')
        o.append(t(x + 22, y + 27, str(c['n']), 14, '#0b1220', True, anchor='middle'))
        o.append(t(x + 44, y + 27, c['red'], 15, line, True))
        o.append(t(x + 22, y + 51, c['green'], 15, COL['green'], True))
        if c.get('green2'):
            o.append(t(x + 22, y + 75, c['green2'], 15, COL['green'], True))
    o.append('</svg>')
    return ('<!doctype html>\n<meta charset="utf-8">\n<title>' + escape(spec['title'][:40]) + '</title>\n'
            '<style>body{margin:0;background:#0b1220} svg text{font-family:"Segoe UI",Arial,sans-serif} '
            '.m{font-family:Consolas,monospace}</style>\n' + '\n'.join(o) + '\n')


for f in sys.argv[1:]:
    spec = json.load(open(f, encoding='utf-8'))
    out = f.replace('.json', '.html')
    open(out, 'w', encoding='utf-8', newline='\n').write(build(spec))
    print(out)
