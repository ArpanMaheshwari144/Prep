// Card 1: Main aur Ravi ne ek second me A1 book ki, dono ko mili. Ilaaj: atomic UPDATE ... WHERE available.
// Agar: network toota (CP / AP), 4 seat me A4 gayi (ROLLBACK), ulte kram (deadlock). Wapas: Ravi ne phir dabaya.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 10:00:01 pe Main aur Ravi dono ne A1 khaali padhi, dono ne book ki'],
  [3, GRN, 'ILAAJ', 'ILAAJ: dekhna + book ek atomic query. Row lock: Main ko 1 row, Ravi ko 0'],
  [6, YEL, 'AGAR', 'AGAR network toota? -> booking: CP, mana karo. search: AP, purani list'],
  [9, YEL, 'AGAR', 'AGAR 4 seat li, A4 pehle hi ja chuki thi? -> badli rows 4 nahi, ROLLBACK'],
  [12, YEL, 'AGAR', 'AGAR do log ulte kram me 4 seat le rahe? -> DB deadlock pakad ke ek abort'],
  [15, GRN, 'WAPAS', 'WAPAS: Ravi ne Book phir dabaya -> WHERE ko phir 0 row, kuch nahi milta'],
];
function setup() {}
function ppl(mSub, mSt, rSub, rSt) {
  B(20, 50, 130, 60, 'Main', mSub, { st: mSt }); if (rSub !== null) B(20, 250, 130, 60, 'Ravi', rSub, { st: rSt });
}
function appDb(dbSub) { B(300, 145, 120, 80, 'App'); B(620, 135, 230, 90, 'SQL DB', dbSub, { st: 'acc' }); }
function seatA1(st, c) { cells(650, 245, [['A1', c], [st, c]], { w: 80 }); }
function frame(ph) {
  if (ph === 0) {
    T(880, 22, '10:00:01', { a: 'end', c: C.muted, size: 13, b: 1 });
    ppl('Book A1', '', 'Book A1', ''); appDb('seats table');
    A(150, 85, 298, 165, C.msg, { n: 1 }); A(150, 275, 298, 205, C.msg, { n: 1 });
    A(420, 165, 618, 162, C.acc, { n: 2 }); L(520, 140, 'SELECT: dono ko A1 khaali', C.acc);
    A(420, 205, 618, 205, C.bad, { n: 3 }); L(520, 240, 'dono ne SET booked', C.bad);
    seatA1('booked', C.bad);
    num(650, 312, 4, C.bad); T(672, 318, 'A1 ke ticket: 2', { a: 'start', c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    ppl('A1 meri', 'good', 'seat ja chuki', 'bad'); appDb('row lock');
    A(150, 85, 298, 165, C.msg, { n: 1 }); A(150, 275, 298, 205, C.msg, { n: 1 });
    A(420, 165, 618, 162, C.good, { n: 2 }); L(520, 140, 'Main: UPDATE -> 1 row', C.good);
    A(420, 205, 618, 205, C.bad, { n: 3 }); L(520, 240, 'Ravi ruka, phir 0 row', C.bad);
    seatA1('booked', C.good);
    T(450, 340, "UPDATE seats SET status = 'booked' WHERE seat_id = 'A1' AND status = 'available'",
      { c: C.muted, size: 12 });
  } else if (ph === 2) {
    ppl('mana (CP)', 'bad', 'search', 'good'); appDb('seats');
    B(620, 255, 230, 60, 'movie list', '2 sec purani', { st: 'good' });
    num(470, 118, 1, C.bad); L(490, 123, 'network toota', C.bad, 'start');
    A(420, 165, 618, 165, C.bad, { dash: 1 }); cross(560, 165);
    A(150, 95, 298, 172, C.msg, { n: 2 }); A(298, 150, 152, 62, C.bad, { n: 3, at: 0.7 });
    L(250, 50, 'pakka nahi pata: mana', C.bad);
    A(150, 275, 298, 205, C.msg, { n: 2 }); A(420, 210, 618, 280, C.good, { n: 3 });
    L(585, 235, 'AP: list chalti', C.good);
    L(450, 342, 'hal: booking me CP (sahi pehle), search me AP (chalta rahe)', C.good);
  } else if (ph === 3) {
    ppl('A1-A4 chahiye', '', null); appDb('ek transaction');
    A(150, 85, 298, 165, C.msg, { n: 1 });
    A(420, 175, 618, 175, C.acc, { n: 2 }); L(520, 152, '4 UPDATE', C.acc);
    cells(640, 245, [['A1', C.acc], ['A2', C.acc], ['A3', C.acc], ['A4', C.bad]], { w: 46 });
    L(805, 292, 'pehle se gayi', C.bad);
    num(650, 320, 3, C.bad); T(670, 325, 'badli rows: 3, 4 nahi', { a: 'start', c: C.bad, b: 1, size: 14 });
    A(422, 210, 618, 210, C.bad, { n: 4 }); L(520, 237, 'ROLLBACK', C.bad);
    L(200, 300, 'hal: ROLLBACK, aadhi booking bekaar', C.good);
  } else if (ph === 4) {
    ppl('A1 phir A4', '', 'A4 phir A1', 'acc');
    B(540, 20, 330, 320, 'SQL DB', '', { st: 'acc' });
    B(620, 70, 180, 60, 'A1', 'lock: Main'); B(620, 250, 180, 60, 'A4', 'lock: Ravi');
    A(150, 72, 618, 95, C.good, { n: 1 }); A(150, 285, 618, 285, C.good, { n: 1 });
    A(150, 98, 618, 262, C.bad, { dash: 1, n: 2, at: 0.25 }); A(150, 262, 618, 118, C.bad, { dash: 1, n: 2, at: 0.25 });
    L(85, 134, 'A4 ka intezaar', C.bad); L(85, 236, 'A1 ka intezaar', C.bad);
    num(600, 190, 3, C.bad); L(710, 195, 'deadlock: Ravi abort', C.bad);
    L(270, 340, 'hal: Ravi retry kare. Alag UPDATE to seat_id ke kram me', C.good);
  } else {
    ppl('A1 meri', 'good', 'seat ja chuki', 'bad'); appDb('seats');
    A(150, 275, 298, 205, C.msg, { n: 1 }); L(215, 220, 'Book (dobara)', C.msg);
    A(420, 205, 618, 205, C.bad, { n: 2 }); L(520, 240, 'WHERE: phir 0 row', C.bad);
    seatA1('booked', C.good); L(750, 305, 'Main ki hi', C.good);
  }
}
