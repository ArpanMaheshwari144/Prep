// Card 2: 1 TB, user:42 kis machine? Ilaaj: consistent hashing ring + vnodes. Agar: machine D judi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: hash % N se machine chuni. Ek machine judi to N badla, ~80% key ka ghar badla'],
  [3, GRN, 'ILAAJ', 'ILAAJ: key ka hash ring pe ek jagah, aage chalo, pehli machine uska ghar'],
  [6, YEL, 'AGAR', 'AGAR machine D judi? -> sirf 201..250 wali key C se D pe, baaki sab HIT'],
];
function setup() {}
// ring ghuma ke banaya: key 150 baayein, taaki App ka teer kisi dabbe pe na chadhe
const CX = 660, CY = 175, R = 135;
function rp(p, r = R) { const t = (p + 120) * Math.PI / 180; return [CX + r * Math.sin(t), CY - r * Math.cos(t)]; }
function ring() {
  el('circle', { cx: CX, cy: CY, r: R, fill: 'none', stroke: C.line, 'stroke-width': 2, 'stroke-dasharray': '6 5' }, DYN);
  T(CX, CY - 4, 'ring 0..360', { c: C.muted, size: 13 }); T(CX, CY + 16, 'ghadi ki disha me aage', { c: C.muted, size: 13 });
}
function mc(name, p, st, sub) { const [x, y] = rp(p); B(x - 46, y - 23, 92, 46, name, sub || ('@' + p), { st }); }
const STEPS = [
  [[C.good, 'App → M2', 'Pehle 4 machine: 46 % 4 = 2, user:42 machine M2 pe mila'],
   [C.bad, 'App → M1', 'Ek machine judi, ab 46 % 5 = 1, App M1 pe gaya, wahan key nahi'],
   [C.bad, 'M1 → DB', 'Aisi ~80% key ka ghar badla, sab MISS, saari bheed DB pe']],
  [[C.acc, 'App → ring', 'user:42 ka hash 150 nikla, ring pe 150 wali jagah'],
   [C.good, 'ring → B', 'Ring pe aage chalo, pehli machine B (200) mili, key ka ghar wahi'],
   [C.acc, 'vnodes', 'Har machine ring pe 100-200 jagah baithti hai, bojh barabar bant-ta']],
  [[C.acc, 'D judi', 'Naya machine D ring pe 250 wali jagah pe judi'],
   [C.acc, 'C → D', 'Sirf 201..250 wali key C se D pe khiski, unki ek baar miss'],
   [C.good, 'App → B', 'user:42 (150) abhi bhi B pe, HIT; baaki saari key apni jagah']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 130, 150, 60, 'App', 'get user:42', { icon: 'server' });
    T(330, 28, 'pehle 4 machine:  hash 46 % 4 = 2', { c: C.muted, b: 1, size: 14, a: 'start' });
    ['M0', 'M1', 'M2', 'M3'].forEach((n, i) => B(330 + i * 110, 40, 95, 46, n, i === 2 ? 'user:42' : '', { st: i === 2 ? 'good' : 'dim' }));
    A(165, 145, 597, 88, C.good, { n: 1, at: 0.5 });
    T(865, 168, '1 machine judi:  hash 46 % 5 = 1', { c: C.bad, b: 1, size: 14, a: 'end' });
    ['M0', 'M1', 'M2', 'M3', 'M4'].forEach((n, i) => B(330 + i * 110, 180, 95, 46, n,
      i === 1 ? 'MISS' : i === 2 ? 'key yahan' : '', { st: i === 1 ? 'bad' : 'dim' }));
    AP([[165, 175], [300, 175], [300, 140], [487, 140], [487, 178]], C.bad, { n: 2, at: 0.6 });
    B(420, 290, 140, 50, 'DB', 'bheed', { st: 'bad', icon: 'db' });
    A(487, 228, 487, 288, C.bad, { n: 3 });
    L(740, 315, '~80% key ka ghar badla', C.bad);
  } else if (ph === 1) {
    B(15, 145, 150, 60, 'App', 'get user:42', { icon: 'server' });
    ring(); mc('A', 100, 'dim'); mc('B', 200, 'good'); mc('C', 300, 'dim');
    const k = rp(150), b = rp(200); dot(k, C.acc, '');
    A(165, 175, k[0] - 10, k[1], C.acc, { n: 1, at: 0.45 }); L(330, 210, 'hash(user:42) = 150', C.acc);
    A(k[0] + 4, k[1] - 12, b[0] - 10, b[1] + 25, C.good, { n: 2, at: 0.5 });
    L(330, 75, 'aage chalo: pehli machine B', C.good); L(330, 97, '= user:42 ka ghar', C.good);
    num(560, 330, 3, C.acc); L(740, 322, 'har machine ring pe 100-200 jagah', C.acc); L(740, 342, '(virtual nodes)', C.acc);
  } else {
    B(15, 145, 150, 60, 'App', 'get user:42', { icon: 'server' });
    ring();
    const [a1, b1] = rp(200), [a2, b2] = rp(250);
    el('path', { d: `M ${a1} ${b1} A ${R} ${R} 0 0 1 ${a2} ${b2}`, fill: 'none', stroke: C.acc, 'stroke-width': 8, opacity: 0.5 }, DYN);
    mc('A', 100, 'dim'); mc('B', 200, 'good'); mc('C', 300, 'dim'); mc('D', 250, 'acc', '@250 naya');
    const d = rp(250), c = rp(300), k = rp(150);
    num(d[0] + 66, d[1] - 8, 1, C.acc);
    A(c[0] - 30, c[1] - 24, d[0] + 30, d[1] + 24, C.acc, { n: 2, dash: 1 });
    L(838, 150, 'key 201..250', C.acc); L(838, 170, 'C se D pe', C.acc);
    dot(k, C.good, '');
    A(165, 175, k[0] - 10, k[1], C.good, { n: 3, at: 0.45 }); L(330, 215, 'user:42 = 150: abhi bhi B, HIT', C.good);
    L(760, 335, 'baaki saari key apni jagah', C.muted);
  }
}
