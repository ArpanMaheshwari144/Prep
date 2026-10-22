"""Kafka concepts (22-Oct, Arpan ke saath padha): topic -> partition -> group, EK kahani teeno me.
Wahi 6 khabar (Arpan / Ravi / Sita ke order), wahi SMS + Fraud. Har tasveer upar "pichhli" se judi, neeche "agla".
python 04_HLD/tools/kafka_samjho.py  -> 12_message_queue_kafka/img/k1_topic.html, k2_partition.html, k3_group.html
phir: node 04_HLD/tools/poster2png.js <html>
"""
import os
from html import escape

OUT = os.path.join(os.path.dirname(__file__), '..', 'SYSTEM_DESIGNS', '12_message_queue_kafka', 'img')
COL = {'blue': '#4aa8ff', 'green': '#3ddc97', 'purple': '#a98bff', 'orange': '#ffa94d', 'grey': '#6b7a90'}
BOX = {'user': ('#10243f', '#4aa8ff'), 'server': ('#0f2a22', '#3ddc97'), 'cache': ('#2a1a10', '#ffa94d'),
       'db': ('#1d1838', '#a98bff'), 'plain': ('#152033', '#9fb0c8'), 'dim': ('#111a2a', '#33425a')}
WHO = {'Arpan': '#ffa94d', 'Ravi': '#7cc4ff', 'Sita': '#ff8fc7'}   # har insaan ka apna rang, teeno tasveer me same
# kahani ki 6 khabar, isi kram me aayi (topic offset 0..5)
MSGS = [('Arpan', 'order'), ('Ravi', 'order'), ('Sita', 'order'), ('Arpan', 'pay'), ('Ravi', 'pay'), ('Arpan', 'cancel')]
PART = {'Ravi': 0, 'Arpan': 1, 'Sita': 2}                         # hash(userId) % 3 (kahani ke liye tay)


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


def msg(x, y, w, who, what, off):
    """ek khabar: upar offset number, andar naam (apne rang me) + kaam."""
    c = WHO[who]
    return [t(x + 4, y - 6, f'offset {off}', 11, '#8a9bb5', mono=True),
            f'<rect x="{x}" y="{y}" width="{w}" height="52" rx="7" fill="#0b1220" stroke="{c}" stroke-width="2"/>',
            t(x + w / 2, y + 22, who, 14, c, True, anchor='middle'),
            t(x + w / 2, y + 41, what, 13, '#c9d6e8', mono=True, anchor='middle')]


def arrow(pts, c='blue', dash=False, label=None, lx=None, ly=None):
    d = 'M' + ' L'.join(f'{a} {b}' for a, b in pts)
    o = [f'<path d="{d}" fill="none" stroke="{COL[c]}" stroke-width="3"'
         f'{" stroke-dasharray=\"8 6\"" if dash else ""} marker-end="url(#a{c})"/>']
    if label:
        o.append(t(lx, ly, label, 13, COL[c], mono=True))
    return o


def chain(step):
    """upar daayein: Topic -> Partition -> Group, abhi wala chamakta."""
    o, x = [], 1040
    for i, nm in enumerate(['1 Topic', '2 Partition', '3 Group']):
        on = i + 1 == step
        w = 120
        o.append(f'<rect x="{x}" y="16" width="{w}" height="34" rx="17" fill="{"#2a1a10" if on else "#111a2a"}" '
                 f'stroke="{"#ffa94d" if on else "#33425a"}" stroke-width="2"/>')
        o.append(t(x + w / 2, 39, nm, 14, '#ffa94d' if on else '#6b7a90', on, anchor='middle'))
        if i < 2:
            o.append(t(x + w + 7, 39, '>', 16, '#6b7a90', True))
        x += w + 22
    return o


def strip(y, label, text, kind):
    """upar 'pichhli tasveer' (grey) / neeche 'agla' (peela)."""
    f, s, lc = {'prev': ('#111a2a', '#33425a', '#9fb0c8'), 'next': ('#221c08', '#ffcc4d', '#ffcc4d')}[kind]
    return [f'<rect x="30" y="{y}" width="1440" height="44" rx="9" fill="{f}" stroke="{s}" stroke-width="2"/>',
            t(48, y + 28, label, 15, lc, True), t(48 + 9 * len(label) + 20, y + 28, text, 15, '#e6dcb8' if kind == 'next' else '#c9d6e8')]


def green(y, lines):
    o = [f'<rect x="30" y="{y}" width="1440" height="{22 * len(lines) + 22}" rx="9" fill="#0f2a22" stroke="#3ddc97" stroke-width="2"/>']
    for i, ln in enumerate(lines):
        o.append(t(48, y + 28 + 22 * i, ln, 15, '#cfeee0', bold=(i == 0)))
    return o


def page(name, H, step, title, body):
    head = [f'<svg width="1500" height="{H}" viewBox="0 0 1500 {H}" xmlns="http://www.w3.org/2000/svg"><defs>']
    for k in COL:
        head.append(f'<marker id="a{k}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
                    f'<path d="M0,0 L10,5 L0,10 Z" fill="{COL[k]}"/></marker>')
    head.append(f'</defs><rect width="1500" height="{H}" fill="#0b1220"/>')
    o = [t(30, 42, title, 26, bold=True)] + chain(step) + body
    html = ('<!doctype html>\n<meta charset="utf-8">\n<title>' + escape(title[:40]) + '</title>\n'
            '<style>body{margin:0;background:#0b1220} svg text{font-family:"Segoe UI",Arial,sans-serif} '
            '.m{font-family:Consolas,monospace}</style>\n' + '\n'.join(head + o) + '\n</svg>\n')
    p = os.path.join(OUT, name + '.html')
    open(p, 'w', encoding='utf-8', newline='\n').write(html)
    print(os.path.normpath(p))


# ---------- 1. TOPIC: ek diary, 6 khabar ek line me ----------
b = strip(66, 'Kahani:', 'Swiggy pe order. Order Svc ko har khabar SMS aur Fraud tak pahunchani hai, bina unka intezaar kiye.', 'prev')
b += box(30, 250, 220, 100, 'user', 'Order Svc', ['ek baar likhta,', 'aage badh jaata'])
b += box(300, 135, 860, 350, 'plain')
b.append(t(730, 162, 'KAFKA', 16, '#9fb0c8', True, anchor='middle'))
b += box(325, 180, 810, 160, 'cache')
b.append(t(343, 206, 'topic: order-events   (ek diary, khabar kram se)', 15, '#ffa94d', True, mono=True))
for i, (who, what) in enumerate(MSGS):
    b += msg(343 + i * 131, 250, 120, who, what, i)
b += box(325, 365, 390, 90, 'dim')
b.append(t(345, 397, 'topic: payment-events', 14, '#6b7a90', True, mono=True))
b.append(t(345, 422, 'alag diary, alag khabar', 13, '#6b7a90', mono=True))
b += box(745, 365, 390, 90, 'dim')
b.append(t(765, 397, 'topic: login-events', 14, '#6b7a90', True, mono=True))
b.append(t(765, 422, 'SMS / Fraud ise sunte hi nahi', 13, '#6b7a90', mono=True))
b += arrow([(250, 300), (321, 300)], 'blue')
b += box(1210, 175, 260, 90, 'server', 'SMS Svc', ['order-events sunta'], tsize=18)
b += box(1210, 300, 260, 90, 'server', 'Fraud Svc', ['order-events sunta'], tsize=18)
b += arrow([(1135, 245), (1170, 245), (1170, 220), (1206, 220)], 'green')
b += arrow([(1135, 275), (1170, 275), (1170, 345), (1206, 345)], 'green')
b += green(510, ['Topic = ek tarah ki khabar ki diary. Likhne wala ek baar likhta, jise chahiye wo khud padhta.',
                 'Padhne se khabar mitti nahi (offset 0, 1, 2 ... likhi rehti), isliye SMS aur Fraud dono same 6 khabar padhte.',
                 'Kal koi naya padhne wala aaye to bas order-events sune. Order Svc ka code nahi chhoona.'])
b += strip(625, 'AGLA >', 'Diwali sale: 1 lakh order / sec. Ek diary = ek machine = sab line me atak gaye. Ilaaj: isi diary ke hisse (tasveer 2).', 'next')
page('k1_topic', 700, 1, 'Topic = naam wali diary', b)

# ---------- 2. PARTITION: WAHI diary, 3 hisse ----------
b = strip(66, 'Pichhli tasveer:', 'ek diary order-events, 6 khabar ek line me. Ab WAHI diary 3 hisson me. Topic EK hi rehta.', 'prev')
b += box(30, 300, 230, 120, 'user', 'Order Svc', ['key = userId', 'hash(key) % 3'])
b += box(300, 130, 860, 450, 'plain')
b.append(t(730, 158, 'topic: order-events   (EK topic, andar 3 hisse)', 16, '#ffa94d', True, mono=True, anchor='middle'))
rows = {0: [], 1: [], 2: []}
for who, what in MSGS:
    rows[PART[who]].append((who, what))
names = {0: 'Ravi', 1: 'Arpan', 2: 'Sita'}
for p in range(3):
    y = 180 + p * 130
    b += box(325, y, 810, 110, 'db')
    b.append(t(343, y + 26, f'P{p} · machine {"ABC"[p]}', 15, '#a98bff', True, mono=True))
    b.append(t(1115, y + 26, f'hash({names[p]}) % 3 = {p}', 13, '#8a9bb5', mono=True, anchor='end'))
    for k, (who, what) in enumerate(rows[p]):
        b += msg(343 + k * 170, y + 50, 155, who, what, k)
    b += arrow([(260, 360), (290, 360), (290, y + 60), (321, y + 60)], 'blue')
b += box(1200, 250, 270, 220, 'plain', 'Ab padhega kaun?', ['3 machine, 3 hisse.', 'SMS ek hi banda?', 'to phir wahi atkan.', '', 'jawab: tasveer 3'], dash=True, tsize=17)
b += green(600, ['Same key = same hissa: Arpan ki teeno khabar (order, pay, cancel) P1 me, kram se. Offset har hisse ka apna (0, 1, 2).',
                 'Kram sirf EK hisse ke andar pakka, poore topic me nahi. Isliye key = userId.'])
b += strip(680, 'AGLA >', 'P0, P1, P2 ko SMS aur Fraud kaise baant ke padhein, bina duplicate SMS? (tasveer 3)', 'next')
page('k2_partition', 755, 2, 'Partition = usi diary ke hisse', b)

# ---------- 3. GROUP: WAHI P0 P1 P2, WAHI SMS + Fraud ----------
b = strip(66, 'Pichhli tasveer:', 'order-events ke 3 hisse P0, P1, P2 (wahi 6 khabar). Ab tasveer 1 wale SMS aur Fraud inhe padhte.', 'prev')
b += box(30, 125, 340, 470, 'plain')
b.append(t(200, 152, 'topic: order-events', 15, '#ffa94d', True, mono=True, anchor='middle'))
py = {0: 170, 1: 310, 2: 450}
for p, y in py.items():
    b += box(45, y, 310, 125, 'db')
    b.append(t(60, y + 24, f'P{p}', 16, '#a98bff', True, mono=True))
    for k, (who, what) in enumerate(rows[p]):
        b += msg(60 + k * 98, y + 50, 90, who, what, k)
b += box(470, 125, 600, 240, 'server')
b.append(t(490, 152, 'group "sms"  (3 bande: kaam baanta)', 16, '#3ddc97', True))
for i, nm in enumerate(['banda-A', 'banda-B', 'banda-C']):
    x = 495 + i * 190
    b += box(x, 205, 170, 90, 'user', nm, [f'sirf P{i}'], tsize=17)
    lane, ly = 385 + i * 22, 168 + i * 12
    b += arrow([(355, py[i] + 62), (lane, py[i] + 62), (lane, ly), (x + 85, ly), (x + 85, 201)], 'green')
b.append(t(495, 340, 'Arpan ki khabar sirf banda-B ke paas -> ek hi SMS', 14, '#cfeee0'))
b += box(470, 400, 600, 195, 'server')
b.append(t(490, 427, 'group "fraud"  (1 banda: poori copy)', 16, '#a98bff', True))
b += box(500, 455, 220, 100, 'user', 'banda-X', ['P0 + P1 + P2'], tsize=17)
for p, y in py.items():
    b += arrow([(355, y + 85), (440, y + 85), (440, 490 + p * 15), (496, 490 + p * 15)], 'purple', dash=True)
b += box(1110, 125, 360, 240, 'cache', 'Har team ka apna offset', ['P1 (Arpan) me:', 'sms   offset 2 (cancel tak)',
                                                                  'fraud offset 1 (pay pe)', '',
                                                                  'fraud peeche = sms nahi rukta'], tsize=17)
b += box(1110, 400, 360, 195, 'plain', 'banda-B gira?', ['P1 -> A ya C ko mil jaata', '(rebalance, Kafka khud)',
                                                          'wahi offset 2 se aage'], tsize=17)
b += green(615, ['key se HISSA (P1)  ->  hisse se team ka BANDA (banda-B)  ->  groupId se TEAM (sms / fraud)',
                 'Team ke andar ek hissa = ek banda (duplicate nahi). Alag team = apni poori copy, apna offset.'])
b += strip(695, 'AGLA >', 'banda-B ne SMS bheja aur offset aage likhne se pehle gir gaya to? Khabar khoti ya dobara? (offset + commit)', 'next')
page('k3_group', 770, 3, 'Consumer group = har service ki team', b)
