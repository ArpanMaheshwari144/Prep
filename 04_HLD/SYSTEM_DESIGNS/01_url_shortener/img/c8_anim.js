// Card 8: spammer script 5,000 link / sec. Ilaaj: gateway pe API key ki ek ginti, 429. Agar: pehchaana hamla -> WAF. Wapas.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: spammer ki script, har second 5,000 link, sab andar tak'],
  [3, GRN, 'ILAAJ', 'ILAAJ: gateway pe har API key ki ek ginti (Redis). Had paar to 429'],
  [6, YEL, 'AGAR', 'AGAR darwaze pe pehchaana hua hamla aaya? -> WAF darwaze pe hi rok deta'],
  [9, GRN, 'WAPAS', 'WAPAS: rokna darwaze pe, asli users ki speed bachi rehti'],
];
function setup() {}
function people(sst) { B(20, 40, 120, 60, 'Spam script', '5,000 / sec', { st: sst }); B(20, 260, 120, 60, 'Users', ''); }
function gw(st, sub) { B(330, 150, 130, 70, 'Gateway', sub, { st }); }
function back(st, sub) { B(560, 150, 110, 70, 'App', sub, { st }); B(750, 150, 110, 70, 'DB', '', { st }); }
function frame(ph) {
  if (ph === 0) {
    people('bad'); back('bad', '');
    A(140, 70, 558, 170, C.bad, { n: 1, at: 0.3 }); L(250, 70, 'sab andar', C.bad, 'start');
    A(670, 185, 748, 185, C.bad, { n: 2 }); L(710, 140, 'bhar gaya', C.bad);
    A(140, 290, 558, 200, C.msg, { dash: 1, n: 3, at: 0.3 }); L(300, 300, 'asli users: dheema', C.msg, 'start');
  } else if (ph === 1) {
    people('bad'); gw('acc', 'ginti: API key'); back('good', '');
    B(330, 280, 130, 50, 'Redis', 'ek ginti');
    A(140, 70, 328, 165, C.bad, { n: 1, at: 0.4 });
    A(395, 220, 395, 278, C.acc, { n: 2 }); L(405, 255, 'gino', C.acc, 'start');
    num(345, 132, 3, C.bad); L(363, 137, '429, App tak nahi', C.bad, 'start');
    A(140, 290, 328, 205, C.good, { n: 4, at: 0.4 }); A(460, 185, 558, 185, C.good);
    L(510, 165, 'asli users', C.good); A(670, 185, 748, 185, C.good);
  } else if (ph === 2) {
    B(20, 40, 120, 60, 'Hamla', 'pehchaana', { st: 'bad' }); B(20, 260, 120, 60, 'Users', '');
    B(190, 150, 90, 70, 'WAF', '', { st: 'acc' }); gw('', ''); back('', '');
    A(140, 70, 188, 165, C.bad, { n: 1 }); cross(235, 125);
    num(268, 125, 2, C.bad); L(286, 130, 'WAF ne roka, aage nahi', C.bad, 'start');
    A(140, 290, 188, 205, C.good, { n: 3 }); A(280, 195, 328, 195, C.good);
    A(460, 185, 558, 185, C.good); A(670, 185, 748, 185, C.good);
    L(560, 300, 'hal: WAF darwaze pe hi rok deta', C.good, 'start');
  } else {
    people('bad'); gw('good', ''); back('good', '');
    B(330, 280, 130, 50, 'Redis', 'ek ginti');
    A(140, 70, 328, 165, C.bad, { n: 1 }); cross(355, 128); L(378, 133, '429, darwaze pe ruka', C.bad, 'start');
    A(140, 290, 328, 205, C.good, { n: 2, at: 0.4 }); A(460, 185, 558, 185, C.good);
    A(670, 185, 748, 185, C.good); L(610, 255, 'speed bachi', C.good);
  }
}
