"""HLD design file ke har mermaid diagram ko SVG image banata (mermaid-cli).
Chalao:  python 04_HLD/tools/render_diagrams.py 04_HLD/SYSTEM_DESIGNS/13_banking_system/13_banking_system.md
- Pehli baar: ```mermaid block -> upar image, neeche <details> me source.
- Baad me: source (details wala mermaid) badlo, phir yahi chalao -> image dobara banti.
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
    return r.returncode == 0, r.stderr[-300:]

def main(f):
    d = os.path.dirname(f); img = os.path.join(d, 'img'); os.makedirs(img, exist_ok=True)
    s = open(f, encoding='utf-8').read()
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
    for f in sys.argv[1:]: main(f)
