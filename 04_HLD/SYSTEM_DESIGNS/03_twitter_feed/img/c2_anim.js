// Card 2: usi request me 5,000 inbox ek-ek karke -> button 5 sec ghooma, "fail" dikha. Ilaaj: DB save + khabar Kafka me,
// turant "ho gaya", worker peeche (4 frame). Agar: worker 60 bhar ke gira -> commit nahi, khabar phir, tweet ek hi baar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: tweet kiya. Usi request me 5,000 inbox ek-ek karke bhare, button 5 sec ghooma'],
  [3, GRN, 'ILAAJ', 'ILAAJ: tweet DB me save, khabar Kafka me, turant "ho gaya". Inbox worker peeche bharta'],
  [6, YEL, 'AGAR', 'AGAR worker 100 me se 60 inbox bhar ke gira? -> commit nahi hua, khabar phir aayegi'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.msg, 'Arpan → Tweet Svc', 'Arpan ne tweet kiya, request Tweet Svc pe aayi'], [C.good, 'Tweet Svc → Tweet DB', 'Tweet Svc ne t9 DB me save kar diya'], [C.bad, 'Tweet Svc → 5,000 inbox', 'Phir usi request me 5,000 follower ke inbox ek-ek karke bharne laga'], [C.bad, 'Tweet Svc → Arpan', '5 sec me timeout, Arpan ko "fail" dikha, dobara daala to tweet do baar']],
  [[C.good, 'Tweet Svc → Tweet DB', 'Tweet Svc ne pehle t9 DB me pakka save kiya'], [C.acc, 'Tweet Svc → Kafka', 't9 ki khabar Kafka me daali, wo disk pe surakshit rehti'], [C.good, 'Tweet Svc → Arpan', 'Arpan ko turant "ho gaya", inbox bharne ka intezaar nahi'], [C.good, 'Kafka → Worker', 'Worker ne peeche apni speed se 5,000 inbox bhare']],
  [[C.acc, 'Kafka → Worker-1', 'Worker-1 ne Kafka se t9 uthaya aur inbox bharne laga'], [C.bad, 'Worker-1 gira', '100 me se 60 inbox bhar ke gira, Kafka ko "ho gaya" nahi bola'], [C.acc, 'Kafka → Worker-2', 'Isliye Kafka ne wahi t9 Worker-2 ko dobara de diya'], [C.good, 'Worker-2 → Mohan ka inbox', 'Mohan ke inbox me dobara likha, sorted set me same id ek hi baar rehti']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 120, 70, 'Arpan', 'ghoom raha', { st: 'bad' });
    B(240, 130, 150, 70, 'Tweet Svc', 'ek hi request me');
    B(600, 40, 200, 60, 'Tweet DB', 't9 save', { st: 'good' });
    B(560, 180, 280, 110, '5,000 inbox', '', { st: 'bad' });
    dots(30, 580, 230, C.bad, { cols: 15, gap: 17, r: 5 });
    T(700, 280, 'ek-ek karke: 1,200 / 5,000', { c: C.bad, size: 11 });
    A(140, 155, 238, 155, C.msg, { n: 1 });
    A(390, 145, 598, 75, C.good, { n: 2 }); L(495, 95, 'save', C.good);
    A(390, 180, 558, 225, C.bad, { n: 3 }); L(470, 225, 'bharta raha', C.bad);
    A(238, 185, 142, 185, C.bad, { n: 4, dash: 1 }); L(190, 225, '5 sec: "fail"', C.bad);
    L(450, 340, 'par tweet to ban chuka tha. User ne dobara daala to do baar', C.bad);
  } else if (ph === 1) {
    panel(0, 'tweet save', ['Tweet Svc', 'Arpan: t9', ''], ['Tweet DB', 't9 save', 'good'], 'save', true, C.good, 'pehle DB me pakka');
    panel(1, 'khabar Kafka me', ['Tweet Svc', '', ''], ['Kafka', 't9 aaya', 'acc'], 'khabar', true, C.acc, 'disk pe, khoti nahi');
    panel(2, 'turant jawab', ['Arpan', '"ho gaya"', 'good'], ['Tweet Svc', '', ''], 'turant', false, C.good, 'apna tweet apni feed me');
    panel(3, 'peeche, worker', ['Kafka', 't9', 'acc'], ['Worker', '5,000 inbox bhare', 'good'], 'padha', true, C.good, 'apni speed se');
    L(450, 320, 'Arpan ko 5,000 inbox ka intezaar nahi. "ho gaya" tab, jab tweet DB me save', C.good);
  } else {
    B(20, 130, 150, 70, 'Kafka', 't9 (commit nahi)', { st: 'acc' });
    B(300, 40, 170, 60, 'Worker-1', '60 / 100 bhare', { st: 'bad' }); cross(495, 70);
    B(300, 230, 170, 60, 'Worker-2', '100 / 100 bhare', { st: 'good' });
    B(640, 120, 220, 90, 'Mohan ka inbox', 'follower: t9 ek hi baar', { st: 'good' });
    A(170, 145, 298, 75, C.acc, { n: 1 }); L(225, 95, 't9', C.acc);
    num(385, 120, 2, C.bad); L(405, 125, 'gira, "ho gaya" nahi bola', C.bad, 'start');
    A(170, 185, 298, 255, C.acc, { n: 3 }); L(225, 245, 't9 phir', C.acc);
    A(470, 255, 638, 185, C.good, { n: 4 }); L(570, 250, 'dobara likha', C.good);
    L(450, 330, 'pehle wale 60 me t9 dobara gaya, par sorted set me same id ek hi baar rehti', C.good);
  }
}
