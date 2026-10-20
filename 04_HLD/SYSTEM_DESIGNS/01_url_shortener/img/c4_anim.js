// Card 4: click ginti ne redirect dheema kiya. Ilaaj: Kafka se peeche. Agar: Analytics dheema.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: click ginti redirect ke raaste me, DB likhai ka wait, redirect der se'],
  [3, GRN, 'ILAAJ', 'ILAAJ: redirect turant (302), click ki khabar Kafka me, ginti peeche'],
  [6, YEL, 'AGAR', 'AGAR Analytics dheema ho gaya? -> khabar Kafka me padi rehti, apni speed se padho'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'User → App', 'click'], [C.bad, 'App → DB', 'ginti likho, wait'], [C.bad, 'App', 'DB likhai (~10 ms) pe ruka'], [C.bad, 'App → User', 'redirect der se']],
  [[C.msg, 'User → App', 'click'], [C.good, 'App → User', '302 turant'], [C.acc, 'App → Kafka', 'click ki khabar'], [C.good, 'Kafka → Analytics', 'ginti apni speed se']],
  [[C.msg, 'User → App', 'click'], [C.good, 'App → User', '302 turant, koi asar nahi'], [C.acc, 'App → Kafka', 'khabar padi, khoti nahi'], [C.bad, 'Kafka → Analytics', 'dheema: offset se, baad me 40 se aage']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 120, 60, 'User'); A(140, 145, 258, 145, C.msg, { n: 1 }); B(260, 130, 120, 60, 'App');
    B(560, 130, 130, 60, 'DB', 'ginti +1', { st: 'bad' });
    A(380, 150, 558, 150, C.bad, { n: 2 }); L(470, 135, 'likho', C.bad);
    L(625, 215, 'disk pe likhai, ~10 ms', C.bad); num(625, 245, 3, C.bad); L(625, 270, 'App yahan ruka', C.bad);
    A(258, 180, 142, 180, C.bad, { n: 4, dash: 1 }); L(200, 212, 'redirect der se', C.bad);
  } else if (ph === 1) {
    B(20, 130, 120, 60, 'User'); A(140, 145, 258, 145, C.msg, { n: 1 }); B(260, 130, 120, 60, 'App');
    A(258, 180, 142, 180, C.good, { n: 2 }); L(200, 215, '302 turant', C.good);
    B(520, 130, 130, 60, 'Kafka', 'disk pe pakki', { st: 'acc' });
    A(380, 160, 518, 160, C.acc, { n: 3 }); L(450, 140, 'click hua', C.acc);
    B(720, 130, 150, 60, 'Analytics', 'ginti yahan', { st: 'good' });
    A(650, 160, 718, 160, C.good, { n: 4 }); L(685, 215, 'apni speed se', C.muted);
    L(450, 300, 'redirect kisi ka intezaar nahi karta', C.good);
  } else {
    B(20, 130, 120, 60, 'User'); A(140, 145, 258, 145, C.msg, { n: 1 }); B(260, 130, 120, 60, 'App');
    A(258, 180, 142, 180, C.good, { n: 2 }); L(200, 215, '302 turant', C.good);
    B(520, 130, 130, 60, 'Kafka', 'offset: 40', { st: 'acc' });
    A(380, 160, 518, 160, C.acc, { n: 3 }); dots(8, 530, 215, C.acc, { gap: 16, r: 6 }); L(585, 245, 'khabar padi hai, khoti nahi', C.acc);
    B(720, 130, 150, 60, 'Analytics', 'dheema', { st: 'bad' }); bar(720, 200, 150, 12, 0.98, '');
    A(650, 160, 718, 160, C.bad, { n: 4, dash: 1 }); L(795, 240, 'baad me 40 se aage', C.muted);
    L(450, 300, 'redirect pe koi asar nahi, ginti thodi der se aati', C.good);
  }
}
