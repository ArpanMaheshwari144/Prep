"""HLD design file ke har mermaid diagram ko SVG image banata (mermaid-cli).
Chalao:  python 04_HLD/tools/render_diagrams.py 04_HLD/SYSTEM_DESIGNS/13_banking_system/13_banking_system.md
- Source = img/<naam>.mmd (md me sirf ![naam](img/naam.svg)). Diagram badalna: .mmd badlo, phir yahi chalao.
- Purani file (```mermaid / <details> wali): pehli baar chalane pe source .mmd me chala jaata, md me sirf image.
- Image commit: git add <folder>/img/*.svg
"""
import re, subprocess, sys, os
HERE = os.path.dirname(os.path.abspath(__file__))
CFG = os.path.join(HERE, 'mermaid_cfg.json')

def render(src, png):
    tmp = os.path.join(HERE, '_tmp.mmd')
    src = '\n'.join(l for l in src.split('\n') if 'n_pad' not in l)
    open(tmp, 'w', encoding='utf-8').write(src)
    r = subprocess.run(['npx', '-y', '@mermaid-js/mermaid-cli@11.4.2', '-i', tmp, '-o', png, '-c', CFG,
                        '-b', 'white', '-w', '1100'], capture_output=True, text=True, shell=True)
    os.remove(tmp)
    if r.returncode == 0: fix_size(png)
    return r.returncode == 0, r.stderr[-300:]

def fix_size(svg_path):
    """width="100%" -> viewBox ki asli chaudai / unchai, taaki chhota diagram khinche nahi."""
    t = open(svg_path, encoding='utf-8').read()
    m = re.search(r'<svg[^>]*>', t)
    tag = m.group(0)
    vb = re.search(r'viewBox="[\d.\-]+ [\d.\-]+ ([\d.]+) ([\d.]+)"', tag)
    if not vb: return
    w, h = round(float(vb.group(1))), round(float(vb.group(2)))
    new = re.sub(r' width="100%"', f' width="{w}" height="{h}"', tag)
    new = re.sub(r'max-width:\s*[\d.]+px;\s*', '', new)
    open(svg_path, 'w', encoding='utf-8').write(t.replace(tag, new, 1))

CONCEPT = os.path.join(HERE, 'concept')

def merge_concept(svg_path, mmd_src, term):
    """Design SVG ke neeche concept SVG jodo, peele dabbe se dashed teer "kaise?"."""
    d = open(svg_path, encoding='utf-8').read()
    c = open(os.path.join(CONCEPT, 'out', term + '.svg'), encoding='utf-8').read()
    def size(t):
        tag = re.search(r'<svg[^>]*>', t).group(0)
        vb = re.search(r'viewBox="([\d.\-]+) ([\d.\-]+) ([\d.]+) ([\d.]+)"', tag)
        return tag, [float(x) for x in vb.groups()]
    dtag, (dvx, dvy, w1, h1) = size(d)
    ctag, (_, _, w2, h2) = size(c)
    W = max(w1, w2); gap = 70; H = h1 + gap + h2
    ox1, ox2 = (W - w1) / 2, (W - w2) / 2
    d2 = d.replace(dtag, re.sub(r' (width|height)="[^"]*"', '', dtag).replace('<svg ', f'<svg x="{ox1:.1f}" y="0" width="{w1:.1f}" height="{h1:.1f}" ', 1), 1)
    c2 = c.replace(ctag, re.sub(r' (width|height)="[^"]*"', '', ctag).replace('<svg ', f'<svg x="{ox2:.1f}" y="{h1+gap:.1f}" width="{w2:.1f}" height="{h2:.1f}" ', 1), 1)
    node = re.search(r'style (\w+) fill:#f9a825', mmd_src).group(1)
    g = re.search(r'id="[^"]*flowchart-' + node + r'-\d+"[^>]*transform="translate\(([\d.\-]+), ([\d.\-]+)\)"><rect[^>]*height="([\d.]+)"', d)
    nx, ny, nh = float(g.group(1)) - dvx + ox1, float(g.group(2)) - dvy, float(g.group(3))
    y1, y2 = ny + nh / 2 + 4, h1 + gap - 4
    arrow = (f'<defs><marker id="kaise" markerUnits="userSpaceOnUse" markerWidth="16" markerHeight="16" refX="12" refY="8" orient="auto">'
             f'<path d="M0,0 L16,8 L0,16 z" fill="#f9a825"/></marker></defs>'
             f'<line x1="{nx:.1f}" y1="{y1:.1f}" x2="{nx:.1f}" y2="{y2:.1f}" stroke="#f9a825" stroke-width="3" '
             f'stroke-dasharray="8,5" marker-end="url(#kaise)"/>'
             f'<text x="{nx+10:.1f}" y="{(y1+y2)/2+5:.1f}" font-family="Arial, Helvetica, sans-serif" font-size="16" '
             f'font-weight="bold" fill="#e65100">kaise?</text>')
    out = (f'<svg xmlns="http://www.w3.org/2000/svg" width="{W:.0f}" height="{H:.0f}" viewBox="0 0 {W:.1f} {H:.1f}">'
           f'<rect width="100%" height="100%" fill="white"/>' + d2 + c2 + arrow + '</svg>')
    open(svg_path, 'w', encoding='utf-8').write(out)

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
        for name in re.findall(r'!\[(\w+)\]\(img/\1\.svg\)', s):
            mmd = os.path.join(img, name + '.mmd')
            if os.path.exists(mmd):
                src = open(mmd, encoding='utf-8').read()
                ok, err = render(src, os.path.join(img, name + '.svg'))
                term = concept_for(f, name) if ok else None
                if term: merge_concept(os.path.join(img, name + '.svg'), src, term)
                print('ok' if ok else 'FAIL', name, ('+ ' + term) if term else '', '' if ok else err)
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
