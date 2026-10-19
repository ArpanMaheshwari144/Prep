// Card 6: ek DB, 5,000 tweet/sec, 0.5 PB -> na rakh sake na likh sake. Ilaaj: Cassandra, user_id se baanto.
const DUR = 17, FIX = 8;
const SRC = [70, 192], DB1 = [450, 192], N = [[560, 70], [560, 192], [560, 314]];
function setup() {
  const bug = grp('bug'), fix = grp('fix');
  box(20, 150, 100, 84, 'Tweets', '5,000 / sec');
  box(380, 140, 140, 104, 'Ek DB', '', { parent: bug }); wire('M120 192 H380', '', bug);
  box(250, 160, 110, 64, 'hash(user_id)', '', { parent: fix, stroke: C.acc }); wire('M120 192 H250', '', fix);
  N.forEach(([x, y], i) => { box(x - 70, y - 35, 140, 70, `Cassandra ${i + 1}`, '', { parent: fix }); wire(`M360 192 L${x - 70} ${y}`, '', fix); });
}
function frame(t, fix, tl) {
  vis('bug', !fix); vis('fix', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: ek hi DB. Har second 5,000 tweet, 5 saal me 0.5 PB. Na rakh sake, na likh sake';
    for (let k = 0; k < 14; k++) { const s = 0.2 + k * 0.3; const p = path([[s, SRC], [s + 0.7, [370, 192]]], tl);
      if (tl < s + 0.7) dot(p, C.msg); else if (k > 6 && tl < s + 1.4) dot([340 - (k - 6) * 22, 230], C.bad); }
    const f = Math.min(1, tl / 5);
    bar(390, 260, 120, 14, f, 'disk');
    T(450, 300, `${(0.5 * f).toFixed(2)} PB`, { c: f >= 1 ? C.bad : C.fg, b: 1 });
    if (tl > 3) T(270, 260, 'likhne ki line', { c: C.bad, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: Cassandra, user_id se baanto. Mere saare tweet ek machine pe, naya sabse upar. Ek read';
  const users = [['Main', 1], ['Ravi', 0], ['Asha', 2], ['Main', 1], ['Neha', 0], ['Main', 1]];
  users.forEach(([u, n], k) => { const s = 0.3 + k * 0.6;
    const p = path([[s, SRC], [s + 0.5, [305, 205]], [s + 1.0, [N[n][0] - 70, N[n][1] + 12]]], tl);
    if (tl < s + 1.2) dot(p, u === 'Main' ? C.acc : C.msg, '', p && p[0] > 380 ? u : '', u === 'Main' ? C.acc : C.msg); });
  if (tl > 4.5) {
    T(700, 192, 'mere saare tweet yahin', { c: C.acc, b: 1, size: 13, a: 'start' });
    T(700, 212, 'profile = 1 machine, 1 read', { c: C.good, size: 12, a: 'start' });
  }
  T(780, 330, 'har machine ki 3 copy', { c: C.muted, size: 12 });
}
