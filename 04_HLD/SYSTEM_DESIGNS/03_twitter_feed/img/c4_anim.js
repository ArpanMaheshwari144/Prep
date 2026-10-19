// Card 4: Virat ka tweet, crore feed DB pe -> DB gira. Ilaaj: hot-tweet cache (RAM), miss pe sirf ek DB jaaye.
const DUR = 17, FIX = 8;
const U = [[70, 70], [70, 140], [70, 210], [70, 280]], FS = [300, 192], HC = [450, 110], DB = [700, 250];
function setup() {
  U.forEach(([x, y], i) => { box(x - 50, y - 22, 100, 44, `crore log ${i + 1}`); wire(`M${x + 50} ${y} L250 175`); });
  box(250, 150, 100, 64, 'Feed Svc');
  box(640, 215, 120, 70, 'Tweet DB', '');
  wire('M350 185 L640 245', 'wdb');
  box(450, 55, 140, 70, 'Hot-tweet cache', 'RAM', { id: 'hc', stroke: C.acc });
  wire('M350 165 L450 95', 'whc'); wire('M590 100 L700 215', 'whd');
}
function frame(t, fix, tl) {
  vis('hc', fix); vis('whc', fix); vis('whd', fix);
  let load = 0;
  for (let k = 0; k < 18; k++) {
    const s = 0.3 + k * 0.22, u = U[k % 4];
    if (!fix) {
      const p = path([[s, u], [s + 0.4, FS], [s + 0.9, DB]], tl);
      if (tl >= s + 0.9) load++;
      if (tl < s + 1.1) dot(p, C.msg);
    } else {
      const first = k === 0;
      const pts = first ? [[s, u], [s + 0.4, FS], [s + 0.8, HC], [s + 1.3, DB]] : [[s, u], [s + 0.4, FS], [s + 0.8, HC]];
      const end = pts[pts.length - 1][0];
      if (first && tl >= end) load = 1;
      if (tl < end + 0.3) dot(path(pts, tl), first ? C.acc : C.good, '', first && tl > s + 0.8 ? 'miss: sirf main DB' : '', C.acc);
    }
  }
  const f = fix ? 0.08 : load / 18;
  bar(650, 300, 100, 14, f, '');
  T(700, 335, `DB bojh ${Math.round(100 * f)}%`, { c: f > 0.9 ? C.bad : C.good, b: 1, size: 12 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: har feed Virat ka tweet DB se laa rahi. Crore read, DB gira, feed band';
    if (f >= 1) cross(700, 250);
  } else {
    CAP.textContent = 'ILAAJ: garam tweet RAM me (hot-tweet cache). Miss pe sirf ek request DB, baaki uska jawab lein';
    T(520, 145, 'baaki sab yahin se', { c: C.good, size: 12, b: 1 });
  }
}
