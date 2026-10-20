// Card 6: ek DB, 5,000 tweet/sec, 0.5 PB -> na rakh sake na likh sake. Ilaaj: Cassandra, user_id se baanto.
// Agar: files bahut, machine giri, famous garam, famous bhar jaaye. Wapas: ek machine, ek read.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi DB. Har second 5,000 tweet, 5 saal me 0.5 PB'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Cassandra, user_id se baanto. Mere saare tweet ek machine pe'],
  [6, YEL, 'AGAR', 'AGAR Cassandra me files bahut? -> padhna dheema. Peeche milao (compaction)'],
  [9, YEL, 'AGAR', 'AGAR meri machine giri? -> 3 copy, 2 me likho, 2 se padho (quorum)'],
  [12, YEL, 'AGAR', 'AGAR famous ki machine garam? -> theek hai, garam tweet alag RAM cache me'],
  [15, YEL, 'AGAR', 'AGAR famous ke bahut tweet, machine bhare? -> key (user_id, mahina)'],
  [18, GRN, 'WAPAS', 'WAPAS: mere saare tweet ek machine pe, naya sabse upar. Ek hi read'],
];
function setup() {}
function cass(st2, sub, o = {}) {
  for (let i = 0; i < 3; i++) {
    const y = 30 + i * 120;
    B(560, y, 160, 60, `Cassandra ${i + 1}`, i === 1 ? sub : '', { st: i === 1 ? st2 : '' });
    if (o.hash) A(360, 185, 558, y + 30, i === 1 ? C.acc : C.line, { n: i === 1 ? 2 : 0, at: 0.6 });
  }
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 110, 70, 'Tweets', '5,000 / sec');
    A(130, 185, 378, 185, C.msg, { n: 1, at: 0.15 });
    dots(5, 220, 185, C.bad, { gap: 32 }); L(285, 220, 'likhne ki line', C.bad);
    B(380, 130, 170, 110, 'Ek DB', '', { st: 'bad' });
    bar(395, 190, 140, 18, 1, 'disk');
    num(600, 165, 2, C.bad); L(620, 170, 'likh nahi paata', C.bad, 'start');
    num(600, 215, 3, C.bad); L(620, 220, 'disk bhari: 0.5 PB', C.bad, 'start');
  } else if (ph === 1 || ph === 6) {
    const w = ph === 6;
    if (!w) {
      B(20, 150, 110, 70, 'Tweets', '5,000 / sec');
      A(130, 185, 218, 185, C.msg, { n: 1 });
      B(220, 155, 140, 60, 'hash(user_id)', '', { st: 'acc' });
      cass('acc', 'Main', { hash: 1 });
      L(800, 170, 'mere saare', C.acc); L(800, 190, 'tweet yahin', C.acc);
    } else {
      B(20, 150, 120, 70, 'Main', 'profile kholi', { st: 'good' });
      cass('good', 'Main');
      A(140, 185, 558, 185, C.good, { n: 1 }); L(350, 210, 'ek hi read', C.good);
      cells(740, 172, [['naya', C.good], ['..', C.fg]], { w: 50, h: 26 });
    }
  } else if (ph === 2) {
    B(20, 70, 120, 60, 'padhna');
    B(300, 20, 570, 230, 'Cassandra 2');
    const fs = []; for (let i = 1; i <= 8; i++) fs.push([`f${i}`, C.bad]);
    cells(320, 60, fs, { w: 54, h: 28 });
    A(140, 95, 298, 80, C.bad, { dash: 1, n: 1 }); L(210, 150, 'padhna dheema', C.bad);
    A(420, 92, 420, 163, C.good, { n: 2 }); L(440, 132, 'peeche milao (compaction)', C.good, 'start');
    cells(320, 168, [['f1-8 mili hui', C.good]], { w: 200, h: 30 });
    num(600, 183, 3, C.good); cells(620, 168, [['bloom filter', C.good]], { w: 140, h: 30 });
    L(690, 225, 'kam file padho', C.good);
    L(450, 305, 'hal: peeche files milao (compaction) + bloom filter', C.good);
  } else if (ph === 3) {
    B(20, 150, 120, 60, 'Tweet Svc');
    for (let i = 0; i < 3; i++) {
      const y = 30 + i * 120;
      B(450, y, 180, 60, `Cassandra ${i + 1}`, 'meri copy', { st: i === 1 ? 'bad' : 'good' });
      if (i !== 1) A(140, 180, 448, y + 30, C.good, { n: 2, at: 0.6 });
    }
    A(140, 180, 415, 180, C.bad, { dash: 1 }); cross(432, 180);
    num(660, 180, 1, C.bad); L(680, 185, 'giri', C.bad, 'start');
    L(760, 70, 'likha + padha', C.good); L(760, 310, 'likha + padha', C.good);
    L(220, 300, 'hal: 3 copy, 2 me likho, 2 se padho (quorum)', C.good);
  } else if (ph === 4) {
    B(20, 150, 120, 60, 'Feed Svc');
    B(430, 40, 200, 65, 'garam tweet cache', 'RAM', { st: 'good' });
    B(430, 230, 200, 65, 'Cassandra 2', 'Bieber ke tweet', { st: 'acc' });
    num(660, 262, 1, C.acc); L(680, 267, 'garam', C.acc, 'start');
    A(140, 165, 428, 75, C.good, { n: 2 }); L(250, 95, 'zyada read yahin', C.good);
    A(140, 195, 428, 262, C.line);
    L(720, 175, 'hal: theek hai, garam tweet RAM cache me', C.good);
  } else if (ph === 5) {
    B(20, 30, 230, 60, 'Cassandra 1', 'Bieber: saare tweet', { st: 'bad' });
    bar(30, 105, 210, 14, 1, '');
    num(40, 145, 1, C.bad); L(60, 150, 'ek machine bhar jaati', C.bad, 'start');
    B(20, 210, 230, 60, 'key = (user_id, mahina)', '', { st: 'acc' });
    ['Jan', 'Feb', 'Mar'].forEach((m, i) => {
      const y = 30 + i * 100;
      B(480, y, 220, 60, `Cassandra ${i + 1}`, `(Bieber, ${m})`, { st: 'good' });
      A(250, 240, 478, y + 30, C.good, { n: i === 1 ? 2 : 0, at: 0.55 });
    });
    L(450, 335, 'hal: key me mahina jodo, ek machine nahi bharegi', C.good);
  }
}
