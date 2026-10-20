// Card 5: 90 TB ek machine me nahi. Ilaaj: code se shard, ring. Agar: machine mari, bojh ek pe. Wapas: har click ek machine pe.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 5 saal me 90 TB, ek machine me aata hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: code se kai machine me baanto. Ring pe aage, pehli machine uski'],
  [6, YEL, 'AGAR', 'AGAR ek machine mar gayi? -> baaki 2 copy se milta, link nahi gaye'],
  [9, YEL, 'AGAR', 'AGAR ek machine pe zyada bojh? -> ring pe 100-200 jagah (virtual nodes)'],
  [12, GRN, 'WAPAS', 'WAPAS: click hamesha code ke saath, seedha ek machine pe'],
];
function setup() {}
const CX = 600, CY = 180, R = 120;
function pt(deg) { const a = deg * Math.PI / 180; return [CX + R * Math.cos(a), CY + R * Math.sin(a)]; }
function ring() { el('circle', { cx: CX, cy: CY, r: R, fill: 'none', stroke: C.line, 'stroke-width': 2 }, DYN); }
// machine M1 upar, M2 daayein, M3 neeche, M4 baayein
const MP = [[-90, 'M1'], [0, 'M2'], [90, 'M3'], [180, 'M4']];
function machines(sts, subs) {
  MP.forEach(([d, n], i) => { const [x, y] = pt(d); B(x - 45, y - 22, 90, 44, n, subs[i] || '', { st: sts[i] }); });
}
function arc(d1, d2, c, o = {}) {
  const pts = []; for (let d = d1; d <= d2; d += 8) pts.push(pt(d)); pts.push(pt(d2));
  AP(pts, c, o);
}
function app(n) {
  B(20, 150, 110, 70, 'App', 'code');
  const p = pt(-140); A(130, 175, p[0] - 4, p[1] + 2, C.msg, { n }); dot(p, C.msg);
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 110, 70, 'App', 'link'); A(130, 185, 298, 185, C.msg, { n: 1 });
    B(300, 130, 200, 110, 'Ek machine', 'DB', { st: 'bad' });
    bar(300, 260, 200, 20, 1, ''); T(400, 305, '90 TB', { c: C.bad, b: 1, size: 18 });
    dots(5, 530, 160, C.bad, { gap: 22 }); num(530, 125, 2, C.bad); L(550, 130, 'aata hi nahi', C.bad, 'start');
  } else if (ph === 1) {
    ring(); machines(['', '', 'good', ''], ['', '', '', '']);
    app(1);
    arc(-140, -112, C.acc, { n: 2, at: 0.5 }); L(430, 70, 'ring pe aage', C.acc);
    const p = pt(-90); num(p[0] + 60, p[1], 3, C.good); L(p[0] + 78, p[1] + 5, 'pehli machine: uski', C.good, 'start');
    L(CX, CY + 5, '3 copy', C.muted);
  } else if (ph === 2) {
    ring(); machines(['bad', 'good', 'good', ''], ['', 'copy', 'copy', '']);
    const p = pt(-90); cross(p[0] + 65, p[1]); num(p[0] + 100, p[1], 1, C.bad); L(p[0] + 118, p[1] + 5, 'mari', C.bad, 'start');
    B(20, 150, 110, 70, 'App', 'code');
    const q = pt(90); A(130, 200, q[0] - 47, q[1], C.good, { n: 2, at: 0.45 });
    L(300, 220, 'copy se mila', C.good);
    L(760, 320, 'hal: 3 copy, link nahi gaye', C.good);
  } else if (ph === 3) {
    ring(); machines(['bad', 'good', 'good', 'good'], ['', '', '', '']);
    const cs = [C.bad, C.good, C.msg, C.acc];
    let k = 0;
    for (let d = -180; d < 180; d += 10) {
      if ([-90, 0, 90, 180, -180].some(b => Math.abs(d - b) < 30)) continue;
      dot(pt(d), cs[k % 4]); k++;
    }
    const off = [[0, -35], [62, 0], [0, 35], [-62, 0]];
    MP.forEach(([d], i) => { const [x, y] = pt(d); dot([x + off[i][0], y + off[i][1]], cs[i]); });
    const p = pt(-90); num(p[0] + 70, p[1] - 18, 1, C.bad); L(p[0] + 88, p[1] - 13, 'zyada bojh', C.bad, 'start');
    L(CX, CY - 5, 'har machine', C.good); L(CX, CY + 13, '100-200 jagah', C.good);
    L(160, 320, 'hal: virtual nodes, bojh barabar', C.good);
  } else {
    ring(); machines(['good', 'good', 'good', 'good'], ['', '', '', '']);
    app(1);
    arc(-140, -112, C.good, { n: 2, at: 0.5 }); L(430, 70, 'seedha ek machine', C.good);
  }
}
