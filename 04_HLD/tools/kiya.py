"""Card ka DIKKAT block hatao, uski jagah ek line KIYA: (scene ke AB label se, wahi shabd).
    python kiya.py <design.md>
Heading = dikkat, KIYA = jawab, phir tasveer. Jis card ka scene nahi, wo waisa hi.
"""
import json, os, re, sys


def main(f):
    img = os.path.join(os.path.dirname(f), 'img')
    s = open(f, encoding='utf-8').read()
    pat = re.compile(r'(^#### [^\n]*\n)```\n(DIKKAT[^`]*?)```\n(!\[(c\w+)\])', re.M)
    n = 0

    def one(m):
        nonlocal n
        sc = os.path.join(img, m.group(4) + '.scene.json')
        if not os.path.exists(sc):
            return m.group(0)
        rows = json.load(open(sc, encoding='utf-8'))['rows']
        ab = next((r['label'] for r in rows if r['tag'] == 'ab'), None)
        if not ab:
            return m.group(0)
        n += 1
        return m.group(1) + 'KIYA: ' + re.sub(r'^AB:\s*', '', ab) + '\n\n' + m.group(3)
    out = pat.sub(one, s)
    left = len(re.findall(r'^DIKKAT', out, re.M))
    tmp = f + '.tmp'
    open(tmp, 'w', encoding='utf-8').write(out)
    os.replace(tmp, f)
    print(f'{os.path.basename(f)}: {n} KIYA, DIKKAT bache {left}')


if __name__ == '__main__':
    for f in sys.argv[1:]:
        main(f)
