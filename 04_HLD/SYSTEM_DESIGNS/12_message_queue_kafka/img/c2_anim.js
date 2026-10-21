// Card 2: sale me 5,000 / sec, ek worker 500. Ilaaj: 10 partition, har ek ka worker. Agar: 12 worker, 10 tukde.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 10 baje sale, 5,000 order / sec ek hi line me. Ek worker 500 hi bhej paata'],
  [3, GRN, 'ILAAJ', 'ILAAJ: topic ke 10 tukde (partition), har tukde ka apna worker. 10 x 500 = 5,000'],
  [6, YEL, 'AGAR', 'AGAR worker 12, tukde 10? -> 2 worker khaali. Ek tukda ek hi worker padhta'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Orders → Queue', '10 baje sale: 5,000 order / sec ek hi line me aaye'],
   [C.bad, 'Queue → Worker', 'Ek worker sirf 500 / sec bhej paata'],
   [C.bad, 'Queue', 'Har second 4,500 peeche chhoote, line lambi hoti gayi (lag)']],
  [[C.msg, 'Orders → P0..P9', 'Topic ke 10 tukde (partition), order unme bat gaye'],
   [C.good, 'P → Worker', 'Har tukde ka apna worker, har ek 500 / sec'],
   [C.good, 'kul', '10 tukde x 500 = 5,000 / sec, line nahi badhti']],
  [[C.msg, 'tukde', 'Topic me 10 tukde hain, P0 se P9'],
   [C.good, 'P → W', 'Ek tukda ek hi worker padhta: 10 worker ko kaam mila'],
   [C.bad, 'W11, W12', '2 worker khaali baithe: pehle tukde badhao, worker baad me']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 140, 180, 60, 'Orders', '5,000 / sec', { icon: 'server' });
    B(290, 130, 300, 80, 'Queue', 'ek hi line', { st: 'bad', icon: 'queue' }); bar(290, 225, 300, 14, 1, '');
    A(195, 170, 288, 170, C.msg, { n: 1 });
    B(690, 140, 190, 60, 'Worker', '500 / sec', { st: 'bad', icon: 'server' });
    A(592, 170, 688, 170, C.bad, { n: 2 });
    num(440, 270, 3, C.bad); L(440, 300, 'har sec 4,500 peeche: lag badhta', C.bad);
  } else if (ph === 1) {
    B(15, 140, 180, 60, 'Orders', '5,000 / sec', { icon: 'server' });
    const ys = [20, 100, 180, 270], ps = ['P0', 'P1', '...', 'P9'], ws = ['W0', 'W1', '...', 'W9'];
    ys.forEach((y, i) => {
      const dim = ps[i] === '...';
      B(320, y, 160, 55, ps[i], dim ? '' : 'tukda', { st: dim ? 'dim' : 'acc' });
      B(620, y, 180, 55, ws[i], dim ? '' : '500 / sec', { st: dim ? 'dim' : 'good', icon: dim ? undefined : 'server' });
      if (!dim) { A(195, 170, 318, y + 27, C.msg, { n: i === 0 ? 1 : 0 }); A(480, y + 27, 618, y + 27, C.good, { n: i === 0 ? 2 : 0 }); }
    });
    num(850, 170, 3, C.good); L(820, 345, '10 x 500 = 5,000', C.good);
  } else {
    num(20, 85, 1, C.msg);
    const P = Array.from({ length: 10 }, (_, i) => ['P' + i, C.acc]);
    cells(40, 65, P, { w: 72, h: 36 });
    const W = Array.from({ length: 10 }, (_, i) => ['W' + (i + 1), C.good]);
    cells(40, 235, W, { w: 72, h: 36 });
    for (let i = 0; i < 10; i++) A(76 + i * 72, 103, 76 + i * 72, 233, C.good, { n: i === 4 ? 2 : 0, at: 0.5 });
    cells(40, 300, [['W11', C.bad], ['W12', C.bad]], { w: 72, h: 36 }); num(205, 318, 3, C.bad);
    L(470, 318, 'khaali: koi tukda bacha hi nahi', C.bad);
  }
}
