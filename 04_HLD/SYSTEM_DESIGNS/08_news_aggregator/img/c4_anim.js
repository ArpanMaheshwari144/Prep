// Card 4: 5 source, wahi khabar 5 baar. Ilaaj: URL Bloom + title hash. Agar: wins / won -> SimHash.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: "India ne match jeeta" 5 source se. Har naya URL nayi khabar, feed me 5 baar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: URL Bloom se. Title chhote akshar, chinh hata ke hash. Hash wahi = khabar wahi'],
  [6, YEL, 'AGAR', 'AGAR "wins" aur "won", hash alag? -> SimHash: 90% same to duplicate'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.msg, '5 source → Worker', 'Wahi khabar 5 source se aayi, 5 alag URL ke saath'], [C.bad, 'Worker → Feed', 'Worker ne har naye URL ko nayi khabar maana'], [C.bad, 'Feed', 'Feed me "India ne match jeeta" 5 baar dikha']],
  [[C.acc, 'Worker → Bloom', 'Worker ne Bloom filter se dekha ye URL pehli baar aaya?'], [C.acc, 'Worker → Title hash', 'Phir title chhote akshar aur bina chinh karke hash nikala'], [C.good, 'Title hash → Worker', 'Ye hash pehle se tha, matlab duplicate, skip kiya'], [C.good, 'Feed', 'Feed me khabar sirf ek baar, pehli wali']],
  [[C.bad, 'Exact hash', '"wins" ki jagah "won", ek shabd badla to hash poora alag'], [C.good, 'SimHash', 'SimHash me 64 me se 59 bit same, matlab 92% milte'], [C.good, 'SimHash → Feed', '90% se upar to duplicate maana, feed me ek hi']],
];
function frame(ph) {
  if (ph === 0) {
    ['NDTV', 'ABP', 'TOI', 'HT', 'BBC'].forEach((s, i) => B(20, 5 + i * 64, 130, 52, s, ''));
    B(320, 120, 200, 80, 'Worker', 'naya URL = naya', { st: 'bad' });
    B(640, 60, 230, 200, 'Feed', '', { st: 'bad' });
    for (let i = 0; i < 5; i++) { A(150, 31 + i * 64, 318, 160, C.msg, i === 2 ? { n: 1 } : {});
      cells(655, 100 + i * 30, [['India ne match jeeta', C.bad]], { w: 200, h: 24 }); }
    A(520, 160, 638, 160, C.bad, { n: 2 });
    num(755, 290, 3, C.bad); L(775, 295, '5 baar', C.bad, 'start');
    L(450, 345, 'URL alag the, khabar ek', C.bad);
  } else if (ph === 1) {
    B(20, 130, 170, 80, 'Worker', 'naya title');
    B(330, 20, 260, 70, 'Bloom (URL)', 'pehli baar? haan');
    B(330, 160, 260, 80, 'Title hash', 'h7f3a pehle se', { st: 'acc' });
    B(700, 130, 170, 80, 'Feed', 'ek hi baar', { st: 'good' });
    A(190, 150, 328, 60, C.acc, { n: 1 });
    A(190, 180, 328, 185, C.acc, { n: 2 });
    A(328, 220, 192, 200, C.good, { n: 3, at: 0.6 });
    A(590, 200, 698, 175, C.good, { n: 4 }); L(640, 225, 'sirf pehli', C.good);
    T(450, 290, '"India WINS the match!" -> india wins the match -> h7f3a', { c: C.fg, size: 13 });
    L(450, 345, 'URL alag ho tab bhi, title ka hash wahi to khabar wahi', C.good);
  } else {
    B(20, 30, 380, 70, 'Title A', 'India wins the match');
    B(20, 200, 380, 70, 'Title B', 'India won the match');
    B(500, 20, 370, 70, 'Exact hash', 'h7f3a  !=  h91c2', { st: 'bad' });
    B(500, 140, 370, 70, 'SimHash', '59 / 64 bit same = 92%', { st: 'good' });
    B(500, 265, 370, 70, 'Feed', 'ek hi khabar', { st: 'good' });
    A(400, 60, 498, 55, C.bad, { n: 1 }); A(400, 230, 498, 75, C.bad);
    A(400, 75, 498, 165, C.good, { n: 2, at: 0.7 }); A(400, 220, 498, 185, C.good);
    A(685, 212, 685, 263, C.good, { n: 3 });
  }
}
