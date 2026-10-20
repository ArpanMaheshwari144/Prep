// Card 7: sab apni had me, phir bhi CPU 100%. Ilaaj: system ki had + 503 kam zaroori pe. Wapas: CPU neeche.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har user apni had me, par kul jod se server CPU 100%'],
  [3, GRN, 'ILAAJ', 'ILAAJ: system ki had 5,000/sec. CPU 90% paar: kam zaroori ko 503'],
  [6, GRN, 'WAPAS', 'WAPAS: CPU 90% se neeche, 503 band, sab request phir andar'],
];
function setup() {}
function frame(ph) {
  for (let i = 0; i < 5; i++) { B(20, 25 + i * 64, 100, 44, `user ${i + 1}`, '', { st: 'good' }); A(120, 47 + i * 64, 308, 175); }
  B(310, 145, 110, 60, 'Gateway', ph ? 'had 5,000/s' : '', { st: 'acc' });
  const cpu = [1, 0.85, 0.5][ph];
  B(620, 130, 140, 90, 'Server', 'CPU', { st: ph === 0 ? 'bad' : 'good' });
  bar(630, 235, 120, 18, cpu, ''); T(690, 278, `${Math.round(cpu * 100)}%`, { c: ph === 0 ? C.bad : C.good, b: 1, size: 16 });
  if (ph === 0) { A(420, 175, 618, 175, C.bad); dots(6, 445, 150, C.msg, { gap: 26 }); }
  if (ph === 1) {
    A(420, 175, 618, 175, C.good); dots(3, 470, 150, C.good, { gap: 34 });
    T(505, 128, 'login / payment', { c: C.good, b: 1, size: 12 });
    dots(6, 310, 245, C.bad, { gap: 20 }); T(360, 280, '503', { c: C.bad, b: 1, size: 15 });
  }
  if (ph === 2) { A(420, 175, 618, 175, C.good); dots(6, 445, 150, C.good, { gap: 26 }); }
}
