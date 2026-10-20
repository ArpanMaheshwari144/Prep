// Card 2: usi request me 5,000 inbox -> button 5 sec ghooma. Ilaaj: DB save + Kafka, workers peeche.
// Agar: Kafka fail, apna tweet, worker gira, workers kam. Wapas: worker phir chala, tweet ek hi baar.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: usi request me 5,000 inbox ek-ek karke. 5 sec ghooma, "fail" dikha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: tweet DB me save, khabar Kafka me, turant "ho gaya". Workers peeche'],
  [6, YEL, 'AGAR', 'AGAR tweet DB me save, Kafka me khabar fail? -> Outbox ya retry lagao'],
  [9, YEL, 'AGAR', 'AGAR fanout peeche, mera tweet meri feed me kab? -> seedha apni feed me'],
  [12, YEL, 'AGAR', 'AGAR worker 100 me se 60 bhar ke gira? -> commit nahi, khabar phir aayegi'],
  [15, YEL, 'AGAR', 'AGAR bheed me workers kam pad gaye? -> worker badhao, partition bat jaati'],
  [18, GRN, 'WAPAS', 'WAPAS: worker phir chala, baaki 40 bhare. Wahi 60 dobara, tweet ek baar'],
];
function setup() {}
// o: rang / badge har teer ke liye
function base(o = {}) {
  B(20, 150, 110, 70, 'Main', '5,000 follower');
  A(130, 175, 188, 175, o.m || C.line, { n: o.mn });
  B(190, 155, 110, 60, 'Tweet Svc');
  B(380, 30, 120, 55, 'Tweet DB', '', { st: o.dbst });
  A(275, 153, 378, 65, o.db || C.line, { n: o.dbn });
  B(380, 280, 120, 55, 'Kafka', '', { st: o.kst });
  A(275, 217, 378, 300, o.k || C.line, { n: o.kn, dash: o.kd, at: o.kat });
  B(580, 280, 120, 55, 'Workers', '', { st: o.wst });
  A(500, 307, 578, 307, o.kw || C.line, { n: o.kwn, dash: o.kwd });
  B(740, 150, 140, 70, o.il || '5,000 inbox', o.is || '', { st: o.ist });
  A(700, 295, 778, 222, o.wi || C.line, { n: o.win, dash: o.wid });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 110, 70, 'Main', '5,000 follower', { st: 'bad' });
    A(130, 175, 188, 175, C.msg, { n: 1 });
    B(190, 155, 110, 60, 'Tweet Svc', '', { st: 'bad' });
    B(380, 30, 120, 55, 'Tweet DB', '', { st: 'good' });
    A(275, 153, 378, 65, C.good, { n: 2 }); L(520, 62, 'tweet ban chuka tha', C.good, 'start');
    A(300, 185, 738, 185, C.acc, { n: 3, at: 0.12 });
    dots(5, 420, 185, C.acc, { gap: 60 }); L(530, 215, 'ek-ek karke 5,000', C.acc);
    B(740, 150, 140, 70, '5,000 inbox', '', { st: 'bad' });
    bar(750, 235, 120, 14, 0.3, '');
    num(35, 250, 4, C.bad); L(55, 255, '5 sec ghooma, "fail"', C.bad, 'start');
  } else if (ph === 1) {
    base({ m: C.msg, mn: 1, db: C.good, dbn: 2, dbst: 'good', k: C.acc, kn: 2, kst: 'acc',
      kw: C.acc, kwn: 4, wst: 'acc', wi: C.acc, win: 5, ist: 'good' });
    A(188, 205, 132, 205, C.good, { n: 3 }); L(75, 250, '"ho gaya" turant', C.good);
    L(810, 250, 'peeche bhare', C.good);
  } else if (ph === 2) {
    base({ m: C.msg, mn: 1, db: C.good, dbn: 2, dbst: 'good', k: C.bad, kn: 3, kd: 1, kat: 0.3, kst: 'bad' });
    cross(345, 275); L(255, 300, 'khabar fail', C.bad);
    B(560, 30, 130, 55, 'Outbox', 'DB ke saath', { st: 'good' });
    A(500, 57, 558, 57, C.good);
    A(600, 87, 470, 278, C.good, { dash: 1, n: 4, at: 0.5 }); L(555, 200, 'retry', C.good, 'start');
    L(800, 115, 'hal: Outbox / retry', C.good);
  } else if (ph === 3) {
    base({ m: C.msg, mn: 1, k: C.acc, kw: C.acc, kwn: 3, kwd: 1, wi: C.acc, wid: 1 });
    B(190, 40, 130, 55, 'meri feed', '', { st: 'good' });
    A(245, 153, 245, 97, C.good, { n: 2 }); L(160, 125, 'turant', C.good, 'end');
    L(640, 360 - 100, 'peeche', C.acc);
    L(620, 120, 'hal: apna tweet seedha apni feed me', C.good);
  } else if (ph === 4) {
    base({ wi: C.good, win: 1, il: 'inbox', is: '60 / 100', ist: 'acc', wst: 'bad', kw: C.acc, kwn: 3, kwd: 1 });
    L(820, 250, '60 bhare, 40 nahi', C.good);
    cross(560, 255); num(600, 262, 2, C.bad); L(620, 267, 'gira', C.bad, 'start');
    L(540, 355 - 10, 'commit nahi kiya', C.acc);
    L(620, 120, 'hal: commit nahi, khabar phir aayegi', C.good);
  } else if (ph === 5) {
    B(20, 150, 110, 60, 'Tweet Svc');
    B(180, 25, 200, 300, 'Kafka');
    A(130, 180, 178, 180, C.acc, { n: 1 }); L(75, 240, 'bheed', C.bad);
    for (let i = 0; i < 4; i++) {
      const y = 65 + i * 62, nw = i >= 2;
      B(200, y, 160, 44, `partition ${i}`);
      B(520, y, 140, 44, `Worker ${i + 1}`, '', { st: nw ? 'acc' : 'good' });
      A(360, y + 22, 518, y + 22, C.good, { n: i === 1 ? 3 : 0 });
      A(660, y + 22, 758, 185, C.line);
    }
    num(540, 318, 2, C.acc); L(560, 323, 'naye worker', C.acc, 'start');
    dots(4, 230, 295 + 15, C.bad, { gap: 20 });
    B(760, 150, 120, 70, 'inbox');
    L(560, 345, 'hal: worker badhao, partition bat jaati', C.good);
  } else {
    base({ kw: C.good, kwn: 1, wst: 'good', wi: C.good, win: 2, il: 'inbox', is: '100 / 100', ist: 'good' });
    L(540, 355 - 10, 'khabar phir aayi', C.good);
    L(810, 250, '40 bhare', C.good);
    L(760, 125, 'wahi 60 dobara: ek hi baar', C.good);
  }
}
