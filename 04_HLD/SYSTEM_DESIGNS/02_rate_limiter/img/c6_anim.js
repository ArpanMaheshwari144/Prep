// Card 6: bot har 60 sec phir. Ilaaj: 429 -> Kafka -> Pattern Svc -> WAF ban. Agar: Kafka peeche, galat ban. Wapas.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 429 bolta 60 sec baad aao. Bot har 60 sec phir aata, din bhar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har 429 Kafka me. 10 min me 500+ 429 -> WAF block list, edge pe roko'],
  [6, YEL, 'AGAR', 'AGAR Kafka peeche chal raha? -> ban kuch minute late, rate limit rok raha'],
  [9, YEL, 'AGAR', 'AGAR office ka IP ban ho gaya? -> ban ~24 ghante me khud hat jaata'],
  [12, GRN, 'WAPAS', 'WAPAS: ban apne aap hata, office phir andar'],
];
function setup() {}
function row(who, wst, sub, n1, n2, appC, n3) {
  B(20, 130, 110, 80, who, sub); A(130, 170, 198, 170, n1 ? C.msg : C.line, { n: n1 });
  B(200, 140, 100, 60, 'WAF', 'edge', { st: wst }); A(300, 170, 388, 170, n2 ? C.msg : C.line, { n: n2 });
  B(390, 140, 110, 60, 'Gateway', '429', { st: 'acc' }); A(500, 170, 618, 170, appC || C.line, { n: n3 });
  B(620, 140, 110, 60, 'App');
}
function pipe(kst, pst, n) {
  B(390, 260, 110, 50, 'Kafka', '', { st: kst }); B(590, 260, 140, 50, 'Pattern Svc', '', { st: pst });
  A(445, 200, 445, 258, n ? C.acc : C.line, { n: n && n[0], at: 0.8 });
  A(500, 285, 588, 285, n && n[1] ? C.acc : C.line, { n: n && n[1] });
  AP([[660, 260], [660, 228], [250, 228], [250, 202]], n && n[2] ? C.bad : C.line, { n: n && n[2], at: 0.75 });
}
function frame(ph) {
  if (ph === 0) {
    row('Bot', '', '', 1, 2);
    num(370, 245, 3, C.bad);
    ['01', '02', '03', '04'].forEach((s, i) => { dot([400 + i * 30, 245], C.bad); T(400 + i * 30, 275, s, { c: C.muted, size: 11 }); });
    T(445, 300, 'minute', { c: C.muted, size: 11 });
    L(445, 120, '429, phir aaya, 429 ...', C.bad);
  } else if (ph === 1) {
    row('Bot', 'bad', '', 0, 0); pipe('', 'bad', [1, 2, 3]);
    L(250, 120, 'block list: bot', C.bad);
    num(115, 250, 4, C.bad); dots(4, 140, 250, C.bad, { gap: 20 }); T(170, 285, 'BAN', { c: C.bad, b: 1, size: 14 });
    T(660, 335, '500+', { c: C.bad, b: 1, size: 15 });
  } else if (ph === 2) {
    row('Bot', 'dim', '', 1, 2); pipe('acc', 'dim', [4, 0, 0]);
    L(445, 345, 'Kafka peeche'); bar(395, 318, 100, 16, 0.95, '');
    num(390, 110, 3, C.bad); dots(4, 415, 110, C.bad, { gap: 22 }); L(450, 90, 'rate limit rok raha', C.bad);
    L(250, 255, 'ban abhi nahi', C.muted);
  } else {
    const ban = ph === 3;
    row('Office', ban ? 'bad' : 'good', '1 IP', 1, ban ? 0 : 2, ban ? C.line : C.good, ban ? 0 : 3);
    dots(5, 25, 240, ban ? C.bad : C.good, { gap: 24 });
    if (ban) { cross(345, 170); L(250, 120, 'office IP ban', C.bad); T(250, 255, '~24 ghante', { c: C.acc, b: 1, size: 14 }); }
    else { L(250, 120, 'ban hata', C.good); dots(3, 525, 125, C.good, { gap: 22 }); }
  }
}
