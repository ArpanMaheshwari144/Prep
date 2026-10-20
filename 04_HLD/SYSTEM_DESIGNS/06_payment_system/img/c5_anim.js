// Card 5: PSP ko call, jawab se pehle server gira, DB me kuch nahi. Ilaaj: pehle PENDING, phir webhook ya reconciliation job.
const DUR = 18, FIX = 7.5;
const SV = [190, 75], PS = [688, 75], PSL = [700, 75], WH0 = [780, 110], DBE = [190, 270];
const JB = [540, 250], PSB = [760, 112], JL = [400, 285], DBR = [202, 285];
let TL = 0;
function setup() {
  wire('M190 75 H700'); wire('M115 110 V230');
  wire('M780 110 L190 270', 'wh'); wire('M540 250 L760 110', 'wj1'); wire('M400 285 H190', 'wj2');
  box(40, 40, 150, 70, 'Payment Svc');
  box(700, 40, 160, 70, 'PSP');
  box(40, 230, 150, 80, 'SQL DB', 'payment ka haal');
  box(400, 250, 180, 70, 'Reconciliation', 'job', { id: 'job' });
}
function crash(a, b) {
  if (TL > a && TL < b) el('rect', { x: 40, y: 40, width: 150, height: 70, rx: 8, fill: C.bad, opacity: 0.4 }, DYN);
  if (TL > a) T(115, 28, 'server gira', { c: C.bad, b: 1, size: 12 });
}
function frame(t, fix, tl) {
  TL = tl;
  ['wh', 'wj1', 'wj2', 'job'].forEach(id => vis(id, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: PSP ko call gayi, jawab se pehle server gira. DB me kuch nahi: paisa gaya ya nahi?';
    if (tl < 1.0) dot(path([[0.3, SV], [1.0, PS]], tl), C.msg, '', 'call');
    if (tl >= 1.2) T(780, 28, 'paisa kata', { c: C.acc, b: 1, size: 12 });
    if (tl > 1.2 && tl < 1.7) dot(path([[1.2, PSL], [2.0, SV]], tl), C.good, '', 'hua');
    crash(1.7, 2.8);
    if (tl >= 2.4) T(115, 296, 'kuch likha nahi', { c: C.bad, b: 1, size: 12 });
    if (tl >= 3.2) T(490, 210, 'paisa gaya ya nahi? pata nahi', { c: C.bad, b: 1, size: 16 });
    return;
  }
  CAP.textContent = 'ILAAJ: call se PEHLE PENDING. PSP bataye (webhook) ya job poochhe, jo bhi -> SUCCESS';
  let st = '', sc = C.acc;
  if (tl < 0.8) dot(path([[0.2, [115, 110]], [0.8, [115, 218]]], tl), C.acc, '', 'PENDING', C.acc);
  if (tl >= 0.8) { st = 'PENDING'; }
  if (tl > 1.0 && tl < 1.7) dot(path([[1.0, SV], [1.7, PS]], tl), C.msg, '', 'call');
  if (tl >= 1.8) T(780, 28, 'paisa kata', { c: C.acc, b: 1, size: 12 });
  crash(1.9, 2.8);
  const lostAt = 0.45, WX = lerp(WH0, DBE, lostAt);
  if (tl > 3.0 && tl < 3.8) dot(lerp(WH0, WX, (tl - 3.0) / 0.8), C.good, '', 'webhook');
  if (tl >= 3.8) { cross(WX[0], WX[1]); T(WX[0], WX[1] - 22, 'webhook kho gaya', { c: C.bad, b: 1, size: 12, halo: 1 }); }
  if (tl > 4.2 && tl < 5.0) dot(path([[4.2, JB], [5.0, PSB]], tl), C.acc, '', 'haal?', C.acc);
  if (tl > 5.0 && tl < 5.8) dot(path([[5.0, PSB], [5.8, JB]], tl), C.good, '', 'hua', C.good);
  if (tl > 5.9 && tl < 6.5) dot(path([[5.9, JL], [6.5, DBR]], tl), C.good, '', 'SUCCESS', C.good);
  if (tl >= 6.5) { st = 'SUCCESS'; sc = C.good; }
  if (st) T(115, 296, st, { c: sc, b: 1, size: 13 });
  if (tl >= 6.8) T(490, 345, 'PENDING pehle likha -> server gira tab bhi tracking bacha', { c: C.muted, size: 12 });
}
