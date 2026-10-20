// Card 3: dono ne pos 0 pe ek saath. Jo aaya wahi laga -> YX vs XY. Ilaaj: OT, server kram, Y 0 -> 1.
const DUR = 17, FIX = 8;
const ME = [170, 95], RV = [170, 275], SVa = [508, 165], SVb = [508, 195];
function setup() {
  wire('M170 95 L520 165'); wire('M170 275 L520 195');
  box(30, 60, 140, 70, 'Main', 'pos 0 pe X');
  box(30, 240, 140, 70, 'Ravi', 'pos 0 pe Y');
  box(520, 140, 200, 84, 'Conn-Server');
}
function frame(t, fix, tl) {
  T(620, 205, fix ? 'OT: kram server ka' : 'jo aaya, aage bhej', { c: fix ? C.good : C.muted, size: 12 });
  let me = 'HELLO', rv = 'HELLO';
  if (tl >= 0.3) { me = 'XHELLO'; rv = 'YHELLO'; }
  dot(path([[0.6, ME], [1.4, SVa]], tl < 1.4 ? tl : -1), C.msg, 'X', 'X pos 0');
  dot(path([[0.6, RV], [1.4, SVb]], tl < 1.4 ? tl : -1), C.acc, 'Y', 'Y pos 0', C.acc);
  if (!fix) {
    CAP.textContent = 'DIKKAT: jo aaya wahi laga diya, kram kisi ka nahi. Meri screen YX.., Ravi ki XY..';
    dot(path([[1.8, SVa], [2.6, [182, 108]]], tl > 1.8 && tl < 2.6 ? tl : -1), C.acc, 'Y', 'Y pos 0', C.acc);
    dot(path([[1.8, SVb], [2.6, [182, 262]]], tl > 1.8 && tl < 2.6 ? tl : -1), C.msg, 'X', 'X pos 0');
    if (tl >= 2.6) { me = 'YXHELLO'; rv = 'XYHELLO'; }
    const bad = tl >= 2.6;
    T(100, 155, `Main: ${me}`, { c: bad ? C.bad : C.fg, b: 1, size: 14 });
    T(100, 335, `Ravi: ${rv}`, { c: bad ? C.bad : C.fg, b: 1, size: 14 });
    if (tl >= 3.0) T(760, 300, 'do alag doc!', { c: C.bad, b: 1, size: 16 });
    return;
  }
  CAP.textContent = 'ILAAJ: kram server tay karta (OT): X pehle v8, Y v9 aur 0 se 1 pe khiska. Dono ke paas XYHELLO';
  if (tl >= 1.4) T(620, 250, 'X pehle aaya -> v8', { c: C.msg, size: 12, b: 1 });
  if (tl >= 1.8) T(620, 270, 'Y -> v9, pos 0 -> 1', { c: C.acc, size: 12, b: 1 });
  dot(path([[2.2, SVa], [3.0, [182, 108]]], tl > 2.2 && tl < 3.0 ? tl : -1), C.acc, 'Y', 'Y pos 1 (v9)', C.acc);
  dot(path([[2.2, SVb], [3.0, [182, 262]]], tl > 2.2 && tl < 3.0 ? tl : -1), C.msg, 'X', 'X pos 0 (v8)');
  if (tl >= 3.0) { me = 'XYHELLO'; rv = 'XYHELLO'; }
  const ok = tl >= 3.0;
  T(100, 155, `Main: ${me}`, { c: ok ? C.good : C.fg, b: 1, size: 14 });
  T(100, 335, `Ravi: ${rv}`, { c: ok ? C.good : C.fg, b: 1, size: 14 });
  if (tl >= 3.4) T(780, 320, 'dono same', { c: C.good, b: 1, size: 16 });
}
