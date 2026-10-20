// Card 10: "sabko bhejo" = ek request me 1 crore ek-ek, ghanton atki, OTP peeche. Ilaaj: 1000 ke tukde low line me, campaign row me aakhri_id.
const DUR = 18, FIX = 8;
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  wire('M120 185 H170'); wire('M320 185 H760', 'd0');
  wire('M320 190 H630', 'f0'); wire('M730 185 H760', 'f1'); wire('M245 220 V280', 'f2'); wire('M320 310 L680 215', 'f3');
  box(20, 150, 100, 70, 'Admin', 'sabko bhejo');
  box(170, 150, 150, 70, 'Notification Svc', '', { id: 'ns' });
  box(170, 150, 150, 70, 'Campaign Svc', '', { id: 'cs', stroke: C.acc });
  box(170, 280, 150, 60, 'Campaign row', '', { id: 'db', stroke: C.acc });
  box(630, 155, 100, 60, 'Worker', 'low line', { id: 'wk' });
  box(760, 150, 110, 70, 'Users', '1 crore');
}
function frame(t, fix, tl) {
  vis('ns', !fix); vis('d0', !fix);
  ['cs', 'db', 'wk', 'f0', 'f1', 'f2', 'f3'].forEach(i => vis(i, fix));
  dot(mv([120, 185], [158, 185], 0.3, 0.8, tl), C.msg, '', 'ek request', C.msg);
  if (!fix) {
    CAP.textContent = 'DIKKAT: "sabko bhejo" = ek request me 1 crore ek-ek. Ghanton atki, OTP bhi peeche';
    for (let k = 0; k < 24; k++) dot(mv([320, 185], [748, 185], 1.0 + k * 0.25, 1.6 + k * 0.25, tl), C.acc);
    T(540, 250, `bheje: ${Math.max(0, Math.round((tl - 1) * 1300)).toLocaleString('en-IN')} / 1,00,00,000`, { c: C.acc, b: 1, size: 13 });
    if (tl > 3) T(540, 278, 'ghanton lagenge', { c: C.bad, b: 1, size: 13 });
    if (tl > 1.5) { dot([245, 120], C.bad, 'OTP'); T(245, 92, 'OTP peeche atka', { c: C.bad, b: 1, size: 12 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: 1000-1000 ke tukde, low line me. Har tukde ke baad campaign row me aakhri_id';
  const sent = [1.2, 2.4, 3.6, 7.0, 8.2, 9.4];
  const done = sent.filter(s => tl >= s).length;
  const left = ['1k', '2k', '3k', '4k', '5k', '6k'].slice(done);
  if (left.length) cells(620 - left.length * 44, 177, left.map(s => [s, C.acc]), { w: 40 });
  T(470, 160, 'tukde (1000 user)', { c: C.muted, size: 12 });
  sent.forEach(s => dot(mv([730, 185], [748, 185], s - 0.3, s, tl), C.acc));
  const down = tl > 4.0 && tl < 5.6;
  if (down) { el('rect', { x: 630, y: 155, width: 100, height: 60, rx: 8, fill: C.bad, opacity: 0.35 }, DYN);
    T(680, 240, 'worker gira', { c: C.bad, b: 1, size: 13 }); }
  dot(mv([320, 310], [672, 222], 5.8, 6.6, tl), C.good, '', 'wahin se: 3001', C.good);
  T(245, 326, `aakhri_id = ${Math.min(done, 6) * 1000}`, { c: C.acc, b: 1, size: 12 });
  dot(mv([400, 60], [748, 165], 0.6, 1.6, tl), C.good, '', 'OTP (tez line)', C.good);
  if (tl > 1.6) T(560, 60, 'OTP turant', { c: C.good, b: 1, size: 13 });
}
