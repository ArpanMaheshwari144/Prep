// Card 6: kram phone ki ghadi se, meri 10:05 (asal 10:00) -> kram ulta. Ilaaj: server ka +1 (Redis INCR seq:chat123).
const DUR = 18, FIX = 8;
const AP = [170, 75], RP = [170, 275], SL = [288, 180], SR = [440, 180], SB = [370, 220], SQ = [370, 268], SC = [588, 180];
function setup() {
  wire('M170 75 L300 165'); wire('M170 275 L300 195'); wire('M440 180 H600'); wire('M370 220 V280', 'ws');
  box(20, 40, 150, 70, 'Arpan ka phone', 'ghadi 10:05');
  box(20, 240, 150, 70, 'Ravi ka phone', 'ghadi sahi');
  box(300, 140, 140, 80, 'Chat Server');
  box(300, 280, 140, 60, 'Redis', 'INCR seq:chat123', { id: 'sq', stroke: C.acc });
  box(600, 40, 270, 290, 'Chat ki screen');
}
function row(i, who, txt, tag, c) {
  const y = 95 + i * 60;
  el('rect', { x: 620, y, width: 230, height: 42, rx: 6, fill: C.bg, stroke: c, 'stroke-width': 1.5 }, DYN);
  T(632, y + 26, `${who}: ${txt}`, { a: 'start', size: 13, c: C.fg });
  T(838, y + 26, tag, { a: 'end', size: 12, c, b: 1 });
}
function frame(t, fix, tl) {
  vis('sq', fix); vis('ws', fix);
  T(95, 130, 'asal 10:00 pe bheja', { c: C.muted, size: 12 });
  T(95, 330, 'asal 10:02 pe jawab', { c: C.muted, size: 12 });
  const m1 = path([[0.4, AP], [1.2, SL]], tl < 1.2 ? tl : -1), m2 = path([[2.4, RP], [3.2, SL]], tl < 3.2 ? tl : -1);
  dot(m1, C.msg, '', 'kal milein?'); dot(m2, C.acc, '', 'haan', C.acc);
  if (tl > 1.2 && tl < 2.0) dot(path([[1.2, SR], [2.0, SC]], tl), C.msg);
  if (tl > 3.2 && tl < 4.0) dot(path([[3.2, SR], [4.0, SC]], tl), C.acc);
  const a = tl > 2.0, r = tl > 4.0;
  if (!fix) {
    CAP.textContent = 'DIKKAT: kram phone ki ghadi se. Meri ghadi 10:05 (asal 10:00) -> Ravi ka "haan" upar, kram ulta';
    if (a && !r) row(0, 'Arpan', 'kal milein?', '10:05', C.msg);
    if (r) { row(0, 'Ravi', 'haan', '10:02', C.bad); row(1, 'Arpan', 'kal milein?', '10:05', C.bad); }
    if (tl > 4.4) T(735, 290, 'jawab sawaal se pehle: ulta', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: har chat ka server pe +1 number (Redis INCR seq:chat123). Kram isi se, ghadi nahi';
  if (tl > 1.2 && tl < 1.7) dot(path([[1.2, SB], [1.7, SQ]], tl), C.acc, '', '+1', C.acc);
  if (tl > 1.7) T(450, 300, 'kal milein? = 4415', { a: 'start', c: C.acc, size: 12, b: 1 });
  if (tl > 3.2 && tl < 3.7) dot(path([[3.2, SB], [3.7, SQ]], tl), C.acc, '', '+1', C.acc);
  if (tl > 3.7) T(450, 320, 'haan = 4416', { a: 'start', c: C.acc, size: 12, b: 1 });
  if (a) row(0, 'Arpan', 'kal milein?', 'seq 4415', C.good);
  if (r) row(1, 'Ravi', 'haan', 'seq 4416', C.good);
  if (tl > 4.4) T(735, 290, 'kram sahi, ghadi se farak nahi', { c: C.good, b: 1, size: 13 });
}
