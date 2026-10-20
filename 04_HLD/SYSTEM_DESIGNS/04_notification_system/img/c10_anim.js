// Card 10: "sabko bhejo" = ek request me 1 crore ek-ek, ghanton atki, OTP peeche.
// Ilaaj: 1000 ke tukde low line me, campaign row me aakhri_id. Agar: 600 bhej ke worker gira, galat offer chala.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sabko bhejo: ek request me 1 crore ek-ek. Ghanton atki, OTP peeche'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 1000-1000 ke tukde, low line me. Har tukde ke baad row me aakhri_id'],
  [6, YEL, 'AGAR', 'AGAR tukde me 600 bhej ke worker gir gaya? -> poora tukda dobara'],
  [9, YEL, 'AGAR', 'AGAR galat offer chal gaya? -> campaign row pe CANCELLED likho'],
  [12, GRN, 'WAPAS', 'WAPAS: tukde low line me chalte rahe. Had + OTP line bachi'],
];
function setup() {}
function base(o = {}) {
  B(20, 150, 100, 70, 'Admin', 'sabko bhejo');
  B(170, 150, 150, 70, 'Campaign Svc', '', { st: 'acc' });
  B(170, 280, 150, 60, 'Campaign row', o.row || 'aakhri_id', { st: o.rowst || 'acc' });
  B(630, 155, 100, 60, 'Worker', o.w || 'low line', { st: o.wst });
  B(760, 150, 110, 70, 'Users', '1 crore', { st: o.ust });
  A(120, 185, 168, 185, C.msg, { n: o.n1 || 0, at: 0.45 });
}
function tukde(list) {
  cells(620 - list.length * 44, 172, list.map(s => [s, C.acc]), { w: 40 });
  T(470, 160, 'tukde (1000 user)', { c: C.muted, size: 12 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 100, 70, 'Admin', 'sabko bhejo');
    B(170, 150, 150, 70, 'Notification Svc', '', { st: 'bad' });
    B(760, 150, 110, 70, 'Users', '1 crore', { st: 'bad' });
    A(120, 185, 168, 185, C.msg, { n: 1, at: 0.45 }); L(70, 135, 'ek request', C.msg);
    A(320, 185, 758, 185, C.acc, { n: 2, at: 0.15 }); dots(11, 420, 185, C.acc, { gap: 28, cols: 11 });
    L(540, 160, 'ek-ek, 1 crore', C.acc); L(540, 230, 'ghanton atki', C.bad);
    num(195, 95, 3, C.bad); dot([245, 120], C.bad); L(245, 100, 'OTP peeche atka', C.bad, 'start');
  } else if (ph === 1) {
    base({ n1: 1, row: 'aakhri_id = 3000', ust: 'good' });
    tukde(['4k', '5k', '6k']);
    A(320, 185, 485, 185, C.acc, { n: 2, at: 0.5 }); A(615, 185, 628, 185, C.line);
    A(730, 185, 758, 185, C.good, { n: 3, at: 0.5 });
    AP([[680, 217], [680, 310], [322, 310]], C.acc, { n: 4, at: 0.5 }); L(500, 335, 'har tukde ke baad likha', C.acc);
  } else if (ph === 2) {
    base({ row: 'aakhri_id = 3000', w: 'gira -> naya', wst: 'acc', ust: 'good' });
    tukde(['4k', '5k', '6k']);
    num(680, 135, 1, C.bad); L(698, 140, '600 bheje, phir gira', C.bad, 'start');
    AP([[322, 300], [680, 300], [680, 217]], C.acc, { n: 2, at: 0.5 }); L(500, 285, 'wahin se: 3001', C.acc);
    A(730, 185, 758, 185, C.good, { n: 3, at: 0.5 });
    L(815, 250, '600 skip, 400 gaye', C.good);
    L(450, 25, 'hal: poora tukda dobara, (campaign_id, user_id) se', C.good);
    L(450, 47, 'pehle wale 600 chhoot jaate. Ek user ko ek hi baar', C.good);
  } else if (ph === 3) {
    base({ n1: 1, row: 'CANCELLED', rowst: 'bad', w: 'ruka', wst: 'bad' });
    tukde(['4k', '5k', '6k']);
    L(70, 135, 'galat offer', C.bad);
    A(245, 222, 245, 278, C.bad, { n: 2 }); L(262, 255, 'CANCELLED likho', C.bad, 'start');
    AP([[680, 217], [680, 310], [322, 310]], C.acc, { n: 3, at: 0.5 }); L(500, 335, 'har tukde se pehle dekha', C.acc);
    A(730, 185, 758, 185, C.bad, { dash: 1 }); cross(745, 160); L(815, 250, 'aage nahi gaya', C.bad);
    L(450, 25, 'hal: worker har tukde se pehle row dekhta, CANCELLED pe ruk', C.good);
  } else {
    base({ row: 'aakhri_id', ust: 'good', wst: 'good' });
    tukde(['7k', '8k']);
    A(320, 185, 485, 185, C.acc, { n: 1, at: 0.6 });
    A(730, 185, 758, 185, C.good, { n: 2, at: 0.5 }); L(815, 250, 'aaram se, had me', C.good);
    B(170, 30, 150, 50, 'OTP', 'tez line', { st: 'good' });
    A(320, 55, 758, 160, C.good, { n: 3, at: 0.5 }); L(600, 70, 'OTP turant', C.good, 'start');
  }
}
