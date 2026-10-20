// Card 1: Arpan -500 hua, Kirana +500 se pehle server gira -> 500 gayab. Ilaaj: ek transaction, COMMIT se pehle gira = WAL se ulta.
const DUR = 17, FIX = 7.5;
const SV = [190, 185], RA = [348, 150], RK = [348, 220];
let TL = 0;
function setup() {
  wire('M190 185 L348 150'); wire('M190 185 L348 220');
  box(40, 150, 150, 70, 'Payment Svc', 'Arpan -> Kirana');
  box(330, 70, 320, 220, 'SQL DB', '', { stroke: C.acc });
  el('rect', { x: 360, y: 130, width: 260, height: 40, rx: 6, fill: C.bg, stroke: C.line });
  el('rect', { x: 360, y: 200, width: 260, height: 40, rx: 6, fill: C.bg, stroke: C.line });
  el('text', { x: 372, y: 155, 'font-size': 13, fill: C.fg }, ST, 'Arpan');
  el('text', { x: 372, y: 225, 'font-size': 13, fill: C.fg }, ST, 'Kirana');
  box(690, 110, 190, 150, 'WAL (DB ka log)', '', { id: 'wal' });
}
function crash(a, b, msg) {
  if (TL > a && TL < b) el('rect', { x: 40, y: 150, width: 150, height: 70, rx: 8, fill: C.bad, opacity: 0.4 }, DYN);
  if (TL > a) T(115, 140, msg, { c: C.bad, b: 1, size: 12 });
}
function frame(t, fix, tl) {
  TL = tl;
  vis('wal', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: do alag badlaav. Arpan -500 hua, Kirana +500 se pehle server gira';
    if (tl < 1.2) dot(path([[0.3, SV], [1.2, RA]], tl), C.acc, '', '-500', C.acc);
    if (tl >= 1.2) T(610, 155, '-500 kata', { a: 'end', c: C.acc, b: 1 });
    crash(1.8, 3.0, 'server gira');
    if (tl >= 3.2) T(610, 225, '+500 nahi aaya', { a: 'end', c: C.bad, b: 1 });
    if (tl >= 4.2) T(490, 325, 'kata: 500 · juda: 0 -> 500 gayab', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: BEGIN, -500, +500, COMMIT = ek transaction. COMMIT se pehle gira -> WAL se ulta';
  const log = [];
  if (tl < 5.0) {
    if (tl >= 0.2) log.push(['BEGIN', C.fg]);
    if (tl < 1.2) dot(path([[0.4, SV], [1.2, RA]], tl), C.acc, '', '-500', C.acc);
    if (tl >= 1.2) log.push(['Arpan -500', C.acc]);
    if (tl >= 1.2 && tl < 3.6) T(610, 155, '-500 (pakka nahi)', { a: 'end', c: C.acc, b: 1 });
    crash(1.6, 2.6, 'COMMIT se pehle gira');
    if (tl >= 2.8) { log.push(['COMMIT nahi', C.bad]); T(490, 52, 'DB utha, log dekha: COMMIT nahi -> ulta', { c: C.acc, b: 1, size: 12 }); }
    if (tl >= 3.6) { log.push(['-> ROLLBACK', C.good]); T(610, 155, '500 wapas', { a: 'end', c: C.good, b: 1 }); }
    if (tl >= 4.2) T(490, 325, 'kata: 0 · juda: 0 -> kuch nahi khoya', { c: C.good, b: 1, size: 15 });
  } else {
    T(490, 52, 'dobara try: is baar poora', { c: C.muted, b: 1, size: 12 });
    log.push(['BEGIN', C.fg]);
    if (tl < 5.8) dot(path([[5.1, SV], [5.8, RA]], tl), C.acc, '', '-500', C.acc);
    if (tl >= 5.8) { log.push(['Arpan -500', C.acc]); T(610, 155, '-500 kata', { a: 'end', c: C.good, b: 1 }); }
    if (tl >= 5.8 && tl < 6.6) dot(path([[5.8, SV], [6.6, RK]], tl), C.good, '', '+500', C.good);
    if (tl >= 6.6) { log.push(['Kirana +500', C.good]); T(610, 225, '+500 aaya', { a: 'end', c: C.good, b: 1 }); }
    if (tl >= 7.0) { log.push(['COMMIT', C.good]); T(490, 325, 'kata: 500 == juda: 500 -> ya dono ya koi nahi', { c: C.good, b: 1, size: 14 }); }
  }
  log.forEach(([s, c], i) => T(705, 160 + i * 22, s, { a: 'start', c, b: 1, size: 13 }));
}
