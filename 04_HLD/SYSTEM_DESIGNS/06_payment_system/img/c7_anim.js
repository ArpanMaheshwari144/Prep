// Card 7: sirf balance column, 2200 -> 1100, "kaise?" ka jawab nahi. Ilaaj: ledger double-entry, line-line dikhta.
const DUR = 17, FIX = 7.5;
const AR = [90, 110], SUP = [90, 228], SUR = [160, 260], WB = [345, 142], LG = [488, 275];
function setup() {
  wire('M90 110 V240'); wire('M160 260 L345 130'); wire('M160 275 H500', 'wl');
  box(20, 40, 140, 70, 'Arpan', '"1100 kaise?"');
  box(20, 240, 140, 70, 'Support');
  box(250, 40, 190, 90, 'wallet', 'balance');
  box(500, 40, 380, 250, 'Ledger (sirf INSERT)', '', { id: 'lg' });
}
function bal(v, when) {
  T(345, 102, String(v), { c: C.fg, b: 1, size: 16 });
  T(345, 122, when, { c: C.muted, size: 11 });
}
function frame(t, fix, tl) {
  vis('lg', fix); vis('wl', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: sirf balance column. 2200 se 1100, par kaise? number hai, kahani nahi';
    if (tl < 1.8) bal(2200, 'kal raat'); else bal(1100, 'subah');
    if (tl > 2.4 && tl < 3.0) dot(path([[2.4, AR], [3.0, SUP]], tl), C.msg, '', 'kaise?');
    if (tl > 3.2 && tl < 3.8) dot(path([[3.2, SUR], [3.8, WB]], tl), C.acc, '', 'kyun?', C.acc);
    if (tl > 3.8 && tl < 4.4) dot(path([[3.8, WB], [4.4, SUR]], tl), C.acc, '', '1100', C.acc);
    if (tl >= 4.6) T(20, 332, 'sirf number, record nahi, jawab nahi', { a: 'start', c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: ledger double-entry. Har paisa do line (DEBIT + CREDIT), jod 0, sirf INSERT';
  bal(tl < 2.2 ? 2200 : 1100, tl < 2.2 ? 'kal raat' : 'subah');
  const rows = [];
  if (tl >= 0.8) rows.push('Arpan  -800   bijli     +800   jod 0');
  if (tl >= 1.6) rows.push('Arpan  -300   recharge  +300   jod 0');
  rows.forEach((s, i) => T(520, 100 + i * 30, s, { a: 'start', c: C.fg, size: 13 }));
  if (tl > 2.8 && tl < 3.4) dot(path([[2.8, AR], [3.4, SUP]], tl), C.msg, '', 'kaise?');
  if (tl > 3.6 && tl < 4.3) dot(path([[3.6, [160, 275]], [4.3, LG]], tl), C.acc, '', 'line dikhao', C.acc);
  if (tl >= 4.5) T(690, 200, '2200 - 800 - 300 = 1100', { c: C.good, b: 1, size: 16 });
  if (tl >= 5.0) T(20, 332, 'line-line dikhta, jawab mil gaya', { a: 'start', c: C.good, b: 1, size: 13 });
}
