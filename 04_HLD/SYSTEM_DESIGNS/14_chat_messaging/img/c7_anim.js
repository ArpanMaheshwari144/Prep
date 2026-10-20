// Card 7: har member ke inbox me copy -> ek message, 500 likhai. Ilaaj: group ka EK copy + bhejna server-wise.
const DUR = 18, FIX = 8;
const AR = [120, 190], SL = [148, 190], SR = [280, 190], SB = [220, 160], CA = [430, 92], RD = [430, 268],
  S2 = [688, 100], S5 = [688, 270];
function setup() {
  wire('M120 190 H160');
  box(20, 160, 100, 60, 'Arpan');
  box(160, 160, 120, 60, 'Chat Server');
  wire('M280 180 L560 120', 'wi');
  box(560, 20, 320, 320, 'Cassandra', 'har member ka inbox', { id: 'ibx' });
  wire('M220 160 L330 95', 'wc'); wire('M220 220 L330 275', 'wr');
  wire('M280 190 L690 100', 'w2'); wire('M280 190 L690 270', 'w5');
  box(330, 50, 200, 80, 'Cassandra', 'group chat: College Gang', { id: 'ca', stroke: C.acc });
  box(330, 250, 200, 80, 'Redis', 'online kahan', { id: 'rd' });
  box(700, 60, 180, 80, 'Server-2', '120 online', { id: 's2' });
  box(700, 230, 180, 80, 'Server-5', '80 online', { id: 's5' });
}
function frame(t, fix, tl) {
  vis('ibx', !fix); vis('wi', !fix);
  ['wc', 'wr', 'w2', 'w5', 'ca', 'rd', 's2', 's5'].forEach(i => vis(i, fix));
  dot(path([[0.3, AR], [0.9, SL]], tl < 0.9 ? tl : -1), C.msg, '', 'msg');
  if (!fix) {
    CAP.textContent = 'DIKKAT: College Gang (500). Har member ke inbox me copy -> ek message, 500 likhai';
    const n = Math.max(0, Math.min(500, Math.round((tl - 1.2) / 4.5 * 500)));
    for (let k = 0; k < 6; k++) {
      const s = 1.2 + k * 0.7;
      if (tl > s && tl < s + 0.6 && n < 500) dot(path([[s, SR], [s + 0.6, [600, 140 + k * 25]]], tl), C.bad);
    }
    for (let i = 0; i < n; i++) {
      const x = 575 + (i % 25) * 12, y = 70 + Math.floor(i / 25) * 12;
      el('rect', { x, y, width: 10, height: 10, rx: 1, fill: C.bad }, DYN);
    }
    T(720, 330, `likhai: ${n}`, { c: C.bad, b: 1, size: 15 });
    T(420, 330, 'ek message, 500 copy', { c: C.bad, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: group ka message EK baar, group chat ke neeche. Bhejna server-wise, har server ko ek call';
  if (tl > 0.9 && tl < 1.6) dot(path([[0.9, SB], [1.6, [318, 102]]], tl), C.acc, '', 'likho', C.acc);
  if (tl > 1.6) { cells(375, 96, [['msg', C.good]], { w: 110, h: 24 }); T(430, 150, 'likhai: 1', { c: C.good, b: 1, size: 15, halo: 1 }); }
  if (tl > 2.2 && tl < 2.9) dot(path([[2.2, [220, 220]], [2.9, [318, 262]]], tl), C.acc, '', 'online kahan?', C.acc);
  if (tl > 2.9 && tl < 3.6) dot(path([[2.9, [318, 262]], [3.6, [220, 220]]], tl), C.acc, '', 'server-2, server-5', C.acc);
  if (tl > 3.8 && tl < 4.8) { dot(path([[3.8, SR], [4.8, S2]], tl), C.good, '', '1 call', C.good);
                              dot(path([[3.8, SR], [4.8, S5]], tl), C.good, '', '1 call', C.good); }
  if (tl > 4.8) { T(790, 160, '120 ko de diya', { c: C.good, b: 1, size: 13 }); T(790, 330, '80 ko de diya', { c: C.good, b: 1, size: 13 }); }
  if (tl > 5.6) T(430, 360 - 15, 'member-wise nahi, server-wise', { c: C.good, size: 12 });
}
