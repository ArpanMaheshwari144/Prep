// Card 7: sab apni had me, phir bhi CPU 100%. Ilaaj: system ki had + 503 kam zaroori pe. Wapas: CPU neeche.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har user apni had me, par kul jod se server CPU 100%'],
  [3, GRN, 'ILAAJ', 'ILAAJ: system ki had 5,000/sec. CPU 90% paar: kam zaroori ko 503'],
  [6, GRN, 'WAPAS', 'WAPAS: CPU 90% se neeche, 503 band, sab request phir andar'],
];
function setup() {}
function frame(ph) {
  for (let i = 0; i < 5; i++) {
    B(20, 25 + i * 64, 100, 44, `user ${i + 1}`, '', { st: 'good' });
    A(120, 47 + i * 64, 308, 175, C.msg, { n: i === 2 ? 1 : 0, at: 0.45 });
  }
  B(310, 145, 110, 60, 'Gateway', ph ? 'had 5,000/s' : '', { st: 'acc' });
  const cpu = [1, 0.93, 0.5][ph];
  B(620, 130, 140, 90, 'Server', 'CPU', { st: ph === 0 ? 'bad' : 'good' });
  bar(630, 235, 120, 18, cpu, ''); T(690, 278, `${Math.round(cpu * 100)}%`, { c: [C.bad, C.acc, C.good][ph], b: 1, size: 16 });
  if (ph === 0) {
    A(420, 175, 618, 175, C.bad, { n: 2, at: 0.85 }); dots(6, 445, 150, C.msg, { gap: 26 });
    num(690, 320, 3, C.bad); L(690, 345, 'server gira', C.bad);
  }
  if (ph === 1) {
    A(420, 175, 618, 175, C.good, { n: 3, at: 0.85 }); dots(3, 470, 150, C.good, { gap: 34 });
    T(505, 128, 'login / payment', { c: C.good, b: 1, size: 12 });
    num(290, 245, 2, C.bad); dots(6, 315, 245, C.bad, { gap: 20 }); T(365, 280, '503', { c: C.bad, b: 1, size: 15 });
    L(690, 320, 'CPU 90% paar', C.acc);
  }
  if (ph === 2) {
    A(420, 175, 618, 175, C.good, { n: 2, at: 0.85 }); dots(6, 445, 150, C.good, { gap: 26 });
    L(690, 320, 'CPU neeche', C.good);
  }
}
