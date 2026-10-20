// Card 5: har inbox me saare id, soye logon ke bhi -> RAM bhari. Ilaaj: naya upar, 800 se neeche kaato, soye ka inbox TTL se mite.
const DUR = 19, FIX = 8;
function setup() {
  box(20, 60, 110, 50, 'Mera inbox');
  box(20, 220, 170, 60, 'Ravi ka inbox', '3 mahine se nahi aaya');
  box(640, 290, 230, 60, 'Redis RAM');
}
function frame(t, fix, tl) {
  if (!fix) {
    CAP.textContent = 'DIKKAT: har inbox me saare tweet id, soye logon ke bhi. 50 crore inbox, RAM bhar rahi';
    const n = Math.min(12, 3 + Math.floor(tl / 0.45));
    const mine = []; for (let i = 0; i < n; i++) mine.push([`t${9000 - i}`, C.fg]);
    cells(150, 72, mine.slice(0, 13), { w: 50, h: 26 });
    T(150 + 13 * 54 - 40, 125, `${Math.round(n * 750)} id`, { c: C.bad, size: 12, b: 1, a: 'start' });
    cells(205, 237, [['t1', C.muted], ['..', C.muted], ['t9000', C.muted]], { w: 60, h: 26 });
    T(300, 290, '9,000 id, koi padhta nahi', { c: C.bad, size: 12 });
    const f = Math.min(1, 0.4 + tl / 8);
    bar(650, 322, 210, 14, f, '');
    if (f >= 1) T(755, 278, 'RAM bhar gayi!', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: naya tweet upar, 800 se neeche wale kaato. Soye ka inbox 30 din me mite (TTL)';
  T(390, 50, '8 dabbe = aakhri 800', { c: C.muted, size: 12 });
  const k = Math.min(4, Math.floor(tl / 1.1));
  const base = []; for (let i = 0; i < 8; i++) base.push(`t${9000 + k - i}`);
  const items = base.map((s, i) => [s, i === 0 && k > 0 ? C.good : C.fg]);
  cells(150, 72, items, { w: 54, h: 26 });
  const ph = (tl % 1.1) / 1.1;
  if (k < 4 && tl > 0.2) {   // gira hua purana
    const x = 150 + 8 * 58 + 6, y = 72 + ph * 60;
    el('rect', { x, y, width: 54, height: 26, rx: 4, fill: C.bg, stroke: C.bad }, DYN);
    T(x + 27, y + 17, `t${9000 + k - 8}`, { c: C.bad, size: 11, b: 1 });
    T(x + 27, y + 46, 'kata', { c: C.bad, size: 11 });
  }
  const gone = tl > 5;
  if (!gone) { cells(205, 237, [['t1', C.muted], ['..', C.muted], ['t800', C.muted]], { w: 60, h: 26 });
               T(420, 255, `TTL: ${Math.max(0, 30 - Math.round(tl * 6))} din`, { c: C.acc, size: 13, b: 1, a: 'start' }); }
  else { T(330, 245, 'inbox khud mit gaya', { c: C.good, size: 13, b: 1 });
         T(330, 265, 'wapas aaye to DB se ek baar bana do', { c: C.muted, size: 12 }); }
  const f = gone ? 0.25 : 0.5;
  bar(650, 322, 210, 14, f, '', true);
  T(755, 278, '5 MB se 50 KB har inbox', { c: C.good, b: 1, size: 12 });
}
