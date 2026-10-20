// Card 6: har akshar seedha DB -> 60 lakh write/sec, DB baitha. Ilaaj: Redis buffer + copy (WAIT 1) -> ACK, har 1 sec batch.
const DUR = 18, FIX = 8;
const U = [150, 182], CSR = [350, 182], DB = [688, 185];
function setup() {
  wire('M150 182 H220');
  box(20, 140, 130, 84, '20 lakh log', '3 akshar / sec');
  box(220, 150, 130, 64, 'Conn-Server');
  box(700, 150, 170, 70, 'Cassandra');
  wire('M350 182 H700', 'w0');
  wire('M350 170 L450 92', 'w1'); wire('M350 195 L450 282', 'w2'); wire('M600 92 L700 170', 'w3');
  box(450, 60, 150, 64, 'Redis buffer', 'op jama', { id: 'rb', stroke: C.acc });
  box(450, 250, 150, 64, 'Redis copy', 'WAIT 1', { id: 'rc', stroke: C.acc });
}
function frame(t, fix, tl) {
  ['rb', 'rc', 'w1', 'w2', 'w3'].forEach(i => vis(i, fix)); vis('w0', !fix);
  for (let k = 0; k < 64; k++) {
    const s = 0.2 + k * 0.15;
    dot(path([[s, U], [s + 0.3, [214, 182]]], tl < s + 0.3 ? tl : -1), C.msg);
    if (!fix) dot(path([[s + 0.35, CSR], [s + 1.0, DB]], tl > s + 0.35 && tl < s + 1.0 ? tl : -1), C.msg);
    else dot(path([[s + 0.35, CSR], [s + 0.8, [438, 92]]], tl > s + 0.35 && tl < s + 0.8 ? tl : -1), C.msg);
  }
  if (!fix) {
    CAP.textContent = 'DIKKAT: har akshar seedha DB me. 20 lakh log x 3 akshar = 60 lakh write / sec';
    const f = Math.min(1, tl / 5);
    bar(710, 245, 150, 16, f, '');
    T(785, 285, `${Math.round(60 * f)} lakh write / sec`, { c: f > 0.9 ? C.bad : C.fg, b: 1, size: 12 });
    if (f >= 1) T(785, 120, 'DB baith gaya', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: op Redis buffer + copy dono me (WAIT 1, ~1 ms), tab ACK. Har 1 sec ek batch Cassandra me';
  if (tl > 0.6) { const ph = (tl - 0.6) % 1; if (ph < 0.4) dot(lerp([360, 200], [438, 282], ph / 0.4), C.acc, '', 'copy', C.acc); }
  if (tl > 1.2) { const ph = (tl - 1.2) % 1; if (ph < 0.4) dot(lerp([214, 200], [150, 212], ph / 0.4), C.good, '', 'ACK', C.good); }
  if (tl > 1.2) T(85, 320, 'ACK "saved" (~1 ms)', { c: C.good, b: 1, size: 13 });
  if (tl > 1.0) { const ph = (tl - 1.0) % 1; if (ph < 0.5) dot(lerp([612, 104], [688, 170], ph / 0.5), C.acc, 'B', 'batch', C.acc); }
  T(785, 120, 'har 1 sec ek batch', { c: C.acc, size: 12, b: 1 });
  bar(710, 245, 150, 16, 0.15, '', 1);
  T(785, 285, 'kam likhai, DB chalu', { c: C.good, b: 1, size: 12 });
}
