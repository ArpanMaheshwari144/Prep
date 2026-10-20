"""Kahani ke scene mermaid se: img/cN_k.mmd -> img/cN_k.png (wahi naam jo md ke <img> me hai).
Chalao:  python 04_HLD/tools/scene_mmd.py 04_HLD/SYSTEM_DESIGNS/06_payment_system/img/c1_*.mmd
Scene file me sabse upar title (DIKKAT / ILAAJ / AGAR / WAPAS caption), phir flowchart; teer ke label pe kram
("1. ...", "2. ..."). Rang: classDef bad / good / warn / idle neeche se apne aap judte.
"""
import os, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__))
CFG = os.path.join(HERE, 'mermaid_scene_cfg.json')
CLASSES = '''
  classDef bad fill:#ffe5e5,stroke:#e53935,color:#000,stroke-width:2px
  classDef good fill:#e6f4ea,stroke:#43a047,color:#000,stroke-width:2px
  classDef warn fill:#fff4e0,stroke:#f0a030,color:#000,stroke-width:2px
  classDef idle fill:#f2f2f2,stroke:#999,color:#555
'''
for src in sys.argv[1:]:
    text = open(src, encoding='utf-8').read().rstrip() + '\n' + CLASSES
    # dabbe / teer ke label me '->' mermaid kha jaata, isliye wahan teer ka nishaan; asli teer (-->| -.->|) waise hi
    head, sep, body = text.rpartition('---\n')
    body = body.replace('->|', '@@').replace(' -->', ' @#').replace('->', '→').replace('@@', '->|').replace(' @#', ' -->')
    text = head + sep + body
    tmp = src + '.tmp.mmd'
    open(tmp, 'w', encoding='utf-8').write(text)
    png = src[:-4] + '.png'
    r = subprocess.run(['npx', '-y', '@mermaid-js/mermaid-cli@11.4.2', '-i', tmp, '-o', png, '-c', CFG,
                        '-b', 'white', '-w', '1600', '-s', '1.5'], capture_output=True, text=True, shell=True)
    os.remove(tmp)
    print(('ok  ' if r.returncode == 0 else 'ERR ') + png + ('' if r.returncode == 0 else ' ' + r.stderr[-300:]))
