// Card 7: paisa Wallet DB me, share Portfolio DB me. INFY ka 20k kata, share nahi aaye (do DB, ek txn nahi).
// Ilaaj: har kadam ka haal likho, fail pe likhe haal se retry, aage hi. Agar: Portfolio DB ghanta band, user dobara?
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: paisa Wallet DB me, share Portfolio DB me. 20k kata, Portfolio DB band, share nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har kadam ka haal likho (status log). Fail hua to wahi se retry, aage hi'],
  [6, YEL, 'AGAR', 'AGAR Portfolio DB ghanta bhar band, user dobara order kare? -> nahi'],
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
    B(20, 140, 150, 60, 'Settlement', 'INFY trade t7');
    B(330, 40, 200, 70, 'Wallet DB', 'Arpan -20k', { st: 'acc' });
    B(330, 230, 200, 70, 'Portfolio DB', 'band', { st: 'bad' }); cross(555, 265);
    A(170, 160, 328, 75, C.acc, { n: 1 }); L(240, 95, '-20k', C.acc);
    A(170, 185, 328, 265, C.bad, { n: 2, dash: 1 }); L(240, 245, '+5 INFY', C.bad);
    B(650, 130, 210, 80, 'Arpan', '20k gaya, share nahi', { st: 'bad' }); num(670, 245, 3, C.bad); L(690, 250, 'aadha sauda', C.bad, 'start');
    L(450, 340, 'do alag DB par ek transaction nahi ban sakta', C.bad);
  } else if (ph === 1) {
    panel(0, 'wallet', ['Settlement', 't7', ''], ['Status log', 'wallet: done', 'good'], 'likha', true, C.good, 'pehla kadam pakka');
    panel(1, 'portfolio fail', ['Settlement', '', ''], ['Portfolio DB', 'band', 'bad'], 'share do', true, C.bad, 'status: PENDING');
    panel(2, 'retry', ['Retry job', 'log padha', 'acc'], ['Portfolio DB', 'wapas aaya', 'good'], 't7 se phir', true, C.acc, 'wahi trade_id');
    panel(3, 'SETTLED', ['Status log', 'portfolio: done', 'good'], ['Arpan', '+5 INFY', 'good'], 'pura', true, C.good, 'aage hi, ulta nahi');
    L(450, 320, 'match ke baad sauda pakka. System aage badhata, Arpan ko PENDING dikhta', C.good);
  } else {
    B(20, 140, 150, 70, 'Arpan', 'dobara order?', { st: 'bad' });
    B(330, 40, 230, 70, 'Trade t7', 'PENDING + alert', { st: 'acc' });
    B(330, 230, 230, 70, 'Naya trade t8', '+5 INFY phir', { st: 'bad' });
    B(690, 130, 170, 90, 'DB wapas aaya', '10 INFY, paisa 2 baar', { st: 'bad' });
    A(170, 160, 328, 75, C.acc, { n: 1 }); L(240, 95, 'purana zinda', C.acc);
    A(170, 190, 328, 265, C.bad, { n: 2 }); L(240, 255, 'naya bhi', C.bad);
    A(560, 75, 688, 160, C.acc); A(560, 265, 688, 195, C.bad); num(720, 245, 3, C.bad); L(740, 250, 'DB lauta: dono chale', C.bad, 'start');
    L(450, 345, 'isliye user kuch nahi karta: system wahi t7 se retry, ops ko alert', C.good);
  }
}
