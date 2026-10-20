// Card 2: fetcher ek-ek source, A atka to B-Z line me, khabar 20 min purani. Ilaaj: fetcher -> Kafka -> worker.
// Agar: likha, commit se pehle crash -> url UNIQUE, ON CONFLICT DO NOTHING.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: meri feed me khabar 20 min purani. Fetcher ek-ek source, ek atka to sab ruke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: fetcher sirf laata, Kafka me daalta. Worker apni speed se DB + Redis me likhta'],
  [6, YEL, 'AGAR', 'AGAR likha, Kafka commit se pehle crash? -> DB khud rokta: url pe UNIQUE'],
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
  [[C.bad, 'Fetcher → Source A', 'Fetcher ek-ek source se khabar laata, Source A pe 30 sec atka'], [C.bad, 'Source B se Z', 'Source B se Z tak line me khade, kisi ki baari nahi aayi'], [C.bad, 'Fetcher → DB', 'Fetcher khud DB me likhta bhi tha, laana aur likhna ek hi line me']],
  [[C.acc, 'Fetcher → Kafka', 'Fetcher ne a91 Kafka me daali (key = source), bas laana, likhna nahi'], [C.acc, 'Kafka → Worker', 'Worker apni speed se Kafka se padhta'], [C.good, 'Worker → DB', 'Worker ne a91 DB me save ki'], [C.good, 'Worker → Redis', 'Phir feed me a91 ka ZADD kiya']],
  [[C.good, 'Worker-1 → DB', 'Worker-1 ne a91 DB me likh di'], [C.bad, 'Worker-1 mara', 'Kafka ko "padh liya" (commit) bolne se pehle Worker-1 mar gaya'], [C.acc, 'Kafka → Worker-2', 'Isliye Kafka ne a91 Worker-2 ko dobara di'], [C.good, 'Worker-2 → DB', 'url UNIQUE tha, ON CONFLICT DO NOTHING se khabar ek hi baar rahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 170, 80, 'Fetcher', 'ek-ek karke', { st: 'bad' });
    B(330, 20, 210, 70, 'Source A', '30 sec atka', { st: 'bad' });
    B(330, 140, 300, 90, 'Line me', '', { st: 'acc' });
    cells(345, 185, [['B', C.acc], ['C', C.acc], ['D', C.acc], ['...', C.line], ['Z', C.acc]], { w: 50, h: 32 });
    B(720, 260, 150, 70, 'DB', 'der se', { st: 'bad', icon: 'db' });
    A(190, 150, 328, 60, C.bad, { n: 1 });
    num(660, 185, 2, C.bad); L(680, 190, 'wait', C.bad, 'start');
    AP([[105, 212], [105, 295], [718, 295]], C.bad, { n: 3, dash: 1, at: 0.4 });
    L(450, 345, 'ek beemar source, aur sab khabar 20 min peeche', C.bad);
  } else if (ph === 1) {
    panel(0, 'laao', ['Fetcher', 'NDTV se a91', ''], ['Kafka', 'key = NDTV', 'acc'], 'daala', true, C.acc, 'bas laana');
    panel(1, 'line', ['Kafka', '10 partition', 'acc'], ['Worker', 'a91 padha', ''], 'padha', true, C.acc, 'apni speed se');
    panel(2, 'likho', ['Worker', '', ''], ['DB', 'a91 save', 'good'], 'INSERT', true, C.good, 'sach DB me');
    panel(3, 'feed', ['Worker', '', ''], ['Redis', 'ZADD a91', 'good'], 'ZADD', true, C.good, 'turant dikhi');
    L(450, 320, 'laana aur likhna alag. Ek source atka to baaki chalte', C.good);
  } else {
    B(20, 130, 160, 80, 'Kafka', 'a91', { st: 'acc' });
    B(330, 20, 220, 80, 'Worker-1', 'commit se pehle mara', { st: 'bad' });
    B(330, 240, 220, 80, 'Worker-2', 'a91 dobara', { st: 'good' });
    B(700, 130, 170, 80, 'DB', 'url UNIQUE', { st: 'good', icon: 'db' });
    A(180, 150, 328, 60, C.muted);
    A(550, 60, 698, 150, C.good, { n: 1 });
    num(440, 125, 2, C.bad); L(460, 130, 'mara', C.bad, 'start');
    A(180, 190, 328, 280, C.acc, { n: 3 });
    A(550, 280, 698, 190, C.good, { n: 4 }); L(640, 285, 'DO NOTHING', C.good, 'start');
    L(450, 345, 'dobara aayi to bhi ek hi baar. Rok DB ke andar hai', C.good);
  }
}
