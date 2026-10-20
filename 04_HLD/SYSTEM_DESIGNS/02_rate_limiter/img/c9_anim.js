// Card 9: ginti IP pe, office ke sab log ek IP ke peeche. Ravi ki galti se sab atke.
// Ilaaj: login ke baad ginti user_id pe (JWT se). Agar: 5 galat password pe lock? -> attacker tujhe hi lock kar dega.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ginti IP pe. Office ke sab ek IP ke peeche, Ravi ki galti se sab block'],
  [3, GRN, 'ILAAJ', 'ILAAJ: login ke baad ginti user_id pe (JWT se). Ravi ki galti sirf Ravi pe'],
  [6, YEL, 'AGAR', 'AGAR 5 galat password pe account lock? -> attacker tujhe hi lock kar dega'],
];
const NAMES = ['Ravi', 'Asha', 'Mohan', 'Neha', 'Suresh'];
function setup() {}
function office(subs, sts) {
  NAMES.forEach((n, i) => {
    B(20, 25 + i * 64, 110, 44, n, subs[i], { st: sts[i] });
    A(130, 47 + i * 64, 228, 180, i === 0 ? C.bad : C.msg, { n: i === 0 ? 1 : 0, at: 0.45 });
  });
  B(230, 145, 130, 70, 'Office NAT', 'sabka IP 1.2.3.4');
}
const STEPS = [
  [[C.bad, 'Ravi → Office NAT', '6 galat login'], [C.msg, 'Office NAT → Gateway', 'sabka ek IP: 1.2.3.4'], [C.bad, 'Gateway → Office NAT', 'IP block: Asha, Mohan, sab atke']],
  [[C.bad, 'Ravi → Office NAT', '6 galat'], [C.msg, 'Office NAT → Gateway', 'ek IP, par JWT alag'], [C.bad, 'Gateway', 'ginti user_id pe: sirf Ravi ko 429'], [C.good, 'Gateway → App', 'baaki andar']],
  [[C.bad, 'Attacker → Arpan ka account', 'Arpan ke naam se 5 galat password'], [C.bad, 'Arpan ka account', 'LOCK lag gaya'], [C.msg, 'Asli Arpan → account', 'sahi password'], [C.bad, 'account → Arpan', 'band, apna hi account']],
];
function frame(ph) {
  if (ph === 0) {
    office(['6 galat login', '', '', '', ''], ['bad', '', '', '', '']);
    B(500, 140, 190, 80, 'Gateway', 'IP 1.2.3.4 = 6 > 5', { st: 'bad' });
    A(360, 170, 498, 170, C.msg, { n: 2 });
    A(498, 200, 362, 200, C.bad, { n: 3 }); L(430, 230, 'IP block', C.bad);
    L(160, 350, 'Asha, Mohan, Neha, Suresh ne kuch nahi kiya, phir bhi sab atke', C.bad, 'start');
  } else if (ph === 1) {
    office(['6 galat: 429', 'andar', 'andar', 'andar', 'andar'], ['bad', 'good', 'good', 'good', 'good']);
    B(500, 120, 210, 120, 'Gateway', '', { st: 'good' });
    cells(515, 160, [['Ravi: 6', C.bad], ['Asha: 1', C.good]], { w: 88, h: 30 });
    T(605, 215, 'ginti user_id pe (JWT)', { c: C.muted, size: 12 });
    A(360, 180, 498, 180, C.msg, { n: 2 });
    num(605, 100, 3, C.bad); L(625, 105, 'sirf Ravi ko 429', C.bad, 'start');
    B(760, 150, 110, 60, 'App', '', { st: 'good' }); A(710, 180, 758, 180, C.good, { n: 4 });
    L(815, 240, 'baaki andar', C.good);
    L(450, 350, 'login se pehle userId nahi hota: wahan IP pe dheeli had + WAF', C.muted);
  } else {
    B(20, 50, 130, 60, 'Attacker', '', { st: 'bad' });
    B(20, 250, 130, 60, 'Asli Arpan', 'sahi password');
    B(320, 140, 200, 80, 'Arpan ka account', 'LOCK (5 galat)', { st: 'bad' });
    A(150, 80, 318, 160, C.bad, { n: 1 }); L(165, 60, 'Arpan ke naam se 5 galat password', C.bad, 'start');
    num(420, 245, 2, C.bad); L(440, 250, 'lock lag gaya', C.bad, 'start');
    A(150, 262, 318, 192, C.msg, { n: 3 }); A(330, 222, 152, 302, C.bad, { n: 4, at: 0.55 }); L(200, 330, 'band, apna hi account', C.bad, 'start');
    B(610, 140, 260, 80, 'hal: lock nahi', 'CAPTCHA ya thodi der ruko', { st: 'good' });
    L(740, 245, 'attacker atka, Arpan andar', C.good);
  }
}
