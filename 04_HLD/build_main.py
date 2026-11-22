"""Hand notes se main file banao: notes ka poora text + har kadam ke neeche uska diagram.

Badlaav sirf HAND NOTES me karo. Diagram main file me <!-- diagram K --> ... <!-- /diagram --> ke beech rehte hain;
script unhe purani main file se utha ke naye text ke saath jod deta hai.

Chalao:  python 04_HLD/build_main.py 01_url_shortener          (ek design)
         python 04_HLD/build_main.py all                       (sab design jinki main me diagram marker hain)
"""
import re, sys, os, glob

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'SYSTEM_DESIGNS')
HEAD = re.compile(r'^\*\*(\d+[a-z]?)\.\s')
MAIN_HEAD = re.compile(r'^## (\d+[a-z]?)\.\s')
MARK = re.compile(r'<!-- diagram (\S+) -->\n(.*?)<!-- /diagram -->', re.S)


def files(design):
    d = os.path.join(ROOT, design)
    notes = glob.glob(os.path.join(d, '*_HAND_NOTES.md'))[0]
    main = [f for f in glob.glob(os.path.join(d, '[0-9]*.md'))][0]
    return notes, main


def diagrams_from_main(text):
    """Pehle marker dekho; na mile to purane '## N.' heading ke neeche ke mermaid block."""
    found = {k: v.strip('\n') for k, v in MARK.findall(text)}
    if found:
        return found
    out, key = {}, None
    lines = text.split('\n')
    i = 0
    while i < len(lines):
        l = lines[i]
        m = MAIN_HEAD.match(l)
        if m:
            key = m.group(1)
        elif l.startswith('## Poora dhaancha'):
            key = 'poora'
        elif l.startswith('## '):
            key = None
        if l.strip() == '```mermaid' and key:
            j = i
            while not lines[j].strip() == '```' or j == i:
                j += 1
            block = '\n'.join(lines[i:j + 1])
            out[key] = (out[key] + '\n\n' + block) if key in out else block
            i = j
        i += 1
    return out


def build(design):
    notes, main = files(design)
    ntext = open(notes, encoding='utf-8').read()
    mtext = open(main, encoding='utf-8').read()
    diag = diagrams_from_main(mtext)
    title = mtext.split('\n', 1)[0]
    nlines = ntext.split('\n')[1:]  # notes ka title chhod do
    out = [title, '',
           '_Ye file hand notes se banti hai (`04_HLD/build_main.py`): wahi dikkat, wahi jawab, wahi Technical, '
           'aur har kadam ke neeche diagram (kahani ka shabd, bracket me asli naam). Badlaav sirf hand notes me._', '']
    cur = None

    def flush(k):
        if k and k in diag:
            out.extend(['', '<!-- diagram %s -->' % k, diag[k], '<!-- /diagram -->', ''])

    for l in nlines:
        m = HEAD.match(l)
        if m:
            flush(cur)
            cur = m.group(1)
        out.append(l)
    flush(cur)
    if 'poora' in diag:
        out.extend(['', '## Poora dhaancha ek nazar me', '', '<!-- diagram poora -->', diag['poora'],
                    '<!-- /diagram -->', ''])
    out.append('[Hand notes](%s)' % os.path.basename(notes))
    missing = [k for k in diag if k != 'poora' and not re.search(r'^\*\*%s\.\s' % re.escape(k), ntext, re.M)]
    open(main, 'w', encoding='utf-8', newline='').write('\n'.join(out).rstrip() + '\n')
    print(design, ': diagram', len(diag), '| notes me kadam nahi mila:', missing or 'koi nahi')


if __name__ == '__main__':
    arg = sys.argv[1]
    if arg == 'all':
        for d in sorted(os.listdir(ROOT)):
            try:
                _, m = files(d)
            except IndexError:
                continue
            if '<!-- diagram ' in open(m, encoding='utf-8').read():
                build(d)
    else:
        build(arg)
