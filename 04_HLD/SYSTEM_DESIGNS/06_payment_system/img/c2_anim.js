// Card 2: wallet 600, phone (dukaan 500) + laptop (Swiggy 500) ek saath, dono ne 600 padha, -400.
// Ilaaj: check + kaato ek query (atomic UPDATE). Agar: WHERE bhool gaye -> DB ka CHECK pakadta.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: wallet me 600. Phone se 500, laptop se 500, ek saath. Pal-pal'],
  [3, GRN, 'ILAAJ', 'ILAAJ: "paisa hai?" aur "kaato" ek hi query me. DB ek-ek karke chalata, jeetta ek'],
  [6, YEL, 'AGAR', 'AGAR WHERE wala check bhool gaye? -> DB ka CHECK (balance >= 0) pakadta'],
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
  [[C.acc, 'Wallet DB → Server-1', 'Phone wali request ne balance padha: 600'], [C.acc, 'Wallet DB → Server-2', 'Usi pal laptop wali ne bhi 600 padha'], [C.bad, 'Server-1 → Wallet DB', 'Server-1 ne 500 kaat ke 100 likha'], [C.bad, 'Server-2 → Wallet DB', 'Server-2 ne bhi 500 kaate, balance -400 ho gaya']],
  [[C.acc, 'Server-1 → Wallet DB', 'Server-1 ne ek query bheji: UPDATE ... WHERE balance >= 500'], [C.good, 'Wallet DB → Server-1', '1 row badli, 600 se 100, paisa kata'], [C.acc, 'Server-2 → Wallet DB', 'Server-2 ki wahi query ek pal baad chali'], [C.bad, 'Wallet DB → Server-2', 'Ab sirf 100 tha, 0 row badli, jawab "paisa kam"']],
  [[C.bad, 'Server → Wallet DB', 'Kisi naye code me WHERE bhool gaye, 100 me se 500 kaatne chala'], [C.acc, 'Wallet DB → Server', 'DB ke CHECK (balance >= 0) ne error diya, balance 100 hi raha']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'phone padhta', ['Server-1', 'phone: 600 dikha', 'acc'], ['Wallet DB', 'Arpan: 600', ''], 'padha', false, C.acc, '600 >= 500, theek');
    panel(1, 'laptop padhta', ['Server-2', 'laptop: 600 dikha', 'acc'], ['Wallet DB', 'Arpan: 600', ''], 'padha', false, C.acc, 'abhi bhi 600');
    panel(2, 'Server-1 kaata', ['Server-1', 'dukaan ko 500', 'bad'], ['Wallet DB', 'Arpan: 100', 'bad'], 'likha -500', true, C.bad, 'pehla kata');
    panel(3, 'Server-2 kaata', ['Server-2', 'Swiggy ko 500', 'bad'], ['Wallet DB', 'Arpan: -400', 'bad'], 'likha -500', true, C.bad, 'doosra bhi kata');
    L(450, 320, '600 the, 1000 kat gaye. Dono ne purana 600 padha tha', C.bad);
  } else if (ph === 1) {
    B(20, 110, 180, 70, 'Server-1', 'phone: 500', { st: 'good' });
    B(700, 110, 180, 70, 'Server-2', 'laptop: 500', { st: 'bad' });
    B(330, 100, 240, 90, 'Wallet DB', '600 -> 100', { st: 'acc' });
    A(200, 130, 328, 130, C.acc, { n: 1 }); A(328, 165, 202, 165, C.good, { n: 2 }); L(265, 205, '1 row: kata', C.good);
    A(698, 130, 572, 130, C.acc, { n: 3 }); A(572, 165, 698, 165, C.bad, { n: 4 }); L(635, 205, '0 row: paisa kam', C.bad);
    L(450, 70, 'ek ke baad ek', C.acc);
    T(450, 250, 'UPDATE wallet SET balance = balance - 500', { c: C.fg, size: 13 });
    T(450, 272, 'WHERE id = Arpan AND balance >= 500', { c: C.fg, size: 13 });
    L(450, 330, 'laptop ki baari pe 100 bacha tha, 100 < 500, kuch nahi kata', C.good);
  } else {
    B(20, 120, 200, 80, 'Server', 'WHERE bhool gaya', { st: 'bad' });
    B(380, 110, 260, 100, 'Wallet DB: Arpan 100', 'CHECK (balance >= 0)', { st: 'acc' });
    A(220, 145, 378, 145, C.bad, { n: 1 }); L(300, 130, '100 - 500', C.bad);
    A(378, 180, 222, 180, C.acc, { n: 2, at: 0.2 }); L(300, 200, 'error: -400 nahi', C.acc);
    L(510, 240, 'DB ne likhne hi nahi diya, 100 wahi', C.acc);
    T(450, 290, 'UPDATE wallet SET balance = balance - 500 WHERE id = Arpan', { c: C.fg, size: 13 });
    L(450, 335, 'query bhool jaaye, to bhi DB ka CHECK aakhri deewar', C.good);
  }
}
