// Card 6: har second 5,000 tweet, 5 saal me 0.5 PB, ek DB me nahi. Ilaaj: Cassandra, user_id se baanto.
// Agar: meri machine giri -> 3 copy, 2 me likho, 2 se padho (quorum).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har second 5,000 tweet, 5 saal me 0.5 PB. Ek DB na itna rakh sake, na likh sake'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Cassandra, user_id se baanto. Mere saare tweet ek machine pe, naya upar. Ek read'],
  [6, YEL, 'AGAR', 'AGAR meri tweet wali machine giri? -> 3 copy, 2 me likho, 2 se padho (quorum)'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Tweet Svc → Ek DB', 'har tweet yahin, 5,000 / sec'], [C.bad, 'Ek DB', 'na jagah (0.5 PB), na itni likhai']],
  [[C.msg, 'Arpan → Tweet Svc', 'tweet'], [C.msg, 'Virat → Tweet Svc', 'tweet'], [C.good, 'Tweet Svc → Node-2', 'user_id dekha: Arpan -> Node-2'], [C.good, 'Tweet Svc → Node-3', 'Virat -> Node-3']],
  [[C.good, 'Tweet Svc → copy A, B', 't9 ek saath 2 me likha: save'], [C.bad, 'copy C giri', 'likhai ruki nahi'], [C.acc, 'padhna', 'A + B se: t9 mila. Har jodi me|ek taaza copy zaroor']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Tweet Svc', '5,000 / sec');
    B(400, 110, 220, 110, 'Ek DB', '', { st: 'bad' }); cross(645, 165);
    bar(420, 160, 180, 16, 1, ''); T(510, 200, 'disk: 0.5 PB?', { c: C.bad, b: 1, size: 13 });
    A(170, 165, 398, 165, C.bad, { n: 1 }); L(285, 145, 'har tweet yahin', C.bad);
    num(510, 255, 2, C.bad); L(510, 280, 'na jagah, na itni likhai', C.bad);
    L(450, 340, 'ek machine ki had hoti hai, tweet ki nahi', C.bad);
  } else if (ph === 1) {
    B(20, 60, 130, 55, 'Arpan', 'tweet');
    B(20, 230, 130, 55, 'Virat', 'tweet');
    B(330, 140, 150, 60, 'Tweet Svc', 'user_id dekho');
    ['Node-1', 'Node-2', 'Node-3'].forEach((n, i) => B(650, 30 + i * 105, 200, 70, n,
      ['', 'Arpan ke saare tweet', 'Virat ke saare tweet'][i], { st: i ? 'good' : '' }));
    A(150, 88, 328, 160, C.msg, { n: 1 }); A(150, 258, 328, 185, C.msg, { n: 2 });
    A(480, 160, 648, 170, C.good, { n: 3 }); L(560, 150, 'Arpan -> Node-2', C.good);
    A(480, 185, 648, 275, C.good, { n: 4 }); L(560, 255, 'Virat -> Node-3', C.good);
    L(450, 340, 'Arpan ki profile kholi: sirf Node-2 se, ek read. Bojh 3 machine pe bat gaya', C.good);
  } else {
    B(20, 140, 130, 60, 'Tweet Svc', 'Arpan: t9');
    ['copy A', 'copy B', 'copy C'].forEach((n, i) => B(560, 30 + i * 110, 200, 65, 'Node-2 ' + n,
      ['t9 likha', 't9 likha', 'gira'][i], { st: i === 2 ? 'bad' : 'good' }));
    cross(785, 282);
    A(150, 160, 558, 62, C.good, { n: 1, at: 0.6 }); A(150, 170, 558, 172, C.good); // ek kadam = ek number
    L(440, 235, 'ek saath A + B me likha: save', C.good);
    num(660, 345, 2, C.bad); L(680, 350, 'C giri', C.bad, 'start');
    num(40, 290, 3, C.acc); L(60, 295, 'padhna bhi A + B se: t9 mila', C.acc, 'start');
    L(450, 20, '3 me se 2 likhe, 2 padhe: har jodi me ek taaza copy zaroor', C.good);
  }
}
