"""SCENE tasveer: card ki dikkat (PEHLE, laal) aur fix (AB, hara). Cheez dikhao, naam nahi.
Spec = img/cN.scene.json. render_diagrams.py design ke neeche jodta. Akela chalao:
    python scene.py <cN.scene.json> <out.svg>
Spec:
  {"title": "...", "rows": [{"tag": "pehle"|"ab", "label": "...", "items": [...], "result": "...", "foot": "..."}]}
  item = {"stack": [box, box, ...], "frame": "ek group"}  (upar-neeche dabbe, pichla teer har dabbe tak)
       | {"box": "naam", "sub": "line" | ["line", ...], "cells": ["s1", ...], "state": "ooge..",
          "pointer": [idx, "yahan tak padha (120)"], "clock": "5 min", "bad": true, "good": true}
       | {"arrow": "label", "dash": true}
  state ka har akshar ek khaana: o = naranji (RAM) · g = hara (disk / theek) · y = peela (agla / yahi)
                                 e = khaali · x = laal cross (roka) · b = neela · d = dhundhla
"""
import json, sys
from xml.sax.saxutils import escape as esc

W = 960
FONT = 'Arial, Helvetica, sans-serif'
COL = {'pehle': ('#e53935', '#fff5f5', '#c62828'), 'ab': ('#43a047', '#f3f9ee', '#2e7d32')}
CELL = {'o': ('#ffe0b2', '#ef6c00'), 'g': ('#c8e6c9', '#43a047'), 'y': ('#f9a825', '#333'),
        'e': ('#ffffff', '#bbb'), 'x': ('#ffebee', '#d32f2f'), 'b': ('#e3f2fd', '#1976d2'),
        'd': ('#eeeeee', '#bbb')}
CW, CH = 34, 26


def tw(t, fs=15):
    return len(t) * fs * 0.52


def subs(it):
    s = it.get('sub', [])
    return [s] if isinstance(s, str) else s


GAP = 10


def size(it):
    if 'stack' in it:   # kai dabbe upar-neeche (teen service, teen machine)
        ss = [size(b) for b in it['stack']]
        fr = 24 if it.get('frame') else 0
        return max(w for w, h in ss) + fr, sum(h for w, h in ss) + GAP * (len(ss) - 1) + (fr + 26 if fr else 0)
    if 'arrow' in it:
        return max(70, tw(it["arrow"], 13) + 30), 0
    lines = [it['box']] + subs(it)
    w = max([tw(l) for l in lines] + [len(it.get('cells', [])) * CW]) + 28
    if it.get('pointer'):
        w = max(w, tw(it['pointer'][1], 13) + 20)
    h = 22 + len(lines) * 19 + (CH + 12 if it.get('cells') else 0) + (34 if it.get('pointer') else 0) + 8
    return w, h


def draw_box(o, it, x, y, w, h):
    stroke = '#e53935' if it.get('bad') else '#43a047' if it.get('good') else '#555'
    o.append(f'<rect x="{x:.0f}" y="{y:.0f}" width="{w:.0f}" height="{h:.0f}" rx="8" fill="white" '
             f'stroke="{stroke}" stroke-width="2"/>')
    ty = y + 26
    for i, l in enumerate([it['box']] + subs(it)):
        b = ' font-weight="bold"' if i == 0 else ''
        o.append(f'<text x="{x+w/2:.0f}" y="{ty:.0f}" text-anchor="middle" font-size="15"{b}>{esc(l)}</text>')
        ty += 19
    cells = it.get('cells', [])
    if cells:
        st = it.get('state', 'g' * len(cells))
        cx0 = x + (w - len(cells) * CW) / 2
        cy = ty - 6
        for i, c in enumerate(cells):
            k = st[i] if i < len(st) else 'g'
            f, s = CELL[k]
            cx = cx0 + i * CW
            o.append(f'<rect x="{cx:.0f}" y="{cy:.0f}" width="{CW-3}" height="{CH}" fill="{f}" stroke="{s}"/>')
            o.append(f'<text x="{cx+(CW-3)/2:.0f}" y="{cy+18:.0f}" text-anchor="middle" font-size="12">'
                     f'{esc(c)}</text>')
            if k == 'x':
                o.append(f'<path d="M{cx+4:.0f},{cy+4:.0f} L{cx+CW-7:.0f},{cy+CH-4:.0f} M{cx+CW-7:.0f},{cy+4:.0f} '
                         f'L{cx+4:.0f},{cy+CH-4:.0f}" stroke="#d32f2f" stroke-width="2.5"/>')
        if it.get('pointer'):
            idx, lab = it['pointer']
            px = cx0 + idx * CW + (CW - 3) / 2
            o.append(f'<line x1="{px:.0f}" y1="{cy+CH+26:.0f}" x2="{px:.0f}" y2="{cy+CH+6:.0f}" stroke="#d32f2f" '
                     'stroke-width="2.5" marker-end="url(#sr)"/>')
            o.append(f'<text x="{x+w/2:.0f}" y="{cy+CH+42:.0f}" text-anchor="middle" font-size="13" '
                     f'fill="#d32f2f" font-weight="bold">{esc(lab)}</text>')
    if it.get('clock'):
        cx, cy = x + w - 4, y + 4
        o.append(f'<circle cx="{cx:.0f}" cy="{cy:.0f}" r="15" fill="white" stroke="#6a1b9a" stroke-width="2"/>')
        o.append(f'<path d="M{cx:.0f},{cy:.0f} L{cx:.0f},{cy-9:.0f} M{cx:.0f},{cy:.0f} L{cx+7:.0f},{cy:.0f}" '
                 'stroke="#6a1b9a" stroke-width="2"/>')
        o.append(f'<text x="{cx+20:.0f}" y="{cy+5:.0f}" font-size="13" fill="#6a1b9a" font-weight="bold">'
                 f'{esc(it["clock"])}</text>')


def draw_stack(o, it, x, y, w, h, dark):
    """stack ke har dabbe ka beech (y) lautao, teer wahan tak."""
    fr = 24 if it.get('frame') else 0
    if fr:
        o.append(f'<rect x="{x:.0f}" y="{y:.0f}" width="{w:.0f}" height="{h:.0f}" rx="10" fill="none" '
                 f'stroke="{dark}" stroke-width="2" stroke-dasharray="8,5"/>')
        o.append(f'<text x="{x+w/2:.0f}" y="{y+20:.0f}" text-anchor="middle" font-size="14" font-weight="bold" '
                 f'fill="{dark}">{esc(it["frame"])}</text>')
    cy, mids = y + (fr + 14 if fr else 0), []
    for b in it['stack']:
        bw, bh = size(b)
        draw_box(o, b, x + (w - bw) / 2, cy, bw, bh)
        mids.append(cy + bh / 2)
        cy += bh + GAP
    return mids


def render(spec):
    y = 50
    body = [f'<text x="24" y="34" font-size="21" font-weight="bold">{esc(spec["title"])}</text>']
    for row in spec['rows']:
        stroke, fill, dark = COL[row['tag']]
        sizes = [size(it) for it in row['items']]
        bh = max(h for w, h in sizes)
        used = sum(w for w, h in sizes)
        res = row.get('result', '')
        res_w = tw(res, 22) + 20 if res else 0
        res_side = 40 + used + 20 + res_w <= W - 30
        rh = 44 + bh + 20 + (34 if res and not res_side else 0) + (28 if row.get('foot') else 0)
        body.append(f'<rect x="20" y="{y}" width="{W-40}" height="{rh:.0f}" rx="10" fill="{fill}" '
                    f'stroke="{stroke}" stroke-width="2.5"/>')
        body.append(f'<text x="38" y="{y+28}" font-size="17" font-weight="bold" fill="{dark}">'
                    f'{esc(row["label"])}</text>')
        x, top = 40, y + 44
        xs = []
        for w, h in sizes:
            xs.append(x); x += w
        mids = {}
        for i, (it, (w, h)) in enumerate(zip(row['items'], sizes)):
            if 'stack' in it:
                mids[i] = draw_stack(body, it, xs[i], top + (bh - h) / 2, w, h, dark)
            elif 'box' in it:
                draw_box(body, it, xs[i], top + (bh - h) / 2, w, h)
        for i, (it, (w, h)) in enumerate(zip(row['items'], sizes)):
            if 'arrow' not in it:
                continue
            my = top + bh / 2
            d = ' stroke-dasharray="7,5"' if it.get('dash') else ''
            x0 = xs[i]
            for ty in (mids.get(i + 1) or [my]):   # agla stack = har dabbe tak teer
                body.append(f'<line x1="{x0+6:.0f}" y1="{my:.0f}" x2="{x0+w-8:.0f}" y2="{ty:.0f}" stroke="#444" '
                            f'stroke-width="2.5"{d} marker-end="url(#sa)"/>')
            if it['arrow']:
                body.append(f'<text x="{x0+w/2:.0f}" y="{my-10:.0f}" text-anchor="middle" font-size="13" '
                            f'fill="{dark}">{esc(it["arrow"])}</text>')
        if res:
            if res_side:
                body.append(f'<text x="{x+20:.0f}" y="{top+bh/2+8:.0f}" font-size="22" font-weight="bold" '
                            f'fill="{dark}">{esc(res)}</text>')
            else:
                body.append(f'<text x="{W-40:.0f}" y="{top+bh+32:.0f}" text-anchor="end" font-size="22" '
                            f'font-weight="bold" fill="{dark}">{esc(res)}</text>')
        if row.get('foot'):
            body.append(f'<text x="38" y="{y+rh-14:.0f}" font-size="15" fill="{dark}">{esc(row["foot"])}</text>')
        if 40 + used > W - 30:
            print('WARN: row bahut chauda', row['label'], round(40 + used), file=sys.stderr)
        y += rh + 18
    H = y + 2
    head = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H:.0f}" viewBox="0 0 {W} {H:.0f}" '
            f'font-family="{FONT}">',
            '<defs><marker id="sa" markerUnits="userSpaceOnUse" markerWidth="14" markerHeight="14" refX="12" refY="7" orient="auto">'
            '<path d="M0,0 L14,7 L0,14 z" fill="#444"/></marker>'
            '<marker id="sr" markerUnits="userSpaceOnUse" markerWidth="14" markerHeight="14" refX="12" refY="7" orient="auto">'
            '<path d="M0,0 L14,7 L0,14 z" fill="#d32f2f"/></marker></defs>',
            f'<rect width="{W}" height="{H:.0f}" fill="white"/>']
    return '\n'.join(head + body + ['</svg>'])


if __name__ == '__main__':
    spec = json.load(open(sys.argv[1], encoding='utf-8'))
    open(sys.argv[2], 'w', encoding='utf-8').write(render(spec))
    print('wrote', sys.argv[2])
