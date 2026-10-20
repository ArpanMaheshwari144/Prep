// Card 1: Main aur Ravi ne ek second me A1 khaali padhi, dono ne book ki -> 2 ticket. Ilaaj: atomic UPDATE ... WHERE available.
const DUR = 18, FIX = 8;
const M = [130, 90], R = [130, 280], AU = [318, 165], AD = [318, 205], OU = [450, 165], OD = [450, 205],
  DU = [588, 165], DD = [588, 205];
function setup() {
  wire('M130 90 L330 165'); wire('M130 280 L330 205'); wire('M450 185 H600');
  box(20, 60, 110, 60, 'Main', 'A1 chahiye');
  box(20, 250, 110, 60, 'Ravi', 'A1 chahiye');
  box(330, 150, 120, 70, 'App');
  box(600, 140, 200, 90, 'SQL DB', 'seats table', { stroke: C.acc });
}
function seat(st, c, who) {
  cells(625, 250, [['A1', c], [st, c]], { w: 72 });
  if (who) T(700, 300, who, { c, size: 12 });
}
function frame(t, fix, tl) {
  T(890, 30, '10:00:01', { a: 'end', c: C.muted, size: 12 });
  dot(path([[0.3, M], [1.0, AU]], tl < 1.0 ? tl : -1), C.msg, '', 'Book A1', C.msg);
  dot(path([[0.3, R], [1.0, AD]], tl < 1.0 ? tl : -1), C.msg, '', 'Book A1', C.msg);
  if (!fix) {
    CAP.textContent = 'DIKKAT: 10:00:01 pe Main aur Ravi dono ne A1 khaali padhi, dono ne book ki -> 2 ticket';
    if (tl > 1.0 && tl < 2.2) {
      const lb = tl < 1.6 ? 'SELECT' : 'khaali';
      dot(path(tl < 1.6 ? [[1.0, OU], [1.6, DU]] : [[1.6, DU], [2.2, OU]], tl), C.acc, '', lb, C.acc);
      dot(path(tl < 1.6 ? [[1.0, OD], [1.6, DD]] : [[1.6, DD], [2.2, OD]], tl), C.acc, '', lb, C.acc);
    }
    if (tl >= 2.2) T(390, 245, 'dono ko: A1 khaali', { c: C.acc, size: 12 });
    if (tl > 2.6 && tl < 3.2) {
      dot(path([[2.6, OU], [3.2, DU]], tl), C.acc, '', 'SET booked', C.acc);
      dot(path([[2.6, OD], [3.2, DD]], tl), C.acc, '', 'SET booked', C.acc);
    }
    if (tl < 3.2) seat('available', C.good); else seat('booked', C.bad, 'likha: Main, phir Ravi');
    if (tl > 3.6) {
      dot(path([[3.6, AU], [4.4, [165, 75]]], tl), C.good, '', 'ticket', C.good);
      dot(path([[3.6, AD], [4.4, [165, 265]]], tl), C.good, '', 'ticket', C.good);
    }
    if (tl > 4.6) T(890, 335, 'A1 ke ticket: 2', { a: 'end', c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: dekhna + book ek query (UPDATE ... WHERE available). Row lock: Main 1 row, Ravi 0';
  T(450, 340, "UPDATE seats SET status = 'booked' WHERE seat_id = 'A1' AND status = 'available'", { c: C.muted, size: 12 });
  // Main pehle pahuncha
  if (tl > 1.0 && tl < 3.0) {
    const p = tl < 1.6 ? path([[1.0, OU], [1.6, DU]], tl) : tl < 2.2 ? path([[1.6, DU], [2.2, OU]], tl) : path([[2.2, AU], [3.0, [165, 75]]], tl);
    dot(p, tl < 1.6 ? C.acc : C.good, '', tl < 1.6 ? 'UPDATE' : '1 row', tl < 1.6 ? C.acc : C.good);
  }
  if (tl >= 3.0) dot([165, 75], C.good, '', 'A1 meri', C.good);
  // Ravi ruka (row lock), phir 0 row
  if (tl > 1.0 && tl < 4.0) {
    const p = tl < 2.2 ? path([[1.0, OD], [1.5, [520, 205]]], tl) : tl < 2.6 ? path([[2.2, [520, 205]], [2.6, DD]], tl)
      : tl < 3.2 ? path([[2.6, DD], [3.2, OD]], tl) : path([[3.2, AD], [4.0, [165, 265]]], tl);
    const lb = tl < 2.2 ? 'ruk (row lock)' : tl < 2.6 ? 'UPDATE' : '0 row';
    dot(p, tl < 2.6 ? C.acc : C.bad, '', lb, tl < 2.6 ? C.acc : C.bad);
  }
  if (tl >= 4.0) dot([165, 265], C.bad, '', 'seat ja chuki', C.bad);
  if (tl < 1.6) seat('available', C.good); else seat('booked', C.good, 'Main ki');
  if (tl > 4.2) T(890, 305, 'A1 ke ticket: 1', { a: 'end', c: C.good, b: 1, size: 15 });
}
