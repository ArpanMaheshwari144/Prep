// Card 5: 2 lakh log seedha DB pe, ek hi rows ke lock pe ruke. Ilaaj: waiting room (~3000 andar) + Kafka (key = show_id) + worker.
const DUR = 18, FIX = 8;
const U = [130, 185], WL = [168, 185], WR = [310, 185], AL = [348, 185];
function setup() {
  wire('M130 185 H360'); wire('M460 185 H720');
  wire('M410 220 V300 H500', 'wk'); wire('M630 300 H720', 'ww'); wire('M795 275 V230', 'wd');
  box(20, 140, 110, 90, '2 lakh log', '12 baje');
  box(180, 140, 130, 90, 'Waiting room', 'ginti ~3000', { id: 'wr', stroke: C.acc });
  box(360, 150, 100, 70, 'App');
  box(500, 272, 130, 56, 'Kafka', 'key = show_id', { id: 'kf' });
  box(720, 275, 150, 56, 'Booking worker', 'ek-ek UPDATE', { id: 'bw' });
  box(720, 145, 150, 85, 'SQL DB', 'show 42 seats', { stroke: C.acc });
}
function frame(t, fix, tl) {
  ['wr', 'kf', 'bw', 'wk', 'ww', 'wd'].forEach(id => vis(id, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: 12 baje 2 lakh log seedha DB pe. 3000 seat, sab ek hi rows ke lock pe ruke, sab dheema';
    for (let k = 0; k < 30; k++) {
      const s = k * 0.2;
      if (tl > s && tl < s + 0.9) dot(path([[s, U], [s + 0.9, [480, 185]]], tl), C.msg);
    }
    const n = Math.min(11, Math.floor(tl * 2));
    for (let i = 0; i < n; i++) dot([696 - i * 20, 185], C.bad);
    if (tl > 1.5) T(590, 140, 'sab ek hi rows ke lock pe ruke', { c: C.bad, b: 1, size: 13 });
    if (tl > 3.0) T(795, 265, 'sab dheema', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: ~3000 andar, baaki line me (waiting room). Kafka: ek show ek line, worker ek-ek UPDATE';
  for (let k = 0; k < 30; k++) {
    const s = k * 0.25;
    if (tl > s && tl < s + 0.5) dot(path([[s, U], [s + 0.5, WL]], tl), C.msg);
  }
  T(245, 255, 'baaki line me', { c: C.acc, size: 12 });
  T(245, 272, '(apni jagah dikhti)', { c: C.muted, size: 12 });
  T(105, 300, '"booking ho rahi hai"', { c: C.acc, size: 12 });
  for (let k = 0; k < 8; k++) {
    const s = 0.4 + k * 1.0;
    const p = path([[s, WR], [s + 0.4, AL], [s + 0.5, [410, 222]], [s + 0.8, [410, 300]], [s + 1.0, [488, 300]]], tl);
    if (tl < s + 1.0) dot(p, C.msg);
    if (tl > s + 1.3 && tl < s + 1.6) dot(path([[s + 1.3, [630, 300]], [s + 1.6, [708, 300]]], tl), C.acc);
    if (tl > s + 1.8 && tl < s + 2.2) dot(path([[s + 1.8, [795, 273]], [s + 2.2, [795, 232]]], tl), C.good);
  }
  const done = Math.max(0, Math.min(8, Math.floor((tl - 2.2) / 1.0) + 1));
  T(795, 125, `UPDATE ek-ek: ${done}`, { c: C.good, b: 1, size: 13 });
  T(565, 350, 'ek show = ek line', { c: C.muted, size: 12 });
}
