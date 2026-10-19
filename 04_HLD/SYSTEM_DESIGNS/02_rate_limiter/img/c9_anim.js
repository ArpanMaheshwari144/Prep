// Card 9: office ka ek IP (NAT), 1000 log. Ginti IP pe -> ek ki galti se sab atke. Ilaaj: key = user_id (JWT).
const DUR = 18, FIX = 8;
const NAMES = ['Ravi', 'Asha', 'Mohan', 'Neha', 'Suresh'];
const P = NAMES.map((_, i) => [80, 70 + i * 62]), NAT = [270, 200], GW = [460, 212], APP = [700, 192];
function setup() {
  P.forEach(([x, y], i) => { box(x - 55, y - 22, 110, 44, NAMES[i]); wire(`M${x + 55} ${y} L225 192`); });
  wire('M315 192 H410'); wire('M510 192 H640');
  box(225, 160, 90, 64, 'Office', '1 IP (NAT)');
  box(410, 160, 100, 64, 'Gateway', 'limiter', { stroke: C.acc });
  box(640, 160, 120, 64, 'App');
}
function frame(t, fix, tl) {
  // pehle Ravi 6 baar galat password, phir baaki chaar ek-ek baar
  const reqs = [];
  for (let k = 0; k < 6; k++) reqs.push([0, 0.3 + k * 0.35]);
  for (let i = 1; i < 5; i++) reqs.push([i, 2.8 + i * 0.5]);
  let ipCount = 0; const uc = [0, 0, 0, 0, 0];
  reqs.forEach(([u, s]) => {
    const arrive = s + 0.9;
    if (tl < arrive) { dot(path([[s, P[u]], [s + 0.5, NAT], [arrive, GW]], tl), C.msg); return; }
    ipCount++; uc[u]++;
    const ok = fix ? uc[u] <= 5 : ipCount <= 5;
    if (tl < arrive + 0.8) {
      if (ok) dot(path([[arrive, GW], [arrive + 0.5, APP]], tl), C.good);
      else dot([385, 212], C.bad, '', '429');
    }
    if (u > 0) T(P[u][0] + 70, P[u][1] + 5, ok ? 'andar' : 'atka!', { a: 'start', c: ok ? C.good : C.bad, b: 1, size: 12 });
  });
  if (!fix) {
    CAP.textContent = 'DIKKAT: ginti IP pe. Ravi ke 6 galat login se office ka IP block, baaki sab atke';
    T(460, 140, `ginti[office IP] = ${ipCount}`, { c: ipCount > 5 ? C.bad : C.acc, b: 1, size: 13 });
  } else {
    CAP.textContent = 'ILAAJ: login ke baad ginti user_id pe (JWT se). Ravi ki galti sirf Ravi pe';
    T(460, 140, `ginti[Ravi] = ${uc[0]}`, { c: uc[0] > 5 ? C.bad : C.acc, b: 1, size: 13 });
    T(460, 250, 'har user ki apni ginti', { c: C.muted, size: 12 });
  }
  T(890, 345, 'had: 5 / minute', { a: 'end', c: C.muted, size: 12 });
}
