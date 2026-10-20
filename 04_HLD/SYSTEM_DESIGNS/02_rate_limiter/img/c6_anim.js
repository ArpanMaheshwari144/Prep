// Card 6: bot har 60 sec baad phir aata. Ilaaj: 429 -> Kafka -> Pattern Svc -> WAF ban, edge pe roko.
// Agar: office ka IP (sab ek IP) -> ban ki jagah captcha, aur ban ~24 ghante me khud hat jaata.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 429 bas "60 sec baad aao" kehta. Bot har 60 sec phir aata, din bhar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har 429 Kafka me. 10 min me 500+ 429 -> WAF block list, edge pe hi roko'],
  [6, YEL, 'AGAR', 'AGAR office ka IP (200 log, ek IP)? -> ban nahi, captcha. Ban bhi 24 ghante me hat jaata'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Bot → Gateway', 'login x 100'], [C.acc, 'Gateway → Bot', '429: 60 sec baad aao'], [C.bad, 'Bot', 'har minute phir aaya, din bhar']],
  [[C.acc, 'Gateway → Kafka', 'har 429 yahan'], [C.acc, 'Kafka → Pattern Svc', 'padhta: 10 min me 500+ 429'], [C.good, 'Pattern Svc → WAF', 'block list me: 5.5.5.5'], [C.good, 'Bot → WAF', 'edge pe hi ruka']],
  [[C.msg, 'Office → WAF', '200 log, ek IP: bheed'], [C.acc, 'WAF → Office', 'captcha, ban nahi'], [C.good, 'WAF → Gateway', 'captcha bhara: andar'], [C.good, 'galti se ban?', 'umar 24 ghante. Haath se bhi hata sakte']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 60, 110, 60, 'Bot', '', { st: 'bad' }); B(330, 60, 150, 60, 'Gateway', 'had paar');
    A(130, 80, 328, 80, C.bad, { n: 1 }); L(230, 65, 'login x 100', C.bad);
    A(328, 105, 132, 105, C.acc, { n: 2 }); L(230, 135, '429: 60 sec baad aao', C.acc);
    el('line', { x1: 60, y1: 230, x2: 840, y2: 230, stroke: C.line, 'stroke-width': 2 }, DYN);
    ['10:00', '10:01', '10:02', '10:03', '10:04', '...'].forEach((s, i) => {
      const x = 100 + i * 140; dot([x, 230], i < 5 ? C.bad : C.muted); T(x, 260, s, { c: C.muted, size: 12 });
      if (i < 5) T(x, 210, '429', { c: C.bad, b: 1, size: 12 });
    });
    num(380, 170, 3, C.bad); L(400, 175, 'har minute phir aaya', C.bad, 'start');
    L(450, 325, '429 sirf ruk ke aane ko kehta, bot din bhar lautta rehta', C.bad);
  } else if (ph === 1) {
    B(20, 60, 110, 60, 'Bot', 'IP 5.5.5.5', { st: 'bad' });
    B(220, 60, 130, 60, 'WAF', 'edge pe', { st: 'good' });
    B(440, 60, 140, 60, 'Gateway', '429 bhejta');
    B(680, 60, 160, 60, 'Kafka', 'har 429 yahan', { st: 'acc' });
    B(680, 230, 160, 70, 'Pattern Svc', '10 min: 500+ 429', { st: 'acc' });
    A(350, 90, 438, 90, C.muted, { dash: 1 }); A(580, 90, 678, 90, C.acc, { n: 1 });
    A(760, 122, 760, 228, C.acc, { n: 2 }); L(775, 175, 'padhta', C.acc, 'start');
    AP([[678, 265], [285, 265], [285, 122]], C.good, { n: 3, at: 0.5 }); L(480, 290, 'block list me daalo: 5.5.5.5', C.good);
    A(130, 90, 218, 90, C.bad, { n: 4 }); cross(175, 125); L(175, 160, 'edge pe hi ruka', C.good);
    L(450, 345, 'gateway ruka nahi: 429 Kafka me daal ke aage badha', C.muted);
  } else {
    B(20, 130, 150, 70, 'Office', '200 log, ek IP');
    B(300, 130, 150, 70, 'WAF', 'is IP pe captcha', { st: 'acc' });
    B(640, 130, 150, 70, 'Gateway', '', { st: 'good' });
    A(170, 150, 298, 150, C.msg, { n: 1 }); L(235, 135, 'bheed', C.msg);
    A(298, 185, 172, 185, C.acc, { n: 2 }); L(235, 215, 'captcha, ban nahi', C.acc);
    A(450, 165, 638, 165, C.good, { n: 3 }); L(545, 150, 'captcha bhara: andar', C.good);
    B(300, 260, 300, 60, 'galti se ban laga?', 'ban ki umar 24 ghante, khud hat jaata', { st: 'dim' });
    num(625, 290, 4, C.good); L(645, 295, 'haath se bhi hata sakte', C.good, 'start');
    L(450, 30, 'turant ban karte to office ke 200 asli log bahar', C.bad);
  }
}
