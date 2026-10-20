// Card 8: Diwali 3000 / sec, ek machine, CPU bhara, timeout, dobara tap. Ilaaj: LB + kai stateless machine, yaad DB me.
const DUR = 17, FIX = 7.5;
const US = [130, 185], S0 = [318, 185], LB = [188, 185], LBR = [280, 185];
const SV = [[368, 60], [368, 185], [368, 310]];
function setup() {
  wire('M130 185 H330', 'r1'); wire('M480 185 H680', 'r2');
  wire('M130 185 H200', 'g1'); wire('M280 185 L380 60', 'g2'); wire('M280 185 H380', 'g3'); wire('M280 185 L380 310', 'g4');
  wire('M530 60 L680 175', 'g5'); wire('M530 185 H680', 'g6'); wire('M530 310 L680 195', 'g7');
  box(20, 150, 110, 70, 'Users', '3000 / sec');
  box(330, 150, 150, 70, 'Payment Svc', 'ek machine', { id: 's0' });
  box(200, 150, 80, 70, 'LB', '', { id: 'lb' });
  box(380, 30, 150, 60, 'Svc 1', 'stateless', { id: 's1' });
  box(380, 155, 150, 60, 'Svc 2', 'stateless', { id: 's2' });
  box(380, 280, 150, 60, 'Svc 3', 'stateless', { id: 's3' });
  box(680, 150, 190, 70, 'DB + Idempotency', 'yaad yahan');
}
function frame(t, fix, tl) {
  ['r1', 'r2', 's0'].forEach(id => vis(id, !fix));
  ['g1', 'g2', 'g3', 'g4', 'g5', 'g6', 'g7', 'lb', 's1', 's2', 's3'].forEach(id => vis(id, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: Diwali, 3000 payment / sec, ek hi machine. CPU bhara, timeout, log dobara tap';
    for (let k = 0; k < 40; k++) {
      const s = 0.2 + 0.15 * k;
      if (tl > s && tl < s + 0.5) dot(path([[s, US], [s + 0.5, S0]], tl), s + 0.5 > 3.0 ? C.bad : C.msg);
    }
    const f = Math.min(1, tl / 3);
    bar(330, 245, 150, 16, f, 'CPU');
    if (tl >= 3.2) { T(405, 290, 'CPU bhara, payment atke', { c: C.bad, b: 1, size: 13 });
                     T(405, 312, 'timeout -> log dobara tap -> aur bojh', { c: C.bad, size: 12 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: kai machine + LB. Machine kuch yaad nahi rakhti (stateless), yaad DB me';
  for (let k = 0; k < 44; k++) {
    const s = 0.2 + 0.15 * k;
    if (tl > s && tl < s + 0.8) dot(path([[s, US], [s + 0.4, LB], [s + 0.4, LBR], [s + 0.8, SV[k % 3]]], tl), C.msg);
  }
  T(240, 135, 'LB: koi bhi machine', { c: C.muted, size: 12 });
  T(790, 240, 'CPU', { c: C.muted, size: 12 });
  const f = Math.min(0.4, tl / 6);
  for (let i = 0; i < 3; i++) {
    T(705, 264 + i * 28, `Svc ${i + 1}`, { a: 'start', c: C.fg, size: 12 });
    bar(760, 252 + i * 28, 110, 14, f, '', true);
  }
}
