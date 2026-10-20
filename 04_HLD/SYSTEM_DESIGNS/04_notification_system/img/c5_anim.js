// Card 5: SMS company pehle se hili, 50 worker har second turant dobara maar rahe. Ilaaj: backoff (1, 2, 4, 8 sec)
// + jitter, had ke baad DLQ + alert. Agar: DLQ me pahunchi -> dashboard, theek karke main line me wapas.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: SMS company pehle se hili. 50 worker fail pe turant dobara, har second, ek saath'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har baar zyada ruko (1, 2, 4, 8 sec) + thoda random (jitter). Had paar: DLQ'],
  [6, YEL, 'AGAR', 'AGAR khabar DLQ me pahunch gayi, ab uska kya? -> dashboard, theek karo, main line me wapas'],
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
  [[C.bad, '50 worker → SMS company', '50 worker fail hote hi har second ek saath retry karte, company theek hi nahi ho paati']],
  [[C.good, 'har baar dugna ruko', 'Har fail ke baad dugna ruko: 1, 2, 4, 8 sec'], [C.good, 'random (jitter)', 'Upar se thoda random waqt, taaki do worker ek pal me na lautein'], [C.acc, 'Worker → DLQ', 'Kai baar fail ho to khabar DLQ me gayi aur alert bhi baja']],
  [[C.acc, 'Worker → DLQ', 'Had paar hui to khabar DLQ me daali, phenki nahi'], [C.acc, 'DLQ → Dashboard', 'DLQ ki khabar dashboard pe on-call ko dikhi'], [C.good, 'On-call → Dashboard', 'On-call ne wajah dekhi (number galat tha) aur theek kiya'], [C.good, 'DLQ → Main line', 'k7 wapas main line me gayi, is baar SMS chala gaya']],
];
function frame(ph) {
  if (ph === 0) {
    el('line', { x1: 60, y1: 240, x2: 640, y2: 240, stroke: C.line, 'stroke-width': 2 }, DYN);
    [0, 1, 2, 3, 4].forEach(s => {
      const x = 100 + s * 125; T(x, 265, s + ' sec', { c: C.muted, size: 12 });
      dots(10, x - 30, 90, C.bad, { cols: 5, gap: 13, r: 5 }); T(x, 125, '50 call', { c: C.bad, size: 11 });
      A(x, 135, x, 232, C.bad);
    });
    B(700, 150, 170, 80, 'SMS company', 'hili hui', { st: 'bad' });
    num(785, 260, 1, C.bad); L(785, 285, 'har second hathoda', C.bad);
    L(450, 330, 'company theek ho hi nahi paati, hum hi use giraaye rakhte', C.bad);
  } else if (ph === 1) {
    el('line', { x1: 60, y1: 120, x2: 620, y2: 120, stroke: C.line, 'stroke-width': 2 }, DYN);
    el('line', { x1: 60, y1: 230, x2: 620, y2: 230, stroke: C.line, 'stroke-width': 2 }, DYN);
    T(60, 100, 'Worker-A', { c: C.fg, b: 1, size: 13, a: 'start' }); T(60, 210, 'Worker-B', { c: C.fg, b: 1, size: 13, a: 'start' });
    const X = s => 80 + s * 30;
    [[1.0, 2.2, 4.1, 8.3], [1.4, 2.6, 4.8, 8.9]].forEach((r, i) => r.forEach((s, k) => {
      dot([X(s), 120 + i * 110], C.acc); T(X(s), 145 + i * 110, s + 's', { c: C.acc, size: 11 });
    }));
    num(200, 175, 1, C.good); L(220, 180, 'har baar dugna ruko', C.good, 'start');
    num(420, 175, 2, C.good); L(440, 180, 'random: dono ek pal me nahi', C.good, 'start');
    B(690, 140, 180, 80, 'DLQ', 'had paar wali khabar', { st: 'acc' });
    A(620, 230, 688, 195, C.acc, { n: 3 }); L(780, 245, 'alert bhi', C.acc);
    L(450, 330, 'company ko saans milti. Khabar phenki nahi jaati', C.good);
  } else {
    panel(0, 'DLQ me', ['Worker', 'had paar', ''], ['DLQ', 'khabar k7', 'acc'], 'daala', true, C.acc, 'phenka nahi');
    panel(1, 'alert', ['DLQ', 'k7', 'acc'], ['Dashboard', 'on-call ko dikha', ''], 'dikhaya', true, C.acc, 'insaan dekhta');
    panel(2, 'theek kiya', ['On-call', 'number galat tha', ''], ['Dashboard', 'k7 theek', 'good'], 'sudhaara', true, C.good, 'wajah hatayi');
    panel(3, 'wapas', ['DLQ', 'k7', ''], ['Main line', 'k7 phir chali', 'good'], 'wapas', true, C.good, 'SMS gaya');
    L(450, 320, 'DLQ kachra-dabba nahi, "dekhna padega" wali line hai', C.good);
  }
}
