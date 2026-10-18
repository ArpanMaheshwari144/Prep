"""Concept SVG ke chhote helper (sab concept script yahi use karte)."""
import os
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'out')

def start(W, H, title):
    return [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
            'font-family="Arial, Helvetica, sans-serif">',
            '<defs><marker id="ca" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">'
            '<path d="M0,0 L10,5 L0,10 z" fill="#555"/></marker>'
            '<marker id="cr" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">'
            '<path d="M0,0 L10,5 L0,10 z" fill="#d32f2f"/></marker>'
            '<marker id="cg" markerWidth="10" markerHeight="10" refX="8" refY="5" orient="auto">'
            '<path d="M0,0 L10,5 L0,10 z" fill="#2e7d32"/></marker></defs>',
            f'<rect width="{W}" height="{H}" fill="white"/>',
            f'<text x="30" y="36" font-size="22" font-weight="bold">{title}</text>']

def box(o, x, y, w, h, text, fill='#e3f2fd', stroke='#1976d2', fs=16, bold=False, dash=False):
    d = ' stroke-dasharray="6,4"' if dash else ''
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="5" fill="{fill}" stroke="{stroke}" stroke-width="2"{d}/>')
    if text:
        b = ' font-weight="bold"' if bold else ''
        o.append(f'<text x="{x+w/2}" y="{y+h/2+fs/3}" text-anchor="middle" font-size="{fs}"{b}>{text}</text>')

def arrow(o, x1, y1, x2, y2, color='#555', dash=False, mark='ca'):
    d = ' stroke-dasharray="7,5"' if dash else ''
    o.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" stroke-width="2"{d} marker-end="url(#{mark})"/>')

def text(o, x, y, t, fs=15, color='#222', bold=False, anchor='start'):
    b = ' font-weight="bold"' if bold else ''
    o.append(f'<text x="{x}" y="{y}" font-size="{fs}" fill="{color}" text-anchor="{anchor}"{b}>{t}</text>')

def panel(o, x, y, lines):
    """lines: (text, heading?) ; '' = khaali line. Daayi taraf NIYAM + mod."""
    for t, h in lines:
        if t: text(o, x, y, t, 17 if h else 16, '#d32f2f' if h else '#222', h)
        y += 26

def save(o, name):
    o.append('</svg>')
    os.makedirs(OUT_DIR, exist_ok=True)
    p = os.path.join(OUT_DIR, name + '.svg')
    open(p, 'w', encoding='utf-8').write('\n'.join(o))
    print('wrote', p)
