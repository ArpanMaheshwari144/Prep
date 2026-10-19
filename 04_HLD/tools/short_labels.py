"""Peele dabbe ka label sirf "naam<br/>(term)". Term = label ke "->" wali line ka aakhri bracket.
    python short_labels.py <design folder> [cN=term ...]   (term na mile to haath se do: c3="lock order")
Jo label pehle se <= 2 line, waisa hi. Na mila term -> NAAM-ONLY list print, file nahi badalti us card ki.
"""
import glob, os, re, sys


def main(folder, manual):
    left = []
    for f in sorted(glob.glob(os.path.join(folder, 'img', 'c*.mmd'))):
        card = os.path.basename(f)[:-4]
        s = open(f, encoding='utf-8').read()
        ys = re.findall(r'style (\w+) fill:#f9a825', s)
        changed = False
        for n in ys:
            m = re.search(r'(\b' + n + r'\[")([^"]*)("\])', s)
            if not m:
                continue
            lines = m.group(2).split('<br/>')
            if len(lines) <= 2:
                continue
            term = manual.get(card)
            if not term:
                arrow = next((l for l in lines if l.lstrip().startswith('->')), '')
                b = re.findall(r'\(([^()]*)\)', arrow)
                term = b[-1] if b else None
            if not term:
                left.append(card + ' ' + n + ' :: ' + m.group(2)[:90])
                continue
            s = s[:m.start(2)] + lines[0] + '<br/>(' + term + ')' + s[m.end(2):]
            changed = True
        if changed:
            open(f, 'w', encoding='utf-8').write(s)
    for l in left:
        print('NAAM-ONLY', l)


if __name__ == '__main__':
    man = dict(a.split('=', 1) for a in sys.argv[2:])
    main(sys.argv[1], man)
