// Card 3: pehle bhejte, na pahunche to DB; DB se pehle server gira -> message gaya. Ilaaj: pehle DB, phir bhejo + push.
const DUR = 18, FIX = 7.5;
const AR = [120, 180], SL = [218, 180], SR = [370, 180], SB = [300, 220], DB = [300, 268], PH = [668, 180],
  PS = [545, 105], PSO = [610, 70], PHT = [760, 128];
function setup() {
  wire('M120 180 H230'); wire('M370 180 H680'); wire('M300 220 V280');
  wire('M370 165 L480 85', 'wp'); wire('M610 70 L760 140', 'wp2');
  box(20, 150, 100, 60, 'Arpan');
  box(230, 140, 140, 80, 'Chat Server');
  box(230, 280, 140, 60, 'Message DB');
  box(480, 40, 130, 60, 'Push', 'FCM / APNs', { id: 'ps', stroke: C.acc });
  box(680, 140, 190, 80, 'Ravi ka phone', '', { id: 'ph' });
}
function crashBox(on) {
  if (!on) return;
  el('rect', { x: 230, y: 140, width: 140, height: 80, rx: 8, fill: C.bad, opacity: 0.35 }, DYN);
  T(300, 130, 'server gira', { c: C.bad, b: 1, size: 13, halo: 1 });
}
function frame(t, fix, tl) {
  vis('ps', fix); vis('wp', fix); vis('wp2', fix);
  const back = fix && tl > 6.2;
  T(775, 200, back ? 'wapas on' : 'band', { c: back ? C.good : C.bad, b: 1, size: 13 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: pehle bhejo, na pahunche to DB. Par DB me likhne se pehle server gira -> "hi" gaya';
    dot(path([[0.4, AR], [1.2, SL]], tl < 1.2 ? tl : -1), C.msg, '', 'hi');
    if (tl > 1.4 && tl < 2.4) dot(path([[1.4, SR], [2.4, PH]], tl), C.msg, '', 'hi');
    if (tl > 2.4) T(560, 165, 'nahi pahuncha', { c: C.bad, size: 12, b: 1 });
    if (tl > 2.8 && tl < 3.2) T(300, 250, 'ab DB me likhta...', { c: C.muted, size: 12 });
    crashBox(tl > 3.2);
    T(450, 312, tl > 3.6 ? 'DB: khaali' : '', { c: C.bad, b: 1, size: 13, a: 'start' });
    if (tl > 4.2) T(640, 280, '"hi" gaya, kahin nahi', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: pehle DB me likho, phir bhejo. Phone band to push ghanti. Wapas aaya: "aakhri ke baad sab"';
  dot(path([[0.4, AR], [1.2, SL]], tl < 1.2 ? tl : -1), C.msg, '', 'hi');
  if (tl > 1.2 && tl < 1.9) dot(path([[1.2, SB], [1.9, DB]], tl), C.acc, '', 'likho', C.acc);
  if (tl > 1.9) cells(390, 298, [['hi', C.good]], { w: 40, h: 26 });
  if (tl > 1.9) T(440, 316, 'DB me hai', { c: C.good, size: 12, a: 'start' });
  if (tl > 2.1 && tl < 3.0) dot(path([[2.1, SR], [3.0, PH]], tl), C.msg, '', 'hi');
  if (tl > 3.0 && tl < 3.8) dot(path([[3.0, [380, 160]], [3.8, PS]], tl), C.acc);
  if (tl > 3.8 && tl < 4.5) dot(path([[3.8, PSO], [4.5, PHT]], tl), C.acc, '', 'ghanti', C.acc);
  if (tl > 4.5) T(775, 125, 'ghanti: "hi"', { c: C.acc, size: 12, b: 1 });
  crashBox(tl > 4.8 && tl < 6.0);
  if (tl > 4.8) T(560, 320, 'server gira, par "hi" DB me', { c: C.good, b: 1, size: 13, a: 'start' });
  if (tl > 6.6 && tl < 7.4) dot(path([[6.6, PH], [7.4, SR]], tl), C.acc, '', 'aakhri ke baad sab do', C.acc);
  if (tl > 7.4 && tl < 8.2) dot(path([[7.4, [370, 300]], [8.2, PH]], tl), C.good, '', 'hi', C.good);
  if (tl > 8.2) T(775, 250, '"hi" mila', { c: C.good, b: 1, size: 14 });
}
