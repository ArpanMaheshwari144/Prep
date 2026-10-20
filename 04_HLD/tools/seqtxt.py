"""Card ki kahani TEXT me (image nahi): upar dabbe, neeche teer 1, 2, 3. Pehle DIKKAT, phir ILAAJ.
Ek scene = (title, [dabbe ke naam], [kadam]). Kadam:
  ('a', from, to, label[, note])   teer, from / to = dabbe ka number (0 se)
  ('x', at, text)                  us dabbe pe X (gira / band) + text
  ('t', at, text)                  us dabbe ke paas sirf text
Chalao: python 04_HLD/tools/seqtxt.py <design.md> <scenes.py>  -> har card ka <details> / text block badal deta.
"""
import re, sys
W = 20

def _row(n):
    s = [' '] * (3 + W * (n - 1) + 1)
    for i in range(n): s[3 + W * i] = '|'
    return s

def scene(title, names, steps):
    global W
    n = len(names)
    # dabbe ki doori itni ki sabse lamba label fit ho (kam se kam 20)
    W = max([20] + [-(-(len(s[3]) + 5) // abs(s[2] - s[1])) + 1 for s in steps if s[0] == 'a'])
    out = [title, '', '  ' + ''.join(x.ljust(W) for x in names).rstrip(), ''.join(_row(n))]
    for st in steps:
        s = _row(n)
        if st[0] == 'a':
            _, a, b, label, *note = st
            x1, x2 = 3 + W * a, 3 + W * b
            lo, hi = min(x1, x2), max(x1, x2)
            seg, body = hi - lo - 1, ' ' + label + ' '
            assert len(body) + 2 <= seg, f'label lamba: {label}'
            k = (seg - len(body)) // 2
            fill = '-' * k + body + '-' * (seg - k - len(body))
            fill = fill[:-1] + '>' if a < b else '<' + fill[1:]
            s[lo + 1:hi] = list(fill)
            line = ''.join(s) + ('   ' + note[0] if note else '')
        else:
            _, at, text = st
            x = 3 + W * at
            if st[0] == 'x': s[x] = 'X'
            line = ''.join(s)
            pos = x + 3
            if at < n - 1 and pos + len(text) >= 3 + W * (at + 1):
                line = line + '   ' + text          # beech me jagah nahi -> aakhri dabbe ke baad
            else:
                line = line[:pos].ljust(pos) + text + line[pos + len(text):]
        out.append(line.rstrip())
    out.append(''.join(_row(n)))
    return out

def card(scenes):
    lines = []
    for i, sc in enumerate(scenes):
        if i: lines.append('')
        lines += scene(*sc)
    for l in lines: assert len(l) <= 110, f'line > 110: {l}'
    return '```text\n' + '\n'.join(lines) + '\n```'   # text = GitHub rang nahi bharta

def apply(md_path, cards):
    t = open(md_path, encoding='utf-8').read()
    parts = re.split(r'(\n## (\d+)\. [^\n]*\n)', t)
    out = parts[0]
    for i in range(1, len(parts), 3):
        head, num, body = parts[i], int(parts[i + 1]), parts[i + 2]
        if num in cards:
            new = card(cards[num])
            body, k = re.subn(r'<details><summary>kahani dekho.*?</details>', lambda m: new, body, count=1, flags=re.S)
            if not k:
                body, k = re.subn(r'```(?:text)?\nDIKKAT:.*?```', lambda m: new, body, count=1, flags=re.S)
            if not k:   # purani GIF line
                body, k = re.subn(r'!\[[^\]]*\]\(img/[^)]*\.gif\)', lambda m: new, body, count=1)
            if not k:   # kuch nahi tha -> Cue line ke neeche
                body, k = re.subn(r'(Cue:[^\n]*\n)', lambda m: m.group(1) + '\n' + new + '\n', body, count=1)
            assert k, f'card {num}: jagah nahi mili'
        out += head + body
    open(md_path, 'w', encoding='utf-8').write(out)

if __name__ == '__main__':
    ns = {}
    exec(open(sys.argv[2], encoding='utf-8').read(), ns)
    apply(sys.argv[1], ns['CARDS'])
    print('ok', sys.argv[1], sorted(ns['CARDS']))
