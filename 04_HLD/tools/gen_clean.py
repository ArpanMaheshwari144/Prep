"""Saaf poster (saancha: Fable c1_clean, 22-Oct): wahi cN_poster.json, naya roop.
python 04_HLD/tools/gen_clean.py <cN_poster.json> [...]  -> cN_poster.html (phir node poster2png.js)

Upar tasveer: sirf dabbe + teer. Tasveer ke neeche hara "Kyun chalta" (json me kyun). Toot ki jagah laal number-gol (X / line nahi).
Tasveer ke 60 px neeche ek row me toot-dabbe (max 4, 345 x 120, beech 17 px): laal title = kya hua, hara = kaise bacha.
Peele wale do tarah: "Pehle ... tha" (bina ?) = upar chhota laal-dhundhla dabba "Pehle aisa tha" (tasveer se pehle);
sawaal wale = peela dabba "Doosra tareeka kyun nahi?". Phir grey "baaki" (grey + 4 se zyada toot).
Text 13.5-15 px, 44 akshar pe wrap.
"""
import json
import re
import sys
import textwrap
from html import escape

COL = {'blue': '#4aa8ff', 'green': '#3ddc97', 'purple': '#a98bff', 'grey': '#6b7a90', 'orange': '#ffa94d'}
BOX = {'user': ('#10243f', '#4aa8ff'), 'server': ('#0f2a22', '#3ddc97'), 'cache': ('#2a1a10', '#ffa94d'),
       'db': ('#1d1838', '#a98bff'), 'lb': ('#152033', '#9fb0c8'), 'plain': ('#152033', '#4aa8ff')}
RED, GRN, YEL = '#ff5a6a', '#7fe0b0', '#ffcc4d'
SUB = 'Laal number = yahan toot sakta hai, neeche usi number pe: laal = kya hua, hara = kaise bacha'
W, TOP, GAP = 1500, 100, 60
TW, TH, TG = 345, 120, 17


def t(x, y, s, size, fill, bold=False, mono=False, anchor='start'):
    return (f'<text x="{x}" y="{y}" font-size="{size}"{" font-weight=\"700\"" if bold else ""} fill="{fill}"'
            f'{" class=\"m\"" if mono else ""}{"" if anchor == "start" else f" text-anchor=\"{anchor}\""}>{escape(s)}</text>')


def J(*parts):
    """hare hisse jodo, beech me full-stop."""
    ps = [x.strip() for x in parts if x and x.strip()]
    out = ''
    for x in ps:
        cont = out and (out[-1] in '.?!:,;' or x[0].islower() or not x[0].isalpha())
        out = x if not out else out + ('' if cont else '.') + ' ' + x
    return out


def wrap(s, n=44):
    return textwrap.wrap(s, n) or ['']


def split_pehle(pehle):
    """peele wale do tarah ke: 'Pehle ... tha' (bina ?) = DIKKAT, upar alag dabba; sawaal (?) = NAHI, peela."""
    dikkat = [c for c in pehle if '?' not in c['red']]
    return dikkat, [c for c in pehle if c not in dikkat]


def bina_pehle(s):
    """label 'Pehle aisa tha' hai, to line ke shuru ka 'Pehle' / 'pehle:' hatao ('Pehle padho, phir' wala kram rehne do)."""
    if re.match(r'(?i)pehle:?\s', s) and 'phir' not in s.split(':')[0]:
        s = re.sub(r'(?i)^pehle:?\s+', '', s)
    return s[0].upper() + s[1:]


def bada(s):
    return s[:1].upper() + s[1:]


def kahani(o, spec):
    """Card aapas me jude (22-Oct, Arpan): kone me 'card N / total', upar patti 'Peeche: .. · Ab: ..'. Lautata: agla y."""
    y = 84
    if spec.get('step'):
        n, tot = spec['step']
        o.append(f'<rect x="1330" y="50" width="140" height="28" rx="14" fill="#2a1a10" stroke="#ffa94d" stroke-width="2"/>')
        o.append(t(1400, 69, f'card {n} / {tot}', 14, '#ffa94d', True, anchor='middle'))
    if spec.get('peeche') or spec.get('ab'):
        o.append(f'<rect x="30" y="{y}" width="1440" height="46" rx="9" fill="#111a2a" stroke="#33425a" stroke-width="2"/>')
        o.append(t(48, y + 29, 'Peeche:', 15, '#9fb0c8', True))
        o.append(t(118, y + 29, spec.get('peeche', ''), 14.5, '#c9d6e8'))
        o.append(t(760, y + 29, 'Ab:', 15, '#ff8a95', True))
        o.append(t(796, y + 29, spec.get('ab', ''), 14.5, '#f0d4d8'))
        y += 46 + 14
    return y


def agla(o, y, spec):
    """Neeche peeli patti: AGLA > agla card kya tootega."""
    if not spec.get('agla'):
        return y
    y += 22
    o.append(f'<rect x="30" y="{y}" width="1440" height="44" rx="9" fill="#221c08" stroke="#ffcc4d" stroke-width="2"/>')
    o.append(t(48, y + 28, 'AGLA >', 15, '#ffcc4d', True))
    o.append(t(118, y + 28, spec['agla'], 14.5, '#e6dcb8'))
    return y + 44


def pehle_box(o, dikkat, y=84):
    """Tasveer se pehle ek chhota laal-dhundhla dabba 'Pehle aisa tha'. Lautata: tasveer kahan se shuru."""
    if not dikkat:
        return y + 16
    # 'tha' = sirf dikkat wala hara hissa (ilaaj 'Ab ...' yahan nahi, wo Kyun chalta me)
    txt = '   ·   '.join(J(bina_pehle(c['red']), *((bada(c['tha']),) if 'tha' in c else
                                                     (bada(c.get('green', '')), c.get('green2', ''))))
                           for c in dikkat)
    lines = textwrap.wrap(txt, 165)
    h = 16 + 20 * len(lines) + 8
    o.append(f'<rect x="30" y="{y}" width="1440" height="{h}" rx="9" fill="#1f1418" stroke="#8a4a55" stroke-width="2"/>')
    o.append(t(48, y + 26, 'Pehle aisa tha:', 14, '#ff8a95', True))
    for k, ln in enumerate(lines):
        o.append(t(170, y + 26 + 20 * k, ln, 13.5, '#d9b8bd'))
    return y + h + 34


def bottom(o, y, toots, pehle, greys, kyun=''):
    """Neeche ka hissa: hara 'Kyun chalta', toot-dabbe (max 4), peela, grey. Docs wala script bhi yahi chalata."""
    # hara: kyun chalta (tasveer ke turant neeche)
    if kyun:
        lines = textwrap.wrap(kyun, 160)
        h = 16 + 20 * len(lines) + 8
        o.append(f'<rect x="30" y="{y}" width="1440" height="{h}" rx="9" fill="#0f2a22" stroke="#3ddc97" stroke-width="2"/>')
        o.append(t(48, y + 26, 'Kyun chalta:', 14, '#3ddc97', True))
        for k, ln in enumerate(lines):
            o.append(t(160, y + 26 + 20 * k, ln, 13.5, '#cfeee0'))
        y += h + 30
    # toot-dabbe
    row = toots[:4]
    if row:
        bodies = [wrap(J(c.get('green', ''), c.get('green2', ''))) for c in row]
        heads = [wrap(c['red'], 38) for c in row]
        h = max(TH, max(32 + 21 * len(hd) + 20 * len(bd) + 14 for hd, bd in zip(heads, bodies)))
        for i, (c, hd, bd) in enumerate(zip(row, heads, bodies)):
            x = 30 + i * (TW + TG)
            o.append(f'<rect x="{x}" y="{y}" width="{TW}" height="{h}" rx="9" fill="#1a0f14" stroke="{RED}" stroke-width="2"/>')
            o.append(f'<circle cx="{x + 22}" cy="{y + 25}" r="12" fill="{RED}"/>')
            o.append(t(x + 22, y + 30, str(i + 1), 13, '#0b1220', True, anchor='middle'))
            for k, ln in enumerate(hd):
                o.append(t(x + 44, y + 30 + 21 * k, ln, 15, RED, True))
            by = y + 30 + 21 * len(hd) + 6
            for k, ln in enumerate(bd):
                o.append(t(x + 22, by + 20 * k, ln, 13.5, GRN))
        y += h + 35
    # peela: doosra tareeka kyun nahi
    if pehle:
        txt = '   ·   '.join(f'{c["red"].rstrip("?")}: {J(c.get('green', ''), c.get('green2', ''))}' for c in pehle)
        lines = textwrap.wrap(txt, 190)
        h = 38 + 20 * len(lines) + 8
        o.append(f'<rect x="30" y="{y}" width="1440" height="{h}" rx="9" fill="#221c08" stroke="{YEL}" stroke-width="2"/>')
        o.append(t(48, y + 26, 'Doosra tareeka kyun nahi?', 14, YEL, True))
        for k, ln in enumerate(lines):
            o.append(t(48, y + 50 + 20 * k, ln, 13.5, '#e6dcb8'))
        y += h + 22
    # grey: baaki
    rest = [x for c in greys for x in (c['red'], c['green'])]
    rest += [f'{i}. {c["red"]}: {J(c.get('green', ''), c.get('green2', ''))}'
             for i, c in enumerate(toots[4:], 5)]
    if rest:
        lines = [ln for r in rest for ln in textwrap.wrap(r, 190)]
        h = 22 + 22 * len(lines) + 6
        o.append(f'<rect x="30" y="{y}" width="1440" height="{h}" rx="9" fill="#141c2b" stroke="#3a4a63" stroke-width="2"/>')
        for k, ln in enumerate(lines):
            o.append(t(48, y + 30 + 22 * k, ln, 13.5, '#c9d6e8'))
        y += h
    return y


def build(spec):
    toots = [c for c in spec['callouts'] if c.get('type', 'x') == 'x']
    dikkat, pehle = split_pehle([c for c in spec['callouts'] if c.get('type') == 'pehle'])
    greys = [c for c in spec['callouts'] if c.get('type') == 'grey']
    # tasveer ki seema
    ys = [b['y'] for b in spec['boxes']] + [r['cy'] - r['r'] for r in spec.get('circles', [])]
    ye = [b['y'] + b['h'] for b in spec['boxes']] + [r['cy'] + r['r'] for r in spec.get('circles', [])]
    for a in spec['arrows']:
        ys += [p[1] for p in a['pts']]; ye += [p[1] for p in a['pts']]
        if a.get('label'):
            ye.append(a['ly'] + 4)
    for c in toots:
        m = c.get('mark')
        if m:
            for p in (m if isinstance(m[0], list) else [m]):
                ys.append(p[1] - 14); ye.append(p[1] + 14)
    o = []
    o.append(t(30, 40, spec['title'], 26, '#e6edf3', True))
    o.append(t(30, 68, SUB, 15, '#9fb0c8'))
    top = pehle_box(o, dikkat, kahani(o, spec))
    dy = top - min(ys)
    dia_end = max(ye) + dy

    def Y(y):
        return y + dy

    for r in spec.get('circles', []):
        o.append(f'<circle cx="{r["cx"]}" cy="{Y(r["cy"])}" r="{r["r"]}" fill="none" stroke="#2a3a5a" stroke-width="3" stroke-dasharray="6 6"/>')
        if r.get('label'):
            o.append(t(r['cx'], Y(r['cy']) + 5, r['label'], 15, '#6b7a90', mono=True, anchor='middle'))
    for b in spec['boxes']:
        fill, st = BOX[b['kind']]
        x, y, w, h = b['x'], Y(b['y']), b['w'], b['h']
        o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="12" fill="{fill}" stroke="{st}" stroke-width="2.5"/>')
        o.append(t(x + w / 2, y + 34, b['title'], 19, '#e6edf3', True, anchor='middle'))
        for i, ln in enumerate(b.get('lines', [])):
            o.append(t(x + w / 2, y + 60 + i * 22, ln, 13, '#c9d6e8', mono=True, anchor='middle'))
    for a in spec['arrows']:
        c = a.get('c', 'blue')
        d = 'M' + ' L'.join(f'{p[0]} {Y(p[1])}' for p in a['pts'])
        o.append(f'<path d="{d}" fill="none" stroke="{COL[c]}" stroke-width="3"'
                 f'{" stroke-dasharray=\"8 6\"" if a.get("dash") else ""} marker-end="url(#a{c})"/>')
        if a.get('label'):
            o.append(t(a['lx'], Y(a['ly']), a['label'], 13, COL[c], mono=True, anchor=a.get('a', 'start')))
    # number-gol, toot ke kram se
    for i, c in enumerate(toots, 1):
        m = c.get('mark')
        if not m:
            continue
        for p in (m if isinstance(m[0], list) else [m]):
            o.append(f'<circle cx="{p[0]}" cy="{Y(p[1])}" r="14" fill="#1a0f14" stroke="{RED}" stroke-width="2.5"/>')
            o.append(t(p[0], Y(p[1]) + 5, str(i), 14, RED, True, anchor='middle'))
    y = bottom(o, dia_end + GAP, toots, pehle, greys, spec.get('kyun', ''))
    y = agla(o, y, spec)
    H = int(y + 30)
    head = [f'<svg width="{W}" height="{H}" viewBox="0 0 {W} {H}" xmlns="http://www.w3.org/2000/svg"><defs>']
    for k in COL:
        head.append(f'<marker id="a{k}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
                    f'<path d="M0,0 L10,5 L0,10 Z" fill="{COL[k]}"/></marker>')
    head.append(f'</defs><rect width="{W}" height="{H}" fill="#0b1220"/>')
    return ('<!doctype html>\n<meta charset="utf-8">\n<title>' + escape(spec['title'][:40]) + '</title>\n'
            '<style>body{margin:0;background:#0b1220} svg text{font-family:"Segoe UI",Arial,sans-serif} '
            '.m{font-family:Consolas,monospace}</style>\n' + '\n'.join(head + o) + '\n</svg>\n')


if __name__ == '__main__':
    for f in sys.argv[1:]:
        spec = json.load(open(f, encoding='utf-8'))
        out = f.replace('.json', '.html')
        open(out, 'w', encoding='utf-8', newline='\n').write(build(spec))
        print(out)
