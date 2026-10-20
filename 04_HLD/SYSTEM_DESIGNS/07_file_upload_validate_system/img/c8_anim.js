// Card 8: pakka S3 link WhatsApp pe, hamesha chalta. Ilaaj: DB me sirf key, har baar naya 5 min ka link. Agar: download adhoora.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine S3 ka pakka link WhatsApp group pe bhej diya. Wo hamesha chalta, koi bhi khole'],
  [3, GRN, 'ILAAJ', 'ILAAJ: DB me sirf S3 key. Download pe pehle maalik check, phir naya 5 min ka link (signed GET)'],
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
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan', 'pakka link');
    B(300, 130, 180, 70, 'WhatsApp group', '200 log', { st: 'bad' });
    B(700, 40, 170, 70, 'S3', 'report.pdf');
    B(700, 230, 170, 70, 'Koi bhi', '6 mahine baad', { st: 'bad' });
    A(170, 165, 298, 165, C.msg, { n: 1 });
    A(480, 185, 698, 255, C.bad, { n: 2 }); L(560, 245, 'aage gaya', C.bad, 'end');
    A(785, 228, 785, 112, C.bad, { n: 3 }); L(800, 170, 'khola', C.bad, 'start');
    A(700, 90, 482, 150, C.bad, { n: 4, dash: 1 }); L(590, 100, 'file, hamesha', C.bad);
    L(450, 340, 'link me koi umar nahi. File sabke haath, hamesha ke liye', C.bad);
  } else if (ph === 1) {
    panel(0, 'maanga', ['Arpan', 'download T-101', ''], ['Download Svc', 'maalik? haan', 'acc'], 'maanga', true, C.msg, 'pehle owner check');
    panel(1, 'naya link', ['Arpan', 'link, 5 min', 'good'], ['Download Svc', 'DB se S3 key', 'acc'], 'signed GET', false, C.acc, 'har baar naya');
    panel(2, 'download', ['Arpan', 'report.pdf', 'good'], ['S3', 'sign sahi', 'good'], 'file', false, C.good, '5 min ke andar');
    panel(3, '1 ghanta baad', ['Group me koi', 'wahi link', 'bad'], ['S3', 'umar khatam', 'bad'], '403', false, C.bad, 'link aage gaya, bekaar');
    L(450, 320, 'pakka link kabhi bahar nahi jaata, sirf 5 min wala', C.good);
  } else {
    el('line', { x1: 80, y1: 160, x2: 820, y2: 160, stroke: C.line, 'stroke-width': 2 }, DYN);
    [[80, '0 min'], [450, '5 min: link ki umar khatam'], [820, '10 min']].forEach(([x, t]) => T(x, 190, t, { c: C.muted, size: 12 }));
    el('line', { x1: 450, y1: 110, x2: 450, y2: 175, stroke: C.acc, 'stroke-width': 2 }, DYN);
    el('rect', { x: 376, y: 120, width: 370, height: 18, rx: 4, fill: C.good }, DYN);
    num(376, 105, 1, C.good); L(396, 110, '4 min pe shuru, 9 min tak chala: poora', C.good, 'start');
    el('rect', { x: 524, y: 230, width: 60, height: 18, rx: 4, fill: C.bad }, DYN);
    num(524, 270, 2, C.bad); L(544, 275, '6 min pe naya shuru: 403', C.bad, 'start');
    L(450, 340, 'umar sirf shuru karne ki hai. Chalu download beech me nahi katta', C.good);
  }
}
