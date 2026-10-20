// Card 3: Worker-A ne email bheja, "padh liya" bolne se pehle mara, Worker-B ne phir bheja: do email.
// Ilaaj: bhejne se pehle khabar ki key (khabar_id + raasta) Redis me. Agar: Redis gira -> DB UNIQUE rokta.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Worker-A ne email bheja, "padh liya" bolne se pehle mara. Khabar phir aayi. Pal-pal'],
  [3, GRN, 'ILAAJ', 'ILAAJ: bhejne se pehle key (khabar_id + raasta) Redis me dekho. Mili to skip'],
  [6, YEL, 'AGAR', 'AGAR key wala Redis hi gira? -> DB me (khabar_id, raasta) UNIQUE, wahi rokta'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    panel(0, 'Worker-A bhejta', ['Worker-A', 'khabar k9', ''], ['Arpan', 'email 1', 'good'], 'bheja', true, C.good, 'pehla email gaya');
    panel(1, 'Worker-A mara', ['Worker-A', 'mara', 'bad'], ['Kafka', '"padh liya" nahi aaya', 'acc'], '', true, C.bad, 'commit hua hi nahi');
    panel(2, 'khabar phir', ['Kafka', 'k9 phir', 'acc'], ['Worker-B', 'k9 mili', ''], 'diya', true, C.acc, 'Kafka ko laga adhoora');
    panel(3, 'Worker-B bhejta', ['Worker-B', '', ''], ['Arpan', 'email 2', 'bad'], 'phir bheja', true, C.bad, 'do email');
    L(450, 320, 'Worker-B ko pata hi nahi tha ki email pehle ja chuka', C.bad);
  } else if (ph === 1) {
    panel(0, 'pehle key', ['Worker-A', 'khabar k9', ''], ['Redis', 'k9:email = hai', 'acc'], 'rakhi', true, C.acc, 'bhejne se pehle');
    panel(1, 'phir bheja', ['Worker-A', '', ''], ['Arpan', 'email 1', 'good'], 'bheja', true, C.good, 'phir Worker-A mara');
    panel(2, 'khabar phir', ['Worker-B', 'k9 phir mili', ''], ['Redis', 'k9:email = hai', 'acc'], 'hai?', true, C.acc, 'pehle key dekho');
    panel(3, 'skip', ['Worker-B', 'skip', 'good'], ['Redis', 'k9:email = hai', 'acc'], 'haan', false, C.good, 'Arpan ko ek hi email');
    L(450, 320, 'dobara aayi khabar key se pehchaan li, dobara nahi bheja', C.good);
  } else {
    B(20, 140, 150, 60, 'Worker-B', 'k9 phir mili');
    B(320, 30, 200, 70, 'Redis', 'gira', { st: 'bad' }); cross(545, 65);
    B(320, 220, 260, 80, 'DB', 'UNIQUE (k9, email)', { st: 'good' });
    A(170, 155, 318, 70, C.bad, { n: 1, dash: 1 }); L(240, 95, 'key?', C.bad);
    A(170, 185, 318, 250, C.acc, { n: 2 }); L(225, 240, 'INSERT (k9, email)', C.acc);
    A(580, 260, 700, 260, C.good, { n: 3 }); L(640, 245, 'pehle se hai', C.good);
    B(700, 230, 160, 60, 'skip', 'email nahi gaya', { st: 'good' });
    L(450, 345, 'Redis tez raasta hai, DB ki UNIQUE aakhri deewar', C.good);
  }
}
