// Card 3: Ravi ne naam do baar badla, purana naam reh gaya. Ilaaj: key = userId, ek partition me kram. Agar: producer retry, ulta pahuncha.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: bina key, Ravi ki do khabar alag tukde me. Baad wali pehle lagi, purana naam reh gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: key = userId. Ravi ki har khabar ek hi tukde (P1) me, tukde ke andar kram pakka'],
  [6, YEL, 'AGAR', 'AGAR retry se khabar ulti pahunchi? -> har khabar ka number, broker kram pakka (idempotent)'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Ravi → P1, P4', 'Naam do baar badla: "Ravi K", phir "Ravi Kumar"; bina key alag tukde me gaye'],
   [C.bad, 'P4 → DB', 'P4 ka worker tez tha, naya "Ravi Kumar" pehle likh diya'],
   [C.bad, 'P1 → DB', 'Baad me purana "Ravi K" upar likha, galat naam reh gaya']],
  [[C.msg, 'Ravi → P1', 'Key = userId: Ravi ki dono khabar hash se ek hi tukde P1 me'],
   [C.acc, 'P1', 'Tukde ke andar kram pakka: pehle "Ravi K", phir "Ravi Kumar"'],
   [C.good, 'P1 → DB', 'Worker kram se likhta, aakhir me sahi naam "Ravi Kumar"']],
  [[C.bad, 'Producer → P1', 'Khabar #1 ka jawab nahi aaya, tab tak khabar #2 chali gayi'],
   [C.msg, 'Producer → P1', 'Khabar #1 retry me phir bheji'],
   [C.good, 'P1', 'Har khabar ka number: broker #1 ke baad hi #2 lagata, kram sahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 140, 170, 60, 'Ravi', 'do baar badla', { icon: 'user' });
    B(320, 40, 170, 55, 'P1', '"Ravi K"', { st: 'acc' }); B(320, 240, 170, 55, 'P4', '"Ravi Kumar"', { st: 'acc' });
    A(185, 160, 318, 70, C.msg, { n: 1 }); A(185, 180, 318, 265, C.msg);
    B(680, 140, 200, 60, 'DB', 'naam = Ravi K', { st: 'bad', icon: 'db' });
    A(490, 265, 678, 185, C.bad, { n: 2 }); L(600, 285, 'tez: pehle likha', C.bad);
    A(490, 65, 678, 155, C.bad, { n: 3 }); L(600, 60, 'baad me: upar likha', C.bad);
    L(780, 240, 'purana naam reh gaya', C.bad);
  } else if (ph === 1) {
    B(15, 140, 170, 60, 'Ravi', 'key = userId', { icon: 'user' });
    B(300, 120, 250, 100, 'P1', '', { st: 'good' });
    cells(315, 160, [['1: Ravi K', C.acc], ['2: Ravi Kumar', C.acc]], { w: 112, h: 36 }); num(425, 245, 2, C.acc);
    L(425, 275, 'tukde ke andar kram', C.acc);
    A(185, 170, 298, 170, C.msg, { n: 1 }); L(240, 140, 'hash', C.msg);
    B(680, 140, 200, 60, 'DB', 'naam = Ravi Kumar', { st: 'good', icon: 'db' });
    A(552, 170, 678, 170, C.good, { n: 3 });
    L(450, 330, 'jiska kram chahiye, wahi key', C.muted);
  } else {
    B(15, 140, 170, 60, 'Producer', '#1, #2 bheje', { icon: 'server' });
    A(185, 155, 378, 115, C.bad, { n: 1, dash: 1, at: 0.4 }); L(270, 105, '#1 jawab nahi', C.bad);
    A(185, 185, 378, 225, C.msg, { n: 2, at: 0.4 }); L(270, 245, '#1 retry', C.msg);
    B(380, 90, 260, 160, 'P1 (broker)', '', { st: 'good' });
    cells(400, 150, [['#1', C.good], ['#2', C.good]], { w: 110, h: 36 }); num(670, 168, 3, C.good);
    L(510, 290, 'number dekh ke kram sahi (idempotent producer)', C.good);
  }
}
