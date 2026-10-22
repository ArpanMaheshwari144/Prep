"""Haath se bane poster (Docs 11) ko saaf saanche me: tasveer wahi, X + callout hata ke number-gol, neeche gen_clean ka hissa.
python 04_HLD/tools/docs_clean.py <cN_poster.html> [...]   (purana HTML git me rehta; ye usi se banata)"""
import html as H
import re
import sys

import gen_clean as G

CALL = {'#1a0f14': 'x', '#221c08': 'pehle', '#141c2b': 'grey'}
EL = re.compile(r'<g [^>]*>.*?</g>|<text [^>]*>.*?</text>|<(?:rect|line|path|circle)[^>]*/>', re.S)


def A(e, k):
    m = re.search(rf'\b{k}="([^"]*)"', e)
    return m.group(1) if m else None


def F(e, k):
    v = A(e, k)
    return float(v) if v is not None else None


def txt(e):
    return H.unescape(re.sub(r'<[^>]+>', '', e)).strip()


def ys_of(e):
    out = []
    for k in ('y', 'y1', 'y2', 'cy'):
        v = F(e, k)
        if v is not None:
            out.append(v)
    if e.startswith('<rect') and F(e, 'height') is not None and F(e, 'y') is not None:
        out.append(F(e, 'y') + F(e, 'height'))
    if e.startswith('<circle') and F(e, 'r'):
        out += [F(e, 'cy') - F(e, 'r'), F(e, 'cy') + F(e, 'r')]
    if e.startswith('<path'):
        out += [float(b) for a, b in re.findall(r'([\d.]+)[ ,]([\d.]+)', A(e, 'd') or '')]
    if e.startswith('<text') and F(e, 'y') is not None:
        out.append(F(e, 'y') - 14)
    return out


def ends(e):
    if e.startswith('<line'):
        return [(F(e, 'x1'), F(e, 'y1')), (F(e, 'x2'), F(e, 'y2'))]
    pts = [(float(a), float(b)) for a, b in re.findall(r'([\d.]+)[ ,]([\d.]+)', A(e, 'd') or '')]
    return [pts[0], pts[-1]] if pts else []


def on_rect(p, r):
    x, y, w, h = r
    return x - 4 <= p[0] <= x + w + 4 and (abs(p[1] - y) <= 4 or abs(p[1] - y - h) <= 4)


def run(f):
    s = open(f, encoding='utf-8').read()
    defs = re.search(r'<defs>.*?</defs>', s, re.S).group(0)
    body = s.split('<rect width="1500"', 1)[1].split('/>', 1)[1].rsplit('</svg>', 1)[0]
    body = re.sub(r'<!--.*?-->', '', body, flags=re.S)
    els = EL.findall(body)
    title = txt([e for e in els if e.startswith('<text') and F(e, 'y') == 40][0])
    els = [e for e in els if not (e.startswith('<text') and F(e, 'y') in (40, 68))]
    calls = []
    for e in els:
        if e.startswith('<rect') and A(e, 'fill') in CALL and A(e, 'rx') == '9':
            calls.append({'type': CALL[A(e, 'fill')], 'r': (F(e, 'x'), F(e, 'y'), F(e, 'width'), F(e, 'height')), 'el': e})
    used = set()
    for c in calls:
        used.add(c['el'])
        x, y, w, h = c['r']
        inside = [e for e in els if (e.startswith('<text') or e.startswith('<circle')) and F(e, 'y' if e.startswith('<text') else 'cy') is not None
                  and y < F(e, 'y' if e.startswith('<text') else 'cy') < y + h and x <= (F(e, 'x') or F(e, 'cx')) <= x + w]
        used.update(inside)
        tx = [e for e in inside if e.startswith('<text')]
        num = [txt(e) for e in tx if txt(e).isdigit()]
        c['n'] = int(num[0]) if num else 99
        rest = [e for e in tx if not txt(e).isdigit()]
        if c['type'] == 'grey':
            c['red'], c['green'] = txt(rest[0]), (txt(rest[1]) if len(rest) > 1 else '')
        else:
            c['red'] = txt(rest[0])
            gr = [re.sub(r'^bacha:\s*', '', txt(e)) for e in rest[1:]]
            c['green'] = gr[0] if gr else ''
            if len(gr) > 1:
                c['green2'] = ' '.join(gr[1:])
    xs = []
    for e in els:
        if e.startswith('<g') and '#ff5a6a' in e:
            ls = re.findall(r'<line [^>]*/>', e)
            pts = [(F(l, 'x1'), F(l, 'y1')) for l in ls] + [(F(l, 'x2'), F(l, 'y2')) for l in ls]
            xs.append((sum(p[0] for p in pts) / len(pts), sum(p[1] for p in pts) / len(pts)))
            used.add(e)
    for e in els:
        if e in used or not (e.startswith('<line') or e.startswith('<path')):
            continue
        if A(e, 'stroke') not in ('#ff5a6a', '#ffcc4d') or A(e, 'stroke-width') != '2':
            continue
        pe = ends(e)
        for c in calls:
            hit = [p for p in pe if on_rect(p, c['r'])]
            if hit:
                other = pe[1] if hit[0] == pe[0] else pe[0]
                near = [q for q in xs if abs(q[0] - other[0]) < 30 and abs(q[1] - other[1]) < 30]
                c.setdefault('mark', near[0] if near else other)
                used.add(e)
                break
    dia = [e for e in els if e not in used]
    ylist = [v for e in dia for v in ys_of(e)] + [c['mark'][1] - 14 for c in calls if c.get('mark') and c['type'] == 'x']
    ylist2 = [v for e in dia for v in ys_of(e)] + [c['mark'][1] + 14 for c in calls if c.get('mark') and c['type'] == 'x']
    toots = sorted([c for c in calls if c['type'] == 'x'], key=lambda c: c['n'])
    dikkat, pehle = G.split_pehle(sorted([c for c in calls if c['type'] == 'pehle'], key=lambda c: c['n']))
    greys = [c for c in calls if c['type'] == 'grey']
    o = [G.t(30, 40, title, 26, '#e6edf3', True), G.t(30, 68, G.SUB, 15, '#9fb0c8')]
    dy = G.pehle_box(o, dikkat) - min(ylist)
    dia_end = max(ylist2) + dy + 6
    o += [f'<g transform="translate(0,{dy:g})">'] + dia + ['</g>']
    for i, c in enumerate(toots, 1):
        if c.get('mark'):
            px, py = c['mark'][0], c['mark'][1] + dy
            o.append(f'<circle cx="{px:g}" cy="{py:g}" r="14" fill="#1a0f14" stroke="{G.RED}" stroke-width="2.5"/>')
            o.append(G.t(px, py + 5, str(i), 14, G.RED, True, anchor='middle'))
    y = G.bottom(o, dia_end + G.GAP, toots, pehle, greys)
    Ht = int(y + 30)
    out = ('<!doctype html>\n<meta charset="utf-8">\n<title>' + H.escape(title[:40]) + '</title>\n'
           '<style>body{margin:0;background:#0b1220} svg text{font-family:"Segoe UI",Arial,sans-serif} '
           '.m{font-family:Consolas,monospace}</style>\n'
           f'<svg width="1500" height="{Ht}" viewBox="0 0 1500 {Ht}" xmlns="http://www.w3.org/2000/svg">{defs}'
           f'<rect width="1500" height="{Ht}" fill="#0b1220"/>\n' + '\n'.join(o) + '\n</svg>\n')
    open(f, 'w', encoding='utf-8', newline='\n').write(out)
    print(f, 'toot', len(toots), 'pehle tha', [c['red'] for c in dikkat], 'nahi', [c['red'] for c in pehle], 'grey', len(greys), 'bina mark', [c['n'] for c in toots if not c.get('mark')])


if __name__ == '__main__':
    for f in sys.argv[1:]:
        run(f)
