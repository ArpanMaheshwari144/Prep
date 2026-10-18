"""HLD design file ke har mermaid diagram ko SVG image banata (mermaid-cli).
Chalao:  python 04_HLD/tools/render_diagrams.py 04_HLD/SYSTEM_DESIGNS/13_banking_system/13_banking_system.md
- Source = img/<naam>.mmd (md me sirf ![naam](img/naam.svg)). Diagram badalna: .mmd badlo, phir yahi chalao.
- Purani file (```mermaid / <details> wali): pehli baar chalane pe source .mmd me chala jaata, md me sirf image.
- Image commit: git add <folder>/img/*.svg
"""
import re, subprocess, sys, os
HERE = os.path.dirname(os.path.abspath(__file__))
CFG = os.path.join(HERE, 'mermaid_cfg.json')

def render(src, png, wide=1100):
    """mermaid -> svg. Design LR me {wide} se chauda bane to TB (upar se neeche) me dobara, source nahi badalta."""
    src = '\n'.join(l for l in src.split('\n') if 'n_pad' not in l)
    tmp = png + '.tmp.mmd'   # har image ki apni temp file (saanjhi file = parallel me gadbad)
    open(tmp, 'w', encoding='utf-8').write(src)
    r = subprocess.run(['npx', '-y', '@mermaid-js/mermaid-cli@11.4.2', '-i', tmp, '-o', png, '-c', CFG,
                        '-b', 'white', '-w', '1100'], capture_output=True, text=True, shell=True)
    os.remove(tmp)
    if r.returncode != 0:
        return False, r.stderr[-300:]
    vb = re.search(r'viewBox="[\d.\-]+ [\d.\-]+ ([\d.]+)', open(png, encoding='utf-8').read())
    if vb and float(vb.group(1)) > wide and 'flowchart LR' in src:
        return render(src.replace('flowchart LR', 'flowchart TB', 1), png, wide)
    fix_size(png)
    return True, ''

CAP = 900   # GitHub pe chauda diagram chhota dikhe; click karo to poora

def fix_size(svg_path):
    """width = min(viewBox chaudai, CAP), height usi anupaat me; viewBox wahi (poora diagram dikhe)."""
    t = open(svg_path, encoding='utf-8').read()
    tag = re.search(r'<svg[^>]*>', t).group(0)
    vb = re.search(r'viewBox="[\d.\-]+ [\d.\-]+ ([\d.]+) ([\d.]+)"', tag)
    if not vb: return
    w, h = float(vb.group(1)), float(vb.group(2))
    k = min(1.0, CAP / w)
    new = re.sub(r' (width|height)="[^"]*"', '', tag)
    new = re.sub(r'max-width:\s*[\d.]+px;\s*', '', new)
    new = new.replace('<svg ', f'<svg width="{round(w*k)}" height="{round(h*k)}" ', 1)
    open(svg_path, 'w', encoding='utf-8').write(t.replace(tag, new, 1))

CONCEPT = os.path.join(HERE, 'concept')

def merge_concept(svg_path, mmd_src, term, csvg=None):
    """Design SVG ke neeche concept SVG jodo, peele dabbe se dashed teer "kaise?"."""
    d = open(svg_path, encoding='utf-8').read()
    c = csvg or open(os.path.join(CONCEPT, 'out', term + '.svg'), encoding='utf-8').read()
    def size(t):
        tag = re.search(r'<svg[^>]*>', t).group(0)
        vb = re.search(r'viewBox="([\d.\-]+) ([\d.\-]+) ([\d.]+) ([\d.]+)"', tag)
        return tag, [float(x) for x in vb.groups()]
    dtag, (dvx, dvy, w1, h1) = size(d)
    ctag, (_, _, w2, h2) = size(c)
    k = max(0.8, min(1.0, w2 / w1))   # design 0.8 se neeche nahi; design chauda ho to usi chaudai pe chhota, warna neeche wali tasveer bahut chhoti dikhti
    w1, h1, W0, H0 = w1 * k, h1 * k, w1, h1
    W = max(w1, w2); gap = 70; H = h1 + gap + h2
    ox1, ox2 = (W - w1) / 2, (W - w2) / 2
    d2 = d.replace(dtag, re.sub(r' (width|height)="[^"]*"', '', dtag).replace('<svg ', f'<svg x="{ox1:.1f}" y="0" width="{w1:.1f}" height="{h1:.1f}" ', 1), 1)
    c2 = c.replace(ctag, re.sub(r' (width|height)="[^"]*"', '', ctag).replace('<svg ', f'<svg x="{ox2:.1f}" y="{h1+gap:.1f}" width="{w2:.1f}" height="{h2:.1f}" ', 1), 1)
    # kai peele ho to wo jiske label me text (<br/>) hai
    ys = re.findall(r'style (\w+) fill:#f9a825', mmd_src)
    lab = lambda n: (re.search(r'\b' + n + r'\["([^"]*)"', mmd_src) or re.match('()', '')).group(1)
    node = max(ys, key=lambda n: lab(n).count('<br/>'))   # sabse lamba peela = is card ka
    # rukawat: sab dabbe (x1, y1, x2, y2) + sab lines (tukde); teer inme se kisi ke upar se na jaaye
    boxes = {}
    for m in re.finditer(r'id="[^"]*flowchart-(\w+)-\d+"[^>]*transform="translate\(([\d.\-]+), ([\d.\-]+)\)"><rect([^>]*)>', d):
        rw = re.search(r'width="([\d.]+)"', m.group(4)); rh = re.search(r'height="([\d.]+)"', m.group(4))
        if rw and rh:
            cx, cy, w, h = float(m.group(2)), float(m.group(3)), float(rw.group(1)), float(rh.group(1))
            boxes[m.group(1)] = (cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2)
    segs = []
    for m in re.finditer(r'<path d="(M[^"]*)"[^>]*flowchart-link', d):
        pts = [(float(a), float(b)) for a, b in re.findall(r'([\d.\-]+),([\d.\-]+)', m.group(1))]
        segs += list(zip(pts, pts[1:]))
    bottom = dvy + h1 / k   # design ka neeche ka kinara (design ke apne naap me)
    M = 10

    def hits(p, q, own, lines=True):
        """seedha (khada / pada) tukda p->q kisi dabbe ya line se takraaye?"""
        (x1, y1), (x2, y2) = p, q
        lo_x, hi_x, lo_y, hi_y = min(x1, x2), max(x1, x2), min(y1, y2), max(y1, y2)
        for n, (a, b, c, e) in boxes.items():
            if n == own:
                continue
            if lo_x < c + M and hi_x > a - M and lo_y < e + M and hi_y > b - M:
                return True
        for (sx1, sy1), (sx2, sy2) in (segs if lines else []):
            if x1 == x2:   # khada
                if (sx1 - x1) * (sx2 - x1) <= 0 and sx1 != sx2:
                    yy = sy1 + (sy2 - sy1) * (x1 - sx1) / (sx2 - sx1)
                    if lo_y + 2 < yy < hi_y:
                        return True
            else:          # pada
                if (sy1 - y1) * (sy2 - y1) <= 0 and sy1 != sy2:
                    xx = sx1 + (sx2 - sx1) * (y1 - sy1) / (sy2 - sy1)
                    if lo_x < xx < hi_x:
                        return True
        return False

    xmin, xmax = dvx + 12, dvx + W0 - 12   # teer image ke andar hi

    def route(nd, lines):
        a, b, c, e = boxes[nd]
        mid = (a + c) / 2
        # 1) seedha neeche, dabbe ke andar kahin se
        for x in sorted([a + 16 + i * 8 for i in range(int((c - a - 32) / 8) + 1)], key=lambda x: abs(x - mid)):
            if not hits((x, e), (x, bottom), nd, lines):
                return [(x, e), (x, bottom)]
        # 2) elbow: dabbe ke left / right kinare se bahar, phir neeche
        ym = (b + e) / 2
        for dx in range(24, 600, 12):
            for side, x0 in ((1, c), (-1, a)):
                x = x0 + side * dx
                if not xmin < x < xmax:
                    continue
                if not hits((x0, ym), (x, ym), nd, lines) and not hits((x, ym), (x, bottom), nd, lines):
                    return [(x0, ym), (x, ym), (x, bottom)]
        return None

    def ncross(path):
        c = 0
        for p, q in zip(path, path[1:]):
            for sg in segs:
                (x1, y1), (x2, y2) = p, q
                (sx1, sy1), (sx2, sy2) = sg
                if x1 == x2 and (sx1 - x1) * (sx2 - x1) <= 0 and sx1 != sx2:
                    yy = sy1 + (sy2 - sy1) * (x1 - sx1) / (sx2 - sx1)
                    c += min(y1, y2) + 2 < yy < max(y1, y2)
                elif y1 == y2 and (sy1 - y1) * (sy2 - y1) <= 0 and sy1 != sy2:
                    xx = sx1 + (sx2 - sx1) * (y1 - sy1) / (sy2 - sy1)
                    c += min(x1, x2) < xx < max(x1, x2)
        return c

    def all_routes(nd):
        a, b, c, e = boxes[nd]
        ym = (b + e) / 2
        out = [[(x, e), (x, bottom)] for x in [a + 16 + i * 8 for i in range(int((c - a - 32) / 8) + 1)]
               if not hits((x, e), (x, bottom), nd, False)]
        for dx in range(24, 600, 12):
            for side, x0 in ((1, c), (-1, a)):
                x = x0 + side * dx
                if xmin < x < xmax and not hits((x0, ym), (x, ym), nd, False) and not hits((x, ym), (x, bottom), nd, False):
                    out.append([(x0, ym), (x, ym), (x, bottom)])
        return out

    ordered = [node] + sorted((n for n in ys if n in boxes and n != node), key=lambda n: -boxes[n][3])
    path = route(node, True) if node in boxes else None   # card ka apna peela dabba, na dabba na line
    if not path and node in boxes:   # line sabse kam, par isi dabbe se
        cands = [(ncross(r), len(r), r) for r in all_routes(node)]
        if cands:
            nc, _, path = min(cands, key=lambda t: (t[0], t[1]))
            print('NOTE: teer', nc, 'line kaatta (dabba nahi)', os.path.basename(svg_path))
    if not path:   # isi dabbe se kuch nahi -> doosra peela
        for nd in ordered[1:]:
            if nd in boxes:
                path = route(nd, False)
                if path:
                    break
    if not path:
        print('WARN: kaise teer ka khaali raasta nahi mila', svg_path)
        a, b, c, e = boxes[node]
        path = [((a + c) / 2, e), ((a + c) / 2, bottom)]
    pts = [((x - dvx) * k + ox1, (y - dvy) * k) for x, y in path]
    pts[0] = (pts[0][0], pts[0][1] + (4 if len(path) == 2 else 0))
    pts.append((pts[-1][0], h1 + gap - 4))
    nx = pts[-1][0]
    poly = ' '.join(f'{x:.1f},{y:.1f}' for x, y in pts)
    arrow = (f'<defs><marker id="kaise" markerUnits="userSpaceOnUse" markerWidth="16" markerHeight="16" refX="12" refY="8" orient="auto">'
             f'<path d="M0,0 L16,8 L0,16 z" fill="#f9a825"/></marker></defs>'
             f'<polyline points="{poly}" fill="none" stroke="#f9a825" stroke-width="3" '
             f'stroke-dasharray="8,5" marker-end="url(#kaise)"/>'
             f'<text x="{(nx + 10) if nx < W - 90 else (nx - 10):.1f}" text-anchor="{'start' if nx < W - 90 else 'end'}" '
             f'y="{h1+gap/2+5:.1f}" font-family="Arial, Helvetica, sans-serif" font-size="16" '
             f'font-weight="bold" fill="#e65100">kaise?</text>')
    P = 24   # chaaron taraf margin
    out = (f'<svg xmlns="http://www.w3.org/2000/svg" width="{W+2*P:.0f}" height="{H+2*P:.0f}" '
           f'viewBox="0 0 {W+2*P:.1f} {H+2*P:.1f}">'
           f'<rect width="100%" height="100%" fill="white"/><g transform="translate({P},{P})">' + d2 + c2 + arrow + '</g></svg>')
    open(svg_path, 'w', encoding='utf-8').write(out)
    fix_size(svg_path)

def concept_for(md_path, name):
    import json
    mp = os.path.join(CONCEPT, 'map.json')
    if not os.path.exists(mp): return None
    key = os.path.basename(os.path.dirname(md_path)) + '/' + name
    return json.load(open(mp, encoding='utf-8')).get(key)

def main(f):
    d = os.path.dirname(f); img = os.path.join(d, 'img'); os.makedirs(img, exist_ok=True)
    s = open(f, encoding='utf-8').read()
    if '```mermaid' not in s:
        # file naam me hash (c1-<hash>.svg): GitHub ?v= nahi maanta, naya naam = nayi image turant
        import hashlib, glob, shutil
        link = re.compile(r'!\[(\w+)\]\(img/\1(?:-[0-9a-f]{8})?\.svg(?:\?v=\w+)?\)')
        def one(m):
            name = m.group(1)
            mmd = os.path.join(img, name + '.mmd')
            if not os.path.exists(mmd):
                term = concept_for(f, name)   # sirf concept / scene, design nahi (jaise SHURU card)
                scene = os.path.join(img, name + '.scene.json')
                tmp = os.path.join(img, name + '.svg')
                if term:
                    shutil.copy(os.path.join(CONCEPT, 'out', term + '.svg'), tmp)
                elif os.path.exists(scene):
                    sys.path.insert(0, HERE); import scene as sc, json
                    open(tmp, 'w', encoding='utf-8').write(sc.render(json.load(open(scene, encoding='utf-8'))))
                    term = 'scene'
                else:
                    return m.group(0)
                fix_size(tmp)
                print('ok', name, '= ' + term)
                return save(name, tmp)
            src = open(mmd, encoding='utf-8').read()
            tmp = os.path.join(img, name + '.svg')
            ok, err = render(src, tmp)
            term = concept_for(f, name) if ok else None
            scene = os.path.join(img, name + '.scene.json')
            if term: merge_concept(tmp, src, term)
            elif ok and os.path.exists(scene):   # scene tasveer: PEHLE / AB
                sys.path.insert(0, HERE); import scene as sc, json
                merge_concept(tmp, src, None, sc.render(json.load(open(scene, encoding='utf-8'))))
                term = 'scene'
            print('ok' if ok else 'FAIL', name, ('+ ' + term) if term else '', '' if ok else err)
            if not ok: return m.group(0)
            return save(name, tmp)
        def save(name, tmp):
            h = hashlib.md5(open(tmp, 'rb').read()).hexdigest()[:8]
            for old in glob.glob(os.path.join(img, name + '-*.svg')):
                if re.fullmatch(re.escape(name) + r'-[0-9a-f]{8}\.svg', os.path.basename(old)): os.remove(old)
            os.replace(tmp, os.path.join(img, f'{name}-{h}.svg'))
            return f'![{name}](img/{name}-{h}.svg)'
        out = link.sub(one, s)   # pehle saari images, phir file (beech me toota to purani file waisi hi)
        if len(out) < len(s) * 0.9:
            raise SystemExit('ROKA: nayi file purani se bahut chhoti ' + f)
        tmp_md = f + '.tmp'
        open(tmp_md, 'w', encoding='utf-8').write(out)
        os.replace(tmp_md, f)
        return
    # already converted blocks: ![name](img/name.png) + details + mermaid
    conv = re.compile(r'!\[(\w+)\]\(img/\1\.svg\)\n\n<details><summary>diagram source \(mermaid\)</summary>\n\n'
                      r'```mermaid\n(.*?)```\n\n</details>\n', re.S)
    if conv.search(s):
        for m in conv.finditer(s):
            ok, err = render(m.group(2), os.path.join(img, m.group(1) + '.svg'))
            print('ok' if ok else 'FAIL', m.group(1), '' if ok else err)
        return
    parts = re.split(r'(```mermaid\n.*?```)', s, flags=re.S)
    out = []; heading = 'top'; cnt = {}
    for p in parts:
        if p.startswith('```mermaid'):
            cnt[heading] = cnt.get(heading, 0) + 1
            name = heading + (f'_{cnt[heading]}' if cnt[heading] > 1 else '')
            ok, err = render(p[len('```mermaid\n'):-3], os.path.join(img, name + '.svg'))
            if not ok:
                print('FAIL', name, err); out.append(p); continue
            out.append(f'![{name}](img/{name}.svg)\n\n<details><summary>diagram source (mermaid)</summary>\n\n'
                       + p + '\n\n</details>\n')
            print('ok', name)
        else:
            hs = re.findall(r'^#### (\S+)', p, re.M)
            if hs: heading = 'c' + re.sub(r'\W', '', hs[-1]).lower()
            out.append(p)
    open(f, 'w', encoding='utf-8').write(''.join(out))

if __name__ == '__main__':
    if sys.argv[1:2] == ['--fix-size']:
        import glob
        for p in glob.glob(os.path.join(HERE, '..', 'SYSTEM_DESIGNS', '*', 'img', '*.svg')): fix_size(p)
    else:
        for f in sys.argv[1:]: main(f)
