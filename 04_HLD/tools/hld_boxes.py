# HLD design file ko final saanche me: upar RAIL dabba, har card me Rail tag, HAL block se "↳ agar" hata ke
# card ke neeche [!IMPORTANT] Yaad rakh + [!TIP] Cross-Q ("sawaal? -> **cue**: poora vakya").
# use: python hld_boxes.py <spec.py> [--write]     (bina --write = sirf dikhao, file nahi badalti)
# spec: FILE, RAIL = [(kadam, is design me, card)], CARDS = {n: (rail tag, yaad rakh, {item_no: cue}, extra_md)}
import re, sys

NL = chr(10)
spec = {}
exec(open(sys.argv[1], encoding='utf-8').read(), spec)
p = spec['FILE']
s = open(p, encoding='utf-8').read()
WRITE = '--write' in sys.argv


def items(block):
    """code block se ↳ items: [(sawaal, pehli line ka jawab, aage ki lines)]"""
    out, cur = [], None
    for l in block.split(NL):
        if l.startswith('        ↳'):
            t = l.strip()[1:].strip()
            if t.startswith('agar '):
                t = t[5:]
            q, _, a = t.partition(' -> ') if ' -> ' in t else t.partition('->')
            cur = [q.strip(), a.strip(), []]
            out.append(cur)
        elif cur and l.startswith('          '):
            cur[2].append(l.strip())
        else:
            cur = None
    return out


def strip_agar(block):
    out, cur = [], False
    for l in block.split(NL):
        if l.startswith('        ↳'):
            cur = True
            continue
        if cur and l.startswith('          '):
            continue
        cur = False
        out.append(l)
    return NL.join(out)


def line(q, a, rest, cue):
    rest = ' '.join(rest)
    if rest.startswith('-> '):
        rest = rest[3:]
    if cue is None:                       # cue khud nikalo
        if a and rest:
            cue, full = a, rest
        elif a:
            m = re.match(r'([^.,]{3,45})[.,]\s*(.+)', a)
            cue, full = (m.group(1).strip(), m.group(2).strip()) if m else (a, '')
        else:
            cue, full = '??', rest
    else:
        full = (a + '. ' + rest).strip('. ') if a and rest else (a or rest)
    full = full.strip()
    return f'> - {q} → **{cue}**' + (f': {full}' if full else ''), cue == '??'


RAIL = spec['RAIL']
rail_md = ('> [!IMPORTANT]' + NL + '> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**' + NL +
           '> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla' + NL +
           '>' + NL + '> | rail | is design me | card |' + NL + '> |---|---|---|' + NL +
           ''.join(f'> | {a} | {b} | {c} |' + NL for a, b, c in RAIL))

bad = 0
for n, (tag, yaad, cues, extra) in spec['CARDS'].items():
    m = re.search(r'^## %d\. .*$' % n, s, flags=re.M)
    assert m, n
    cm = re.compile(r'^Cue: (.*)$', re.M).search(s, m.end())
    s = s[:cm.start()] + f'**Rail {tag}** · Cue: ' + cm.group(1) + s[cm.end():]
    st = s.index('</details>', m.end())
    b1 = s.index('```', st)
    b2 = s.index('```', b1 + 3) + 3
    block = s[b1:b2]
    its = items(block)
    lines = []
    print(f'== card {n}')
    for k, (q, a, rest) in enumerate(its, 1):
        ln, unk = line(q, a, rest, cues.get(k))
        bad += unk
        lines.append(ln)
        print(f'  {k}. {ln[4:]}')
    box = (NL + NL + '> [!IMPORTANT]' + NL + '> **Yaad rakh:** ' + yaad + NL)
    if lines:
        box += NL + '> [!TIP]' + NL + '> **Cross-Q: sawaal → cue, phir poora jawab**' + NL + NL.join(lines) + NL
    if extra:
        box += NL + extra.rstrip() + NL
    s = s[:b1] + strip_agar(block) + box + s[b2:]

a = NL + '---' + NL + NL + '*ab tootega kahan?'
if a in s:
    s = s.replace(a, NL + rail_md + a, 1)
else:
    i = s.index(NL + '---' + NL, s.index('## Shuru'))
    s = s[:i] + NL + NL + rail_md + s[i:]
s = s.replace('Neeche har card ek block: HAL (↳ agar = cross-Q) · KYUN · NAHI (doosra kyun nahi).',
              'Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).')
left = s.count('↳')
print('cue ?? =', bad, '| bacha ↳ =', left)
if WRITE and not bad:
    open(p, 'w', encoding='utf-8').write(s)
    print('likh diya')
