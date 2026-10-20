// Card 8: pakka S3 link WhatsApp pe, hamesha chalta. Ilaaj: DB me sirf key, har baar naya 5 min ka link. Agar: download adhoora.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine S3 ka pakka link WhatsApp group pe bhej diya. Wo hamesha chalta, koi bhi khole'],
  [3, GRN, 'ILAAJ', 'ILAAJ: DB me sirf S3 key. Pehle maalik check, phir naya 5 min ka link (signed GET)'],
  [6, YEL, 'AGAR', 'AGAR 5 min me download adhoora? -> chalu download beech me nahi katta'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
const STEPS = [
  [[C.msg, 'Arpan → WhatsApp group', 'Arpan ne S3 ka pakka link|group me bhej diya'],
   [C.bad, 'Group → koi bhi', 'link aage share hota raha,|6 mahine baad bhi koi click karta'],
   [C.bad, 'Koi bhi → S3', 'jo bhi link kholta hai,|seedha S3 tak pahunchta'],
   [C.bad, 'S3 → koi bhi', 'report.pdf mil gayi. Link kabhi expire|nahi, wo S3 ka pakka pata hai']],
  [[C.msg, 'Arpan → Download Svc', 'download maanga. Pehle check:|T-101 ka maalik Arpan hi hai?'],
   [C.acc, 'Download Svc → Arpan', 'DB me sirf S3 key. Usse naya|5 min ka signed link banaya'],
   [C.good, 'S3 → Arpan', 'sign sahi, 5 min ke andar:|report.pdf mil gayi'],
   [C.bad, 'S3 → group me koi', '1 ghanta baad wahi link khola:|umar khatam, 403']],
  [[C.good, '4 min pe shuru', 'link chalu tha, download shuru hua.|9 min tak chala, poora mila'],
   [C.bad, '6 min pe naya shuru', 'link ki umar khatam: 403.|Umar sirf shuru karne ki hai']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 200, 76, 'Arpan', 'pakka link', { icon: 'user', st: 'acc', stroke: C.msg });
    B(320, 120, 230, 76, 'WhatsApp group', '200 log', { icon: 'chat', st: 'good' });
    B(670, 10, 210, 76, 'S3', 'report.pdf', { icon: 'bucket', st: 'acc', stroke: C.msg });
    B(670, 230, 210, 76, 'Koi bhi', '6 mahine baad', { icon: 'users', st: 'bad' });
    A(220, 158, 318, 158, C.msg, { n: 1 });
    A(550, 175, 668, 255, C.bad, { n: 2 }); L(575, 245, 'aage gaya', C.bad, 'end');
    A(740, 228, 740, 88, C.bad, { n: 3 }); L(725, 160, 'khola', C.bad, 'end');
    A(820, 88, 820, 228, C.bad, { n: 4, dash: 1 }); L(835, 160, 'file', C.bad, 'start');
    L(450, 345, 'link me koi umar nahi. File sabke haath, hamesha ke liye', C.bad);
  } else if (ph === 1) {
    panel(0, 'maanga', ['Arpan', 'T-101', ''], ['Download Svc', 'maalik? haan', 'acc'], 'maanga', true, C.msg, 'pehle owner check');
    panel(1, 'naya link', ['Arpan', 'link, 5 min', 'good'], ['Download Svc', 'DB se S3 key', 'acc'], 'link', false, C.acc, 'har baar naya');
    panel(2, 'download', ['Arpan', 'report.pdf', 'good'], ['S3', 'sign sahi', 'good'], 'file', false, C.good, '5 min ke andar');
    panel(3, '1 ghanta baad', ['Group me koi', 'wahi link', 'bad'], ['S3', 'umar khatam', 'bad'], '403', false, C.bad, 'link aage gaya, bekaar');
    L(450, 330, 'pakka link kabhi bahar nahi jaata, sirf 5 min wala', C.good);
  } else {
    el('line', { x1: 80, y1: 170, x2: 820, y2: 170, stroke: C.line, 'stroke-width': 2 }, DYN);
    [[80, '0 min'], [450, '5 min: umar khatam'], [820, '10 min']].forEach(([x, t]) => T(x, 200, t, { c: C.muted, size: 14 }));
    el('line', { x1: 450, y1: 110, x2: 450, y2: 185, stroke: C.acc, 'stroke-width': 2.5 }, DYN);
    el('rect', { x: 376, y: 125, width: 370, height: 22, rx: 5, fill: C.good }, DYN);
    num(376, 100, 1, C.good); L(400, 106, '4 se 9 min: poora chala', C.good, 'start');
    el('rect', { x: 524, y: 245, width: 60, height: 22, rx: 5, fill: C.bad }, DYN);
    num(524, 290, 2, C.bad); L(548, 296, '6 min pe naya: 403', C.bad, 'start');
    L(450, 345, 'umar sirf shuru karne ki. Chalu download beech me nahi katta', C.good);
  }
}
