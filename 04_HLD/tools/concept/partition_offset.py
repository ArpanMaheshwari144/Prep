"""Partition + offset (12_kafka card 3). Chalao: python partition_offset.py -> out/partition_offset.svg"""
from _svg import *
o = start(960, 400, 'Partition + offset: key se tukda, offset se jagah')
text(o, 30, 78, 'topic "signup" = 3 tukde (partition), har khaane me number = offset', 15, '#555')
X0, CW = 230, 52
for r, (p, y) in enumerate([('P0', 110), ('P1', 170), ('P2', 230)]):
    text(o, X0 - 40, y + 27, p, 17, '#00897b', True)
    for i in range(6):
        if r == 1:
            fill = '#c8e6c9' if i <= 3 else '#fff8e1'
        else:
            fill = '#f5f5f5'
        box(o, X0 + i * CW, y, CW, 40, str(i), fill, '#00897b' if r == 1 else '#bbb', 15)
box(o, 20, 172, 110, 36, 'user:7', '#f9a825', '#333', 16, True)
arrow(o, 130, 190, X0 - 48, 190)
text(o, 132, 162, 'hash % 3 = 1', 14, '#555')
lx = X0 + 4 * CW
o.append(f'<line x1="{lx}" y1="160" x2="{lx}" y2="300" stroke="#d32f2f" stroke-width="3" stroke-dasharray="7,5"/>')
text(o, lx, 322, 'email group ka offset: 3 tak padha', 15, '#d32f2f', True, 'middle')
text(o, lx, 344, 'restart -> 4 se aage, kuch nahi khoya', 15, '#d32f2f', False, 'middle')
text(o, X0, 376, 'hara = padh liya   peela = abhi baaki   (padhne se kuch mitta nahi)', 14, '#555')
panel(o, 640, 100, [('NIYAM', True), ('ek key = hamesha ek tukda,', False), ('kram us tukde ke andar', False), ('', False),
                    ('HAR GROUP APNA OFFSET', True), ('email 3 pe, analytics 5 pe;', False), ('ek peeche, doosra nahi rukta', False),
                    ('', False), ('DOBARA PADHNA', True), ('offset peeche karo (replay)', False)])
save(o, 'partition_offset')
