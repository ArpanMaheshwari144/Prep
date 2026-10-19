// Card 3: Bieber ka push = 10 crore kaam, mera tweet line me peeche. Ilaaj: celeb pull, padhte waqt milao.
const DUR = 17, FIX = 8;
const BI = [70, 90], ME = [70, 290];
function setup() {
  const fix = grp('fix');
  box(20, 55, 100, 70, 'Bieber', '10 crore fan');
  box(20, 255, 100, 70, 'Main', 'tweet m1');
  box(220, 160, 300, 64, 'Kafka line (fanout kaam)');
  wire('M120 90 L220 180'); wire('M120 290 L220 205');
  box(640, 170, 120, 44, 'inbox');
  wire('M520 192 H640');
  box(220, 35, 160, 64, 'Celeb cache', '', { parent: fix, stroke: C.acc }); wire('M120 70 H220', '', fix);
  box(620, 265, 230, 80, 'Feed = inbox + celeb', '', { parent: fix, stroke: C.good });
}
function frame(t, fix, tl) {
  vis('fix', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: Bieber ka bhi push. Kafka line me 10 crore kaam, mera tweet line me peeche, ghanton';
    const f = Math.min(1, tl / 2);
    el('rect', { x: 226, y: 196, width: 288 * f, height: 16, rx: 3, fill: C.bad }, DYN);
    T(370, 150, `line me: ${Math.round(10 * f)} crore kaam`, { c: C.bad, b: 1, size: 13 });
    dot(path([[1.5, ME], [2.3, [205, 240]]], tl), C.msg, '', tl > 2.3 ? '' : 'm1', C.msg);
    if (tl > 2.3) T(250, 262, 'm1: line ke aakhir me', { c: C.msg, b: 1, size: 12, a: 'start' });
    if (tl > 3) T(370, 300, 'mera follower: "Arpan ka tweet kahan?"', { c: C.bad, size: 12, b: 1 });
    return;
  }
  CAP.textContent = 'ILAAJ: aam = push (inbox), celeb = pull. Bieber ka tweet celeb cache me, feed khule tab milao';
  if (tl < 0.9) dot(path([[0.3, BI], [0.9, [230, 80]]], tl), C.acc, '', 'B1', C.acc); else cells(282, 66, [['B1', C.acc]], { w: 36, h: 22 });
  dot(path([[0.6, ME], [1.2, [230, 205]], [2.0, [510, 192]], [2.5, [660, 192]]], tl < 2.5 ? tl : -1), C.msg, '', 'm1');
  T(370, 240, 'line khaali, sirf aam tweet', { c: C.good, size: 12 });
  const inbox = tl >= 2.5 ? [['m1', C.good], ['r4', C.fg]] : [['r4', C.fg]];
  cells(660, 135, inbox, { w: 36, h: 22 });
  if (tl > 3.2) {
    T(735, 250, 'feed kholi: time se milao', { c: C.muted, size: 12 });
    const items = [['B1', C.acc], ['m1', C.good], ['r4', C.fg]];
    cells(640, 300, items.slice(0, Math.min(3, Math.floor((tl - 3.2) / 0.5) + 1)), { w: 60, h: 26 });
  }
}
