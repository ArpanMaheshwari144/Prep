"""Token bucket (02_rate_limiter SHURU card). Chalao: python token_bucket.py -> out/token_bucket.svg"""
from _svg import *
o = start(960, 430, 'Token bucket: had 10 token, har second 1 bharta, request 1 leti')
# bucket (do baar: pehle bhara, phir khaali)
def bucket(x, filled, cap):
    o.append(f'<path d="M{x},90 L{x+20},300 L{x+140},300 L{x+160},90" fill="none" stroke="#1976d2" stroke-width="3"/>')
    for i in range(filled):
        r, c = divmod(i, 4)
        cx, cy = x + 38 + c * 28, 282 - r * 30
        o.append(f'<circle cx="{cx}" cy="{cy}" r="11" fill="#f9a825" stroke="#333"/>')
    text(o, x + 80, 330, cap, 15, '#1976d2', True, 'middle')
bucket(40, 10, 'bhara: 10 token')
bucket(330, 0, 'khaali: 0 token')
# token aata
arrow(o, 120, 60, 120, 86, '#2e7d32', mark='cg')
text(o, 132, 70, '+1 token / sec', 14, '#2e7d32', True)
# 10 request nikli
arrow(o, 210, 200, 320, 200, '#555')
text(o, 265, 186, '10 request', 14, '#555', True, 'middle')
text(o, 265, 222, 'ek saath = pass', 14, '#2e7d32', True, 'middle')
text(o, 265, 240, '(burst)', 14, '#2e7d32', False, 'middle')
# 11vi
box(o, 360, 360, 100, 34, '11vi', '#ffebee', '#d32f2f', 15, True)
arrow(o, 410, 356, 410, 306, '#d32f2f', mark='cr')
text(o, 470, 383, '-> 429 + kab aao (Retry-After)', 15, '#d32f2f', True)
panel(o, 600, 100, [('NIYAM', True), ('Redis me sirf 2 number:', False), ('kitne token + aakhri baar kab', False), ('', False),
                    ('BHARNA KAB?', True), ('request aane pe hisaab:', False), ('beeta time x 1, had 10 tak', False),
                    ('koi job nahi (lazy)', False), ('', False), ('GHANTA SOYA', True), ('lautne pe bhi max 10', False)])
save(o, 'token_bucket')
