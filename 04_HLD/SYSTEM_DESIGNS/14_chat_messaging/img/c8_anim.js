// Card 8: har message x har member ka "padha" record: 50 x 500 = 25,000 likhai. Ilaaj: read_upto = 4467, ek likhai.
const DUR = 18, FIX = 8;
const RV = [150, 190], SL = [208, 190], SR = [350, 190], NSP = [418, 75], NSR = [650, 75], AP = [688, 250];
function setup() {
  wire('M150 190 H220');
  box(20, 150, 130, 80, 'Ravi ka phone', '50 padhe');
  box(220, 150, 130, 80, 'Chat Server');
  wire('M350 190 L430 190', 'wrec');
  box(430, 30, 450, 310, '"padha" record', 'har message x har member', { id: 'rec' });
  wire('M350 170 L430 85', 'wns'); wire('M650 85 L760 210', 'wap');
  box(430, 40, 220, 80, 'Nishaan store', 'Ravi: read_upto', { id: 'ns', stroke: C.acc });
  box(700, 210, 180, 130, 'Arpan ka phone', 'meri chat', { id: 'ap' });
}
function frame(t, fix, tl) {
  vis('rec', !fix); vis('wrec', !fix);
  ['ns', 'wns', 'wap', 'ap'].forEach(i => vis(i, fix));
  dot(path([[0.3, RV], [0.9, SL]], tl < 0.9 ? tl : -1), C.msg, '', 'padh liya');
  if (!fix) {
    CAP.textContent = 'DIKKAT: har message x har member ka "padha" record: 50 x 500 = 25,000 likhai';
    const f = Math.max(0, Math.min(1, (tl - 1.2) / 4.5)), n = Math.round(f * 25000);
    for (let k = 0; k < 6; k++) {
      const s = 1.2 + k * 0.7;
      if (tl > s && tl < s + 0.6 && f < 1) dot(path([[s, SR], [s + 0.6, [445, 100 + k * 30]]], tl), C.bad);
    }
    const cellsN = Math.round(f * 500);
    for (let i = 0; i < cellsN; i++) {
      const x = 455 + (i % 34) * 12, y = 80 + Math.floor(i / 34) * 12;
      el('rect', { x, y, width: 10, height: 10, rx: 1, fill: C.bad }, DYN);
    }
    T(655, 285, `likhai: ${n.toLocaleString('en-IN')}`, { c: C.bad, b: 1, size: 15 });
    T(655, 310, '(har dabba = 50 record)', { c: C.muted, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: har member ka ek number, kahan tak padha (read_upto). 50 padhe -> 4467, ek likhai';
  if (tl > 0.9 && tl < 1.6) dot(path([[0.9, SR], [1.6, NSP]], tl), C.acc, '', 'read_upto 4467', C.acc);
  if (tl > 1.6) { cells(470, 130, [['Ravi: 4467', C.good]], { w: 140, h: 26 }); T(540, 180, 'likhai: 1', { c: C.good, b: 1, size: 15, halo: 1 }); }
  if (tl > 2.4 && tl < 3.4) dot(path([[2.4, NSR], [3.4, AP]], tl), C.msg, '', 'push: read_upto 4467');
  const blue = tl > 3.4;
  cells(712, 260, [['4418', blue ? C.msg : C.muted], ['..', blue ? C.msg : C.muted], ['4467', blue ? C.msg : C.muted]], { w: 48, h: 24 });
  T(790, 305, blue ? '50 ek saath neeli' : 'tick abhi grey', { c: blue ? C.msg : C.muted, b: 1, size: 12 });
  if (tl > 4.4) T(260, 300, 'unread = aakhri seq - nishaan', { c: C.good, size: 13 });
  if (tl > 4.4) T(260, 322, 'ghatao, gino nahi', { c: C.muted, size: 12 });
}
