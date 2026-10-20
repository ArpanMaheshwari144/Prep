// Card 1: poora text overwrite -> baad wala jeeta, WORLD gayab. Ilaaj: op + baseVersion, server khiska ke lagata.
const DUR = 17, FIX = 7.5;
const ME = [160, 92], RV = [160, 272], SV = [548, 182];
function setup() {
  wire('M160 92 L560 170'); wire('M160 272 L560 195');
  box(30, 60, 130, 64, 'Main (App)', '10:05');
  box(30, 240, 130, 64, 'Ravi (App)', '10:07');
  box(560, 140, 220, 84, 'Server + DB');
}
function frame(t, fix, tl) {
  if (!fix) {
    CAP.textContent = 'DIKKAT: Save = poora text upar likh do. 10:07 Ravi ne save kiya, mera WORLD gayab';
    dot(path([[0.3, ME], [1.3, SV]], tl < 1.3 ? tl : -1), C.msg, '', '"HELLO WORLD"');
    dot(path([[2.5, RV], [3.5, SV]], tl < 3.5 ? tl : -1), C.acc, '', '"HELLO THERE"', C.acc);
    let doc = 'HELLO', col = C.fg;
    if (tl >= 1.3) { doc = 'HELLO WORLD'; col = C.good; }
    if (tl >= 3.5) { doc = 'HELLO THERE'; col = C.bad; }
    T(670, 200, `DB: ${doc}`, { c: col, b: 1, size: 14 });
    if (tl >= 3.8) { T(670, 260, 'baad wala jeeta', { c: C.bad, b: 1, size: 14 });
                     T(670, 282, 'mera WORLD gayab!', { c: C.bad, b: 1, size: 14 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: sirf badlaav (op) bhejo, kis version pe bana (baseVersion v7). Server khiska ke lagata';
  dot(path([[0.3, ME], [1.3, SV]], tl < 1.3 ? tl : -1), C.msg, '', "pos 5 ' WORLD' @v7");
  dot(path([[0.6, RV], [1.6, SV]], tl < 1.6 ? tl : -1), C.acc, '', "pos 5 ' THERE' @v7", C.acc);
  let doc = 'HELLO', v = 7;
  if (tl >= 1.3) { doc = 'HELLO WORLD'; v = 8; }
  if (tl >= 2.0) { doc = 'HELLO WORLD THERE'; v = 9; }
  T(670, 200, `doc v${v}: ${doc}`, { c: v === 9 ? C.good : C.fg, b: 1, size: 13 });
  if (tl >= 1.3) T(670, 250, "v8: ' WORLD' pos 5", { c: C.msg, size: 12 });
  if (tl >= 2.0) T(670, 270, "v9: ' THERE' khiska 5 -> 11", { c: C.acc, size: 12 });
  if (tl >= 2.6 && tl < 3.6) dot(path([[2.6, SV], [3.6, [172, 285]]], tl), C.good, '', 'v8 + v9 Ravi ko', C.good);
  if (tl >= 3.6) T(95, 330, 'Ravi: HELLO WORLD THERE', { c: C.good, b: 1, size: 13 });
  if (tl >= 2.0) T(95, 150, 'kisi ka likha nahi mita', { c: C.good, size: 12 });
}
