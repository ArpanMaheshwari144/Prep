// Card 9: ginti IP pe, Ravi ki galti se office atka. Ilaaj: user_id (JWT). Agar: lock karo? Wapas: CAPTCHA.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ginti IP pe. Ravi ke 6 galat login se office IP block, sab atke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: login ke baad ginti user_id pe (JWT se). Ravi ki galti sirf Ravi pe'],
  [6, YEL, 'AGAR', 'AGAR 5 galat password pe account lock? -> attacker tujhe hi lock kar dega'],
  [9, GRN, 'WAPAS', 'WAPAS: lock ki jagah CAPTCHA. Ravi andar, attacker atka'],
];
const NAMES = ['Ravi', 'Asha', 'Mohan', 'Neha', 'Suresh'];
function setup() {}
function office(sts, gsub) {
  NAMES.forEach((n, i) => { B(20, 15 + i * 64, 110, 46, n, '', { st: sts[i] }); A(130, 38 + i * 64, 228, 175); });
  B(230, 140, 110, 70, 'Office', '1 IP');
  A(340, 175, 428, 175); B(430, 140, 130, 70, 'Gateway', gsub, { st: 'acc' });
  B(720, 140, 120, 70, 'App');
}
function frame(ph) {
  if (ph === 0) {
    office(['acc', 'bad', 'bad', 'bad', 'bad'], 'ginti: IP');
    A(560, 175, 718, 175, C.bad, { dash: 1 }); cross(640, 175);
    T(495, 250, 'IP: 10 > 5', { c: C.bad, b: 1, size: 15 });
  } else if (ph === 1) {
    office(['bad', 'good', 'good', 'good', 'good'], 'ginti: user_id');
    A(560, 175, 718, 175, C.good); dots(4, 585, 150, C.good, { gap: 26 });
    T(495, 250, 'Ravi: 6 > 5', { c: C.bad, b: 1, size: 15 });
  } else {
    const cap = ph === 3;
    B(20, 40, 120, 60, 'Attacker', '', { st: 'bad' });
    B(20, 250, 120, 60, 'Ravi', '', { st: cap ? 'good' : 'bad' });
    B(330, 140, 130, 70, cap ? 'CAPTCHA' : 'Gateway', '', { st: 'acc' });
    A(140, 70, 328, 158, C.bad); dots(5, 165, 92, C.bad, { gap: 22 });
    if (!cap) {
      B(620, 140, 190, 70, 'Ravi ka account', 'LOCK', { st: 'bad' });
      A(460, 175, 618, 175, C.bad);
      A(140, 280, 328, 195, C.bad, { dash: 1 }); cross(235, 238);
    } else {
      B(620, 140, 190, 70, 'App', '', { st: 'good' }); A(460, 175, 618, 175, C.good);
      A(140, 280, 328, 195, C.good); dot([235, 238], C.good);
      cross(300, 128);
    }
  }
}
