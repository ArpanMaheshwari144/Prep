"""Bani hui image me "kaise?" teer kisi dabbe / line ko kaatta to nahi? (render ke baad alag se jaanch)
Chalao: python 04_HLD/tools/check_arrows.py
"""
import re, glob, os
HERE = os.path.dirname(os.path.abspath(__file__))


def check(path):
    t = open(path, encoding='utf-8').read()
    pl = re.search(r'<polyline points="([^"]+)"[^>]*url\(#kaise\)', t)
    if not pl:
        return None
    pts = [tuple(map(float, p.split(','))) for p in pl.group(1).split()]
    # pehla andar wala svg = design: x, y, width, height + viewBox -> design naap me badlo
    m = re.search(r'<svg x="([\d.]+)" y="0" width="([\d.]+)" height="([\d.]+)"[^>]*viewBox="([\d.\-]+) ([\d.\-]+) ([\d.]+) ([\d.]+)"', t)
    if not m:
        m2 = re.search(r'<svg[^>]*viewBox="([\d.\-]+) ([\d.\-]+) ([\d.]+) ([\d.]+)"[^>]*x="([\d.]+)" y="0" width="([\d.]+)" height="([\d.]+)"', t)
        if not m2:
            return 'design svg nahi mila'
        vx, vy, vw, vh, ox, w, h = map(float, m2.groups())
    else:
        ox, w, h, vx, vy, vw, vh = map(float, m.groups())
    k = w / vw
    dp = [((x - ox) / k + vx, y / k + vy) for x, y in pts]
    dp = [p for p in dp if p[1] <= vy + vh]   # sirf design ke andar wala hissa
    rects = []
    for n in re.finditer(r'flowchart-(\w+)-\d+"[^>]*transform="translate\(([\d.\-]+), ([\d.\-]+)\)"><rect([^>]*)>', t):
        rw = re.search(r'width="([\d.]+)"', n.group(4)); rh = re.search(r'height="([\d.]+)"', n.group(4))
        if rw and rh:
            cx, cy, ww, hh = float(n.group(2)), float(n.group(3)), float(rw.group(1)), float(rh.group(1))
            rects.append((n.group(1), cx - ww / 2, cy - hh / 2, cx + ww / 2, cy + hh / 2))
    bad = []
    for (x1, y1), (x2, y2) in zip(dp, dp[1:]):
        for name, a, b, c, e in rects:
            inside_start = a <= x1 <= c and b <= y1 <= e   # teer isi dabbe se nikalta
            if inside_start:
                continue
            if min(x1, x2) < c and max(x1, x2) > a and min(y1, y2) < e and max(y1, y2) > b:
                bad.append(name)
    return ('dabba kaata: ' + ', '.join(sorted(set(bad)))) if bad else 'ok'


if __name__ == '__main__':
    n = ok = 0
    for p in sorted(glob.glob(os.path.join(HERE, '..', 'SYSTEM_DESIGNS', '*', 'img', '*.svg'))):
        r = check(p)
        if r is None:
            continue
        n += 1
        if r == 'ok':
            ok += 1
        else:
            print(os.path.relpath(p, os.path.join(HERE, '..')), '->', r)
    print(f'{ok} / {n} teer saaf (kisi dabbe ko nahi kaatte)')
