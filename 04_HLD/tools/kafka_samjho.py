"""Kafka samjho (22-Oct, Arpan ke saath padha): topic / partition / group ki 3 tasveer.
python 04_HLD/tools/kafka_samjho.py  -> 12_message_queue_kafka/img/k1_topic.html, k2_partition.html, k3_group.html
phir: node 04_HLD/tools/poster2png.js <html>
"""
import os
from html import escape

OUT = os.path.join(os.path.dirname(__file__), '..', 'SYSTEM_DESIGNS', '12_message_queue_kafka', 'img')
COL = {'blue': '#4aa8ff', 'green': '#3ddc97', 'purple': '#a98bff', 'orange': '#ffa94d', 'grey': '#6b7a90'}
BOX = {'user': ('#10243f', '#4aa8ff'), 'server': ('#0f2a22', '#3ddc97'), 'cache': ('#2a1a10', '#ffa94d'),
       'db': ('#1d1838', '#a98bff'), 'plain': ('#152033', '#9fb0c8')}


def t(x, y, s, size=15, fill='#e6edf3', bold=False, mono=False, anchor='start'):
    return (f'<text x="{x}" y="{y}" font-size="{size}"{" font-weight=\"700\"" if bold else ""} fill="{fill}"'
            f'{" class=\"m\"" if mono else ""}{"" if anchor == "start" else f" text-anchor=\"{anchor}\""}>{escape(s)}</text>')


def box(x, y, w, h, kind, title=None, lines=(), dash=False, tsize=19):
    f, s = BOX[kind]
    o = [f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="12" fill="{f}" stroke="{s}" stroke-width="2.5"'
         f'{" stroke-dasharray=\"8 6\"" if dash else ""}/>']
    if title:
        o.append(t(x + w / 2, y + 32, title, tsize, bold=True, anchor='middle'))
    for i, ln in enumerate(lines):
        o.append(t(x + w / 2, y + 58 + i * 21, ln, 13, '#c9d6e8', mono=True, anchor='middle'))
    return o


def cell(x, y, w, s, kind='plain'):
    f, st = BOX[kind]
    return [f'<rect x="{x}" y="{y}" width="{w}" height="34" rx="6" fill="{f}" stroke="{st}" stroke-width="1.5"/>',
            t(x + w / 2, y + 22, s, 13, '#e6edf3', mono=True, anchor='middle')]


def arrow(pts, c='blue', dash=False, label=None, lx=None, ly=None):
    d = 'M' + ' L'.join(f'{a} {b}' for a, b in pts)
    o = [f'<path d="{d}" fill="none" stroke="{COL[c]}" stroke-width="3"'
         f'{" stroke-dasharray=\"8 6\"" if dash else ""} marker-end="url(#a{c})"/>']
    if label:
        o.append(t(lx, ly, label, 13, COL[c], mono=True))
    return o


def page(name, H, title, body, caption):
    head = [f'<svg width="1500" height="{H}" viewBox="0 0 1500 {H}" xmlns="http://www.w3.org/2000/svg"><defs>']
    for k in COL:
        head.append(f'<marker id="a{k}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
                    f'<path d="M0,0 L10,5 L0,10 Z" fill="{COL[k]}"/></marker>')
    head.append(f'</defs><rect width="1500" height="{H}" fill="#0b1220"/>')
    o = [t(30, 42, title, 26, bold=True)] + body
    y = H - 30 - 22 * len(caption)
    o.append(f'<rect x="30" y="{y - 26}" width="1440" height="{22 * len(caption) + 22}" rx="9" fill="#0f2a22" '
             f'stroke="#3ddc97" stroke-width="2"/>')
    for i, ln in enumerate(caption):
        o.append(t(48, y + 22 * i, ln, 15, '#cfeee0', bold=(i == 0)))
    html = ('<!doctype html>\n<meta charset="utf-8">\n<title>' + escape(title[:40]) + '</title>\n'
            '<style>body{margin:0;background:#0b1220} svg text{font-family:"Segoe UI",Arial,sans-serif} '
            '.m{font-family:Consolas,monospace}</style>\n' + '\n'.join(head + o) + '\n</svg>\n')
    p = os.path.join(OUT, name + '.html')
    open(p, 'w', encoding='utf-8', newline='\n').write(html)
    print(os.path.normpath(p))


# 1. TOPIC
b = []
rows = [('order-events', ['order#101', 'order#102', 'order#103', 'order#104'], 'Order Svc', 'SMS · Email · Fraud'),
        ('payment-events', ['paisa 500', 'paisa 200', 'paisa 900'], 'Payment Svc', 'Ledger · Fraud'),
        ('login-events', ['login Ravi', 'login Sita'], 'Login Svc', 'Security')]
b += box(330, 80, 780, 420, 'plain')
b.append(t(720, 110, 'KAFKA', 17, '#9fb0c8', True, anchor='middle'))
for i, (tp, msgs, prod, cons) in enumerate(rows):
    y = 130 + i * 120
    b += box(30, y + 5, 220, 80, 'user', prod)
    b += box(360, y, 720, 90, 'cache')
    b.append(t(378, y + 26, 'topic: ' + tp, 15, '#ffa94d', True, mono=True))
    for k, m in enumerate(msgs):
        b += cell(378 + k * 165, y + 42, 150, m)
    b += box(1180, y + 5, 290, 80, 'server', cons, tsize=17)
    b += arrow([(250, y + 45), (356, y + 45)], 'blue')
    b += arrow([(1080, y + 45), (1176, y + 45)], 'green')
page('k1_topic', 640, 'Topic = naam wali diary. Likhne wala ek baar likhta, jise chahiye wo khud padhta', b,
     ['Topic = ek tarah ki khabar ka naam wala khaana (diary / log)',
      'Padhne se khabar mitti nahi (retention tak rehti), isliye SMS, Email, Fraud teeno same diary padh lete.',
      'Naya padhne wala (Fraud) aaye to bas topic sune, Order Svc ka code nahi chhoona (decoupling).'])

# 2. PARTITION
b = []
b += box(30, 230, 230, 120, 'user', 'Order Svc', ['key = userId'])
b += box(330, 80, 780, 440, 'plain')
b.append(t(720, 110, 'topic: order-events', 17, '#ffa94d', True, mono=True, anchor='middle'))
prow = [('P0 · machine A', [('Ravi: order', 'plain'), ('Mohan: order', 'plain'), ('Ravi: pay', 'plain')]),
        ('P1 · machine B', [('Arpan: order', 'cache'), ('Arpan: pay', 'cache'), ('Arpan: cancel', 'cache')]),
        ('P2 · machine C', [('Sita: order', 'plain'), ('Sita: pay', 'plain')])]
for i, (lab, msgs) in enumerate(prow):
    y = 130 + i * 125
    b += box(360, y, 720, 100, 'db')
    b.append(t(378, y + 26, lab, 15, '#a98bff', True, mono=True))
    for k, (m, kd) in enumerate(msgs):
        b += cell(378 + k * 190, y + 46, 175, m, kd)
    b += arrow([(260, 290), (300, 290), (300, y + 50), (356, y + 50)], 'blue')
    b += box(1180, y + 10, 290, 80, 'server', f'SMS banda-{i + 1}', [f'sirf P{i} padhta'], tsize=17)
    b += arrow([(1080, y + 50), (1176, y + 50)], 'green')
page('k2_partition', 700, 'Partition = topic ke hisse. Bojh kai machine pe, kai bande ek saath padhte', b,
     ['Same key (userId) = same partition: Arpan ki saari khabar P1 me, kram se (order, pay, cancel)',
      'Kram sirf EK partition ke andar pakka, poore topic me nahi.  Partition jitne, utne bande hi kaam ke',
      '(3 partition pe 5 bande = 2 khaali).  Kaunsa partition = hash(key) % partitions, producer tay karta.'])

# 3. GROUP
b = []
b += box(30, 90, 300, 470, 'plain')
b.append(t(180, 120, 'topic: order-events', 15, '#ffa94d', True, mono=True, anchor='middle'))
py = {0: 160, 1: 300, 2: 440}
for p, y in py.items():
    b += box(55, y, 250, 80, 'db', f'P{p}', [f'msg ... {[120, 88, 140][p]}'], tsize=17)
b += box(470, 80, 600, 250, 'server')
b.append(t(490, 108, 'group "sms"  (3 bande: kaam baanta)', 16, '#3ddc97', True))
for i, (nm, p) in enumerate([('banda-A', 0), ('banda-B', 1), ('banda-C', 2)]):
    x = 495 + i * 190
    b += box(x, 165, 170, 90, 'user', nm, [f'P{p}'], tsize=17)
    lane, ly = 375 + i * 22, 124 + i * 12
    b += arrow([(305, py[p] + 40), (lane, py[p] + 40), (lane, ly), (x + 85, ly), (x + 85, 161)], 'green')
b.append(t(495, 305, 'har khabar team me sirf EK bande ko -> ek hi SMS', 14, '#cfeee0'))
b += box(470, 380, 600, 180, 'server')
b.append(t(490, 408, 'group "fraud"  (1 banda: poori copy)', 16, '#a98bff', True))
b += box(500, 430, 220, 100, 'user', 'banda-X', ['P0 + P1 + P2'], tsize=17)
for p, y in py.items():
    b += arrow([(305, y + 50), (430, y + 50), (430, 470 + p * 15), (496, 470 + p * 15)], 'purple', dash=True)
b += box(1120, 120, 350, 170, 'cache', 'Har team ka apna offset', ['sms   P0 = 120', 'fraud P0 =  95',
                                                                    'fraud peeche = sms nahi rukta'], tsize=17)
b += box(1120, 380, 350, 150, 'plain', 'banda-B gira?', ['P1 -> A ya C ko', '(rebalance, Kafka khud)'], tsize=17)
page('k3_group', 700, 'Group = ek service ki team. Team ke andar kaam bata, har team ko poori copy', b,
     ['key se PARTITION  ->  partition se team ka BANDA  ->  groupId se TEAM',
      'Team ke andar ek partition = ek banda (duplicate nahi). Alag team (sms, fraud) = apni poori copy, apna offset.'])
