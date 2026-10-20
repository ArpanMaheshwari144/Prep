"""md me card N ka <details> block = us card ki maujooda PNG (cN_1.., ?v=V). python set_scenes.py <md> <v> <N> [N ...]"""
import re, sys, os, glob
md, v, cards = sys.argv[1], sys.argv[2], [int(x) for x in sys.argv[3:]]
d = os.path.join(os.path.dirname(md), 'img')
t = open(md, encoding='utf-8').read()
for n in cards:
    k = len(glob.glob(os.path.join(d, f'c{n}_[0-9].png')))
    imgs = '\n\n'.join(f'<img src="img/c{n}_{i}.png?v={v}" width="900">' for i in range(1, k + 1))
    new = f'<details><summary>kahani dekho ({k} scene)</summary>\n\n{imgs}\n\n</details>'
    pat = re.compile(r'(\n## ' + str(n) + r'\. .*?)<details><summary>kahani dekho.*?</details>', re.S)
    t, c = pat.subn(lambda m: m.group(1) + new, t, count=1)
    assert c, f'card {n} nahi mila'
open(md, 'w', encoding='utf-8').write(t); print('ok', cards)
