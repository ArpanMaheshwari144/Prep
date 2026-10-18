"""Leader + ISR + acks=all (12_kafka card 6). Chalao: python isr_acks.py -> out/isr_acks.svg"""
from _svg import *
o = start(960, 420, 'Leader + ISR (saath chalti copy), acks=all')
BX, BW, BH, CX, CW = 220, 340, 56, 330, 36
rows = [('A (leader)', 90, 6, '#1976d2'), ('B (copy)', 190, 6, '#1976d2'), ('C (copy)', 290, 3, '#d32f2f')]
for name, y, n, c in rows:
    box(o, BX, y, BW, BH, '', '#fafafa', c)
    text(o, BX + 12, y + 34, name, 16, c, True)
    for i in range(6):
        fill = ('#c8e6c9' if i < n else 'white')
        if i == 5 and n == 6: fill = '#f9a825'
        box(o, CX + i * CW, y + 10, CW, 36, str(i) if i < n else '', fill, '#999', 14)
o.append(f'<rect x="{BX-12}" y="78" width="{BW+24}" height="180" rx="8" fill="none" stroke="#2e7d32" '
         'stroke-width="2.5" stroke-dasharray="8,5"/>')
text(o, BX - 10, 280, 'ISR = A, B', 16, '#2e7d32', True)
box(o, 20, 98, 120, 40, 'Producer', '#e3f2fd', '#1976d2', 16, True)
arrow(o, 140, 112, BX - 14, 112)
text(o, 150, 104, 'msg 5', 14, '#555')
arrow(o, BX - 14, 130, 140, 130, '#2e7d32', mark='cg')
text(o, 20, 168, 'OK tab, jab A + B', 14, '#2e7d32', True)
text(o, 20, 186, 'dono ne likha (acks=all)', 14, '#2e7d32', True)
arrow(o, BX + BW - 30, 146, BX + BW - 30, 188, '#555', True)
arrow(o, BX + BW - 50, 146, BX + BW - 50, 288, '#d32f2f', True, 'cr')
text(o, BX, 372, 'C peeche (sirf 0..2) -> ISR se bahar,', 15, '#d32f2f', True)
text(o, BX, 394, 'OK uska intezaar nahi karta', 15, '#d32f2f', True)
panel(o, 640, 90, [('NIYAM', True), ('acks=all: ISR me sab likhe,', False), ('tabhi producer ko OK', False), ('', False),
                   ('A MARA', True), ('Controller ISR se B ko leader', False), ('(B ke paas sab hai)', False), ('', False),
                   ('C PEECHE', True), ('leader nahi ban sakta', False), ('', False),
                   ('COPY KAM (min ISR)', True), ('likhna band, khona nahi', False)])
save(o, 'isr_acks')
