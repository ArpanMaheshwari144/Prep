// Card 4: ek DB machine 46,000 likhai / sec nahi jhelti. Ilaaj: Cassandra, key = chat_id, ek chat ek partition, andar kram.
const DUR = 18, FIX = 8;
const SRC = [150, 190];
const COL = [C.msg, C.acc, '#b388ff'];
const PART = [[478, 75], [478, 190], [478, 305]];
function setup() {
  box(20, 130, 130, 120, 'Chat Server', '46,000 / sec');
  box(490, 140, 180, 100, 'Ek DB machine', '', { id: 'one' });
  wire('M150 190 H490', 'w1');
  ['chat 1', 'chat 2', 'chat 3'].forEach((c, i) => {
    box(490, 45 + i * 115, 200, 64, 'Cassandra', `partition: ${c}`, { id: 'p' + i });
    wire(`M150 190 L490 ${77 + i * 115}`, 'wp' + i);
  });
}
function frame(t, fix, tl) {
  vis('one', !fix); vis('w1', !fix);
  [0, 1, 2].forEach(i => { vis('p' + i, fix); vis('wp' + i, fix); });
  if (!fix) {
    CAP.textContent = 'DIKKAT: ek DB machine. 46,000 likhai / sec aayi, ek machine nahi jhelti';
    let inn = 0, stuck = 0;
    for (let k = 0; k < 40; k++) {
      const s = 0.3 + k * 0.13;
      if (tl < s + 0.6) { dot(path([[s, SRC], [s + 0.6, [478, 190]]], tl), COL[k % 3]); continue; }
      if (inn < 22) inn++; else stuck++;
    }
    for (let j = 0; j < Math.min(stuck, 12); j++) dot([466 - j * 20, 280], C.bad);
    if (stuck) T(350, 310, `atki: ${stuck}`, { c: C.bad, b: 1, size: 13 });
    bar(500, 200, 160, 18, inn / 22, 'bojh');
    if (inn >= 22) { T(780, 180, 'DB atka', { c: C.bad, b: 1, size: 15 }); T(780, 205, 'likhai ruk gayi', { c: C.bad, size: 12 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: Cassandra, key = chat_id. Ek chat ke saare message ek partition, andar kram se';
  const cnt = [0, 0, 0];
  for (let k = 0; k < 36; k++) {
    const s = 0.3 + k * 0.13, i = k % 3;
    if (tl < s + 0.6) { dot(path([[s, SRC], [s + 0.6, PART[i]]], tl), COL[i]); continue; }
    cnt[i]++;
  }
  cnt.forEach((n, i) => {
    for (let j = 0; j < Math.min(n, 12); j++)
      el('rect', { x: 700 + j * 14, y: 64 + i * 115, width: 10, height: 26, rx: 2, fill: COL[i] }, DYN);
    T(785, 112 + i * 115, `${n} message, kram se`, { c: C.muted, size: 12 });
  });
  T(300, 335, 'har machine ko apna hissa', { c: C.good, size: 13 });
  if (tl > 6.6) {
    el('rect', { x: 696, y: 60, width: 172, height: 34, rx: 4, fill: 'none', stroke: C.good, 'stroke-width': 2 }, DYN);
    T(885, 30, 'chat 1 khuli: aakhri 50 ek jagah, ek baar', { a: 'end', c: C.good, b: 1, size: 13 });
  }
}
