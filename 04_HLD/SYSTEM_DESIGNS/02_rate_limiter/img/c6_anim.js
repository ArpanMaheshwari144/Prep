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
function row(who, wst, sub) {
  B(20, 130, 110, 80, who, sub); A(130, 170, 198, 170);
  B(200, 140, 100, 60, 'WAF', 'edge', { st: wst }); A(300, 170, 388, 170);
  B(390, 140, 110, 60, 'Gateway', '429', { st: 'acc' }); A(500, 170, 618, 170);
  B(620, 140, 110, 60, 'App');
}
function pipe(kst, pst) {
  A(445, 200, 445, 258); B(390, 260, 110, 50, 'Kafka', '', { st: kst });
  A(500, 285, 588, 285); B(590, 260, 140, 50, 'Pattern Svc', '', { st: pst });
  el('path', { d: 'M660 260 L660 228 L250 228 L250 202', fill: 'none',
    stroke: pst === 'bad' ? C.bad : C.muted, 'stroke-width': 2 }, DYN);
}
function frame(ph) {
  if (ph === 0) {
    row('Bot');
    ['01', '02', '03', '04'].forEach((s, i) => { dot([400 + i * 30, 245], C.bad); T(400 + i * 30, 275, s, { c: C.muted, size: 11 }); });
    T(445, 305, 'minute', { c: C.muted, size: 11 });
    T(445, 120, '429 . 429 . 429', { c: C.bad, b: 1, size: 14 });
  } else if (ph === 1) {
    row('Bot', 'bad'); pipe('', 'bad');
    dots(4, 140, 250, C.bad, { gap: 20 }); T(170, 285, 'BAN', { c: C.bad, b: 1, size: 14 });
    T(660, 335, '500+', { c: C.bad, b: 1, size: 15 });
  } else if (ph === 2) {
    row('Bot', 'dim'); pipe('acc', 'dim');
    bar(395, 318, 100, 16, 0.95, ''); T(445, 350, 'peeche', { c: C.acc, b: 1, size: 12 });
    dots(4, 410, 110, C.bad, { gap: 22 }); T(445, 95, '429', { c: C.bad, b: 1, size: 13 });
  } else {
    const ban = ph === 3;
    row('Office', ban ? 'bad' : 'good', '1 IP');
    dots(5, 25, 240, ban ? C.bad : C.good, { gap: 24 });
    if (ban) { cross(325, 125); T(250, 255, '~24 ghante', { c: C.acc, b: 1, size: 14 }); }
    else { A(500, 170, 618, 170, C.good); dots(3, 525, 125, C.good, { gap: 22 }); }
  }
}
