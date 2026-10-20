// Card 4: PSP 30 sec, timeout nahi -> har thread ruka, sab atke. Ilaaj: 2-3 sec me PENDING, 5 fail -> circuit OPEN.
const DUR = 18, FIX = 8;
const US = [130, 185], SVL = [238, 185], SVR = [500, 185], PS = [708, 185];
function setup() {
  wire('M130 185 H250'); wire('M500 185 H720');
  box(20, 150, 110, 70, 'Users', 'top-up 2000');
  box(250, 90, 250, 190, 'Payment Svc', 'thread');
  box(720, 150, 150, 70, 'PSP (bank)', 'dheema: 30 sec');
  box(560, 240, 130, 60, 'Circuit', '', { id: 'cb' });
}
function threads(st) {
  const it = st.map(s => s === 0 ? ['free', C.good] : s === 1 ? ['ruka', C.bad] : ['wait', C.acc]);
  cells(270, 145, it.slice(0, 4), { w: 50, h: 30 }); cells(270, 195, it.slice(4), { w: 50, h: 30 });
}
function frame(t, fix, tl) {
  vis('cb', fix);
  const st = [0, 0, 0, 0, 0, 0, 0, 0];
  if (!fix) {
    CAP.textContent = 'DIKKAT: timeout hi nahi. Har thread PSP ke jawab (30 sec) tak ruka, saare atke, system band';
    for (let k = 0; k < 10; k++) {
      const s = 0.3 + 0.6 * k;
      if (tl < s + 0.5) { dot(path([[s, US], [s + 0.5, SVL]], tl), C.msg); continue; }
      if (k < 8) { st[k] = 1; if (tl < s + 1.1) dot(path([[s + 0.5, SVR], [s + 1.1, PS]], tl), C.acc); }
      else dot([190, 245], C.bad, '', 'atka', C.bad);
    }
    threads(st);
    T(795, 245, `intezaar: ${Math.max(0, Math.floor(tl - 0.8))} sec`, { c: C.bad, b: 1, size: 13 });
    if (tl >= 5.6) T(375, 315, 'saare thread atke, system band', { c: C.bad, b: 1, size: 14 });
    return;
  }
  CAP.textContent = 'ILAAJ: 2-3 sec me jawab nahi -> PENDING. 5 fail -> circuit OPEN, call hi nahi: "abhi nahi"';
  const OPEN = 0.3 + 0.6 * 4 + 3.0;
  let fails = 0, pend = 0;
  for (let k = 0; k < 11; k++) {
    const s = 0.3 + 0.6 * k;
    if (tl < s + 0.5) { dot(path([[s, US], [s + 0.5, SVL]], tl), C.msg); continue; }
    if (s + 0.5 >= OPEN) { if (tl < s + 1.1) dot(path([[s + 0.5, SVL], [s + 1.1, US]], tl), C.bad, '', 'abhi nahi', C.bad); continue; }
    if (tl < s + 3.0) { st[k % 8] = 2; if (tl < s + 1.1) dot(path([[s + 0.5, SVR], [s + 1.1, PS]], tl), C.acc); }
    else { fails++; pend++; }
  }
  threads(st);
  const open = tl >= OPEN;
  T(625, 288, open ? 'OPEN' : `CLOSED · fail ${Math.min(fails, 5)}`, { c: open ? C.bad : C.good, b: 1, size: 13 });
  T(375, 305, `PENDING likha: ${pend} (baad me pata karo)`, { c: C.acc, b: 1, size: 12 });
  if (open) { T(75, 250, '"abhi nahi"', { c: C.bad, b: 1, size: 12 }); T(75, 268, 'paisa hila nahi', { c: C.muted, size: 12 }); }
}
