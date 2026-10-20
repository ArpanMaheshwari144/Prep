// Card 8: har worker apni 100 / sec -> 100 worker = 100 guna -> 429. Ilaaj: saanjha token bucket (Redis), kul 100 / sec, token nahi = ruko.
const DUR = 18, FIX = 8;
const WY = [55, 135, 215, 305], WN = ['W1', 'W2', 'W3', 'W100'];
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  WY.forEach((y, i) => {
    wire(`M120 ${y} L680 190`, 'd' + i); wire(`M120 ${y} L330 190`, 'f' + i);
    box(30, y - 22, 90, 44, WN[i]);
  });
  el('text', { x: 75, y: 268, 'text-anchor': 'middle', 'font-size': 16, fill: C.muted }, ST, '...');
  wire('M480 190 H680', 'f9');
  box(330, 150, 150, 80, 'Redis bucket', '100 token / sec', { id: 'bk', stroke: C.acc });
  box(680, 150, 180, 80, 'SMS company', 'had 100 / sec');
}
function frame(t, fix, tl) {
  [0, 1, 2, 3].forEach(i => { vis('d' + i, !fix); vis('f' + i, fix); });
  vis('bk', fix); vis('f9', fix);
  let ok = 0, bad = 0;
  if (!fix) {
    CAP.textContent = 'DIKKAT: har worker apni speed 100 / sec. 100 worker = 100 guna, company ne 429 diya';
    for (let w = 0; w < 6; w++) WY.forEach((y, i) => {
      const s = 0.4 + w * 1.1 + i * 0.08, e = s + 0.8, good = i === w % 4;
      dot(mv([120, y], [668, 172 + i * 12], s, e, tl), C.msg);
      if (tl >= e) { good ? ok++ : bad++; if (tl < e + 0.4) dot([668, 172 + i * 12], good ? C.good : C.bad); }
    });
    T(400, 120, 'har worker: apni 100 / sec', { c: C.bad, b: 1, size: 13, halo: 1 });
    T(770, 265, `OK: ${ok}`, { c: C.good, b: 1, size: 14 });
    T(770, 290, `429: ${bad}`, { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: sab worker ka ek saanjha token bucket (Redis). Pehle token lo, mila to bhejo';
  for (let w = 0; w < 7; w++) WY.forEach((y, i) => {
    const s = 0.4 + w * 1.2 + i * 0.08, good = i === w % 4;
    dot(mv([120, y], [318, 180 + i * 6], s, s + 0.5, tl), good ? C.good : C.acc);
    if (tl >= s + 0.5 && tl < s + 1.0) {
      if (good) dot(mv([525, 190], [668, 190], s + 0.5, s + 1.0, tl), C.good, '', 'token mila', C.good);
      else T(160, y + 5, 'ruko', { c: C.acc, b: 1, size: 12, a: 'start', halo: 1 });
    }
    if (good && tl >= s + 1.0) ok++;
  });
  T(405, 260, 'kul 100 / sec', { c: C.good, b: 1, size: 13 });
  T(405, 285, 'token nahi = ruko, phenko nahi', { c: C.acc, size: 12 });
  T(770, 265, `OK: ${ok}`, { c: C.good, b: 1, size: 14 });
  T(770, 290, '429: 0', { c: C.good, b: 1, size: 15 });
}
