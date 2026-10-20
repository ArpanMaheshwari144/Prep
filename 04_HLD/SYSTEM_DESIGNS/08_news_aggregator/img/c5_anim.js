// Card 5: chhota source 30 sec atka, fetcher poora jawab tak ruka, baaki 999 bhi ruke.
// Ilaaj: timeout + thodi retry, baar-baar fail to breaker OPEN. Agar: 429, kuch din OPEN, HALF-OPEN. Wapas: CLOSED.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: chhota source 30 sec atka, fetcher ruka, baaki 999 bhi ruke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: timeout 2-3 sec, thodi retry. Baar-baar fail: breaker OPEN'],
  [6, YEL, 'AGAR', 'AGAR source ne 429 diya? -> dheere chalo, samay badhao, Retry-After maano'],
  [9, YEL, 'AGAR', 'AGAR breaker kuch din se OPEN? -> alert, source ko list se bahar karo'],
  [12, YEL, 'AGAR', 'AGAR OPEN hai, wapas kab chalu? -> thodi der baad HALF-OPEN: ek test call'],
  [15, GRN, 'WAPAS', 'WAPAS: test call theek nikla, breaker CLOSED. Source ki khabar phir aayi'],
];
function setup() {}
function base(o = {}) {
  B(30, 140, 130, 80, 'Fetcher', o.f, { st: o.fst });
  B(700, 40, 170, 60, 'chhota source', o.s, { st: o.sst });
  B(700, 150, 170, 60, 'source 2', '');
  B(700, 260, 170, 60, 'baaki 999', '');
  if (o.br !== undefined) B(400, 40, 150, 60, 'Breaker', o.br, { st: o.brst });
}
// Fetcher se source 2 aur baaki 999 tak
function others(c, n) { A(160, 190, 698, 180, c, { n, at: 0.6 }); A(160, 205, 698, 290, c, { n, at: 0.6 }); }
function frame(ph) {
  if (ph === 0) {
    base({ fst: 'bad', f: 'ruka', s: '30 sec atka', sst: 'bad' });
    A(160, 160, 698, 70, C.bad, { n: 1, at: 0.4 }); L(330, 85, 'poora jawab tak ruko', C.bad);
    others(C.line); dots(3, 565, 225, C.acc, { gap: 22, hollow: 1 });
    num(538, 225, 2, C.acc); L(615, 254, 'peeche khade', C.acc);
    T(330, 330, 'nayi khabar aani ruki', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    base({ fst: 'good', s: 'fail', sst: 'bad', br: 'OPEN', brst: 'bad' });
    A(160, 160, 398, 75, C.acc, { n: 1 }); L(230, 95, 'timeout 2-3 sec', C.acc);
    num(415, 125, 2, C.bad); L(433, 130, 'baar-baar fail -> OPEN', C.bad, 'start');
    A(550, 70, 698, 70, C.bad, { dash: 1 }); cross(624, 70); L(624, 30, 'call nahi', C.bad);
    others(C.good, 3); L(430, 335, 'baaki chalte', C.good);
  } else if (ph === 2) {
    base({ fst: 'acc', f: 'dheere', s: '429', sst: 'acc' });
    A(160, 155, 698, 60, C.acc, { n: 1, at: 0.35 }); L(300, 90, 'khabar do', C.acc);
    A(698, 85, 160, 175, C.bad, { n: 2, at: 0.6 }); L(420, 165, '429 + Retry-After', C.bad);
    others(C.good);
    L(20, 330, 'hal: dheere chalo, uska samay badhao, Retry-After maano', C.good, 'start');
  } else if (ph === 3) {
    base({ fst: 'good', s: 'band', sst: 'dim', br: 'OPEN: kuch din', brst: 'bad' });
    num(385, 22, 1, C.bad); L(403, 27, 'kuch din se OPEN', C.bad, 'start');
    B(190, 40, 130, 55, 'Alert', '', { st: 'acc' });
    A(398, 67, 322, 67, C.acc, { n: 2 }); L(360, 122, 'alert bhejo', C.acc);
    cross(675, 70); num(712, 122, 3, C.bad); L(730, 127, 'list se bahar', C.bad, 'start');
    others(C.good); L(20, 330, 'hal: alert bhejo, source ko list se bahar karo', C.good, 'start');
  } else if (ph === 4) {
    base({ fst: 'good', s: '', sst: 'acc', br: 'HALF-OPEN', brst: 'acc' });
    num(415, 125, 1, C.acc); L(433, 130, 'thodi der baad', C.acc, 'start');
    A(550, 70, 698, 70, C.acc, { n: 2 }); L(624, 30, 'ek test call', C.acc);
    others(C.good); L(20, 330, 'hal: test theek nikla to CLOSED', C.good, 'start');
  } else {
    base({ fst: 'good', s: 'chalu', sst: 'good', br: 'CLOSED', brst: 'good' });
    A(698, 85, 552, 85, C.good, { n: 1 }); L(624, 115, 'test theek', C.good);
    num(415, 125, 2, C.good); L(433, 130, 'CLOSED', C.good, 'start');
    A(160, 160, 398, 75, C.good, { n: 3 }); A(550, 55, 698, 55, C.good, { n: 3 });
    L(624, 30, 'khabar phir aayi', C.good);
    others(C.good, 3);
  }
}
