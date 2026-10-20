// Card 2: wallet 600, phone + laptop dono ne 600 padha, dono ne 500 kaata -> -400. Ilaaj: check + kaato ek query.
const DUR = 17, FIX = 7.5;
const PH = [140, 70], LP = [140, 270], S1 = [258, 70], S2 = [258, 270], S1R = [400, 70], S2R = [400, 270];
const DB1 = [608, 160], DB2 = [608, 195];
function setup() {
  wire('M140 70 H270'); wire('M140 270 H270'); wire('M400 70 L620 160'); wire('M400 270 L620 195');
  box(20, 40, 120, 60, 'Phone', 'dukaan 500'); box(20, 240, 120, 60, 'Laptop', 'Swiggy 500');
  box(270, 40, 130, 60, 'Server-1'); box(270, 240, 130, 60, 'Server-2');
  box(620, 130, 210, 90, 'DB: Arpan wallet', '', { stroke: C.acc });
}
function frame(t, fix, tl) {
  let bal = 600;
  if (tl < 1.0) { dot(path([[0.3, PH], [1.0, S1]], tl), C.msg, '', '-500'); dot(path([[0.3, LP], [1.0, S2]], tl), C.msg, '', '-500'); }
  if (!fix) {
    CAP.textContent = 'DIKKAT: pehle padho, phir kaato. Dono server ne 600 dekha, dono ne 500 kaata';
    if (tl > 1.2 && tl < 2.4) {
      const go = tl < 1.8, lb = go ? 'padho' : '600';
      dot(path(go ? [[1.2, S1R], [1.8, DB1]] : [[1.8, DB1], [2.4, S1R]], tl), C.acc, '', lb, C.acc);
      dot(path(go ? [[1.2, S2R], [1.8, DB2]] : [[1.8, DB2], [2.4, S2R]], tl), C.acc, '', lb, C.acc);
    }
    if (tl >= 2.4) { T(335, 122, '600 >= 500, haan', { c: C.good, size: 12 }); T(335, 322, '600 >= 500, haan', { c: C.good, size: 12 }); }
    if (tl > 2.8 && tl < 3.4) {
      dot(path([[2.8, S1R], [3.4, DB1]], tl), C.bad, '', 'kaato 500', C.bad);
      dot(path([[2.8, S2R], [3.4, DB2]], tl), C.bad, '', 'kaato 500', C.bad);
    }
    if (tl >= 3.4) bal = 100;
    if (tl >= 3.7) bal = -400;
    if (tl >= 4.0) T(725, 250, 'dono kate, wallet -400', { c: C.bad, b: 1, size: 14 });
  } else {
    CAP.textContent = 'ILAAJ: check + kaato EK query (atomic UPDATE). DB ek-ek chalata, jeetta ek';
    T(450, 345, 'SET balance = balance - 500 WHERE id = ? AND balance >= 500', { c: C.acc, size: 13, b: 1 });
    if (tl > 1.2 && tl < 2.4) {
      const go = tl < 1.8;
      dot(path(go ? [[1.2, S1R], [1.8, DB1]] : [[1.8, DB1], [2.4, S1R]], tl), go ? C.acc : C.good, '', go ? 'UPDATE' : '1 row', go ? C.acc : C.good);
    }
    if (tl > 1.2 && tl < 2.8) {
      const w = [[1.2, S2R], [1.7, [560, 225]], [2.0, [560, 225]], [2.2, DB2]];
      const p = tl < 2.2 ? path(w, tl) : path([[2.2, DB2], [2.8, S2R]], tl);
      dot(p, tl < 2.2 ? C.acc : C.bad, '', tl < 2.0 ? 'UPDATE (ruk)' : tl < 2.2 ? 'UPDATE' : '0 row', tl < 2.2 ? C.acc : C.bad);
    }
    if (tl >= 1.8) bal = 100;
    if (tl >= 2.2) T(725, 250, '100 < 500 -> 0 row', { c: C.muted, size: 12 });
    if (tl >= 2.4) T(335, 122, '1 row -> hua, 100 bacha', { c: C.good, b: 1, size: 12 });
    if (tl >= 2.8) T(335, 322, '0 row -> "paisa kam"', { c: C.bad, b: 1, size: 12 });
  }
  T(725, 195, `balance: ${bal}`, { c: bal < 0 ? C.bad : C.fg, b: 1, size: 16 });
}
