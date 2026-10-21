// Card 1: Ravi ne baad me save kiya, mera WORLD mit gaya. Ilaaj: sirf op + baseVersion bhejo. Agar: maine mitaya, Ravi ne wahin likha.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Save pe poora text upar likha. Ravi ne baad me save kiya, mera "WORLD" mit gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sirf badlaav (op) bhejo + kis version pe bana (v7). Server khiska ke lagata'],
  [6, YEL, 'AGAR', 'AGAR maine mitaya, Ravi ne wahin likha? -> mitana bhi op, Ravi ka akshar khisak ke sahi jagah'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Main → DB', 'Maine Save dabaya, poora text "HELLO WORLD" DB me likh diya'],
   [C.bad, 'Ravi → DB', 'Ravi ne baad me Save dabaya, uska poora "HELLO THERE" upar likha'],
   [C.bad, 'DB', 'Ravi ka text mere wale ke upar baith gaya, mera "WORLD" mit gaya']],
  [[C.msg, 'Main → Server', 'Maine sirf badlaav bheja: " WORLD" pos 5 pe, v7 pe bana'],
   [C.msg, 'Ravi → Server', 'Ravi ne bhi sirf badlaav bheja: " THERE" pos 5 pe, wo bhi v7 pe'],
   [C.acc, 'Server', 'Mera op v8 bana; Ravi ka v7 pe tha, to khiska ke pos 11 pe laga (v9)'],
   [C.good, 'Server → doc', 'Doc ab "HELLO WORLD THERE", dono ka likha bacha, dono ko yahi bheja']],
  [[C.msg, 'Main → Server', 'Maine "HELLO " mitaya: pos 0 se 6 akshar, v9 pe bana'],
   [C.msg, 'Ravi → Server', 'Ravi ne end pe "!" likha, pos 11 pe, wo bhi v9 pe bana'],
   [C.good, 'Server', 'Mitana bhi op: 6 akshar gaye, to Ravi ka "!" pos 11 se 5 pe khiska']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 30, 200, 60, 'Main', 'HELLO WORLD', { icon: 'user' });
    B(15, 240, 200, 60, 'Ravi', 'HELLO THERE', { icon: 'user' });
    B(560, 120, 290, 75, 'DB: doc', 'HELLO THERE', { st: 'bad', icon: 'db' });
    A(215, 60, 558, 140, C.msg, { n: 1 }); L(390, 70, 'Save: poora text', C.msg);
    A(215, 270, 558, 180, C.bad, { n: 2 }); L(390, 270, 'baad me Save: poora text', C.bad);
    num(705, 225, 3, C.bad); L(705, 255, '"WORLD" mit gaya', C.bad);
    L(705, 290, 'jo aakhir me save kare, wahi bachta', C.muted);
  } else if (ph === 1) {
    B(15, 30, 200, 60, 'Main', 'op: " WORLD" @5, v7', { icon: 'user' });
    B(15, 240, 200, 60, 'Ravi', 'op: " THERE" @5, v7', { icon: 'user' });
    B(500, 110, 260, 95, 'Server', '', { st: 'good', icon: 'server' });
    T(630, 165, 'v8: mera @5', { c: C.good, b: 1, size: 14 }); T(630, 188, 'v9: Ravi ka @5 -> @11', { c: C.acc, b: 1, size: 14 });
    A(215, 55, 498, 125, C.msg, { n: 1, at: 0.35 }); A(215, 275, 498, 195, C.msg, { n: 2, at: 0.35 });
    num(785, 160, 3, C.acc);
    cells(460, 270, [['HELLO'], [' WORLD', C.good], [' THERE', C.acc]], { w: 110, h: 36 });
    A(630, 207, 630, 266, C.good, { n: 4 }); L(625, 335, 'dono ke paas: HELLO WORLD THERE', C.good);
  } else {
    B(15, 30, 210, 60, 'Main', 'mitaya "HELLO ", v9', { icon: 'user' });
    B(15, 240, 210, 60, 'Ravi', '"!" @11, v9', { icon: 'user' });
    T(560, 40, 'v9 + dono ke op:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(560, 55, [['HELLO ', C.bad], ['WORLD'], ['+! @11', C.acc]], { w: 100, h: 34 });
    A(225, 60, 498, 150, C.msg, { n: 1, at: 0.4 }); A(225, 270, 498, 190, C.msg, { n: 2, at: 0.4 });
    B(500, 130, 220, 75, 'Server', '@11 - 6 = @5', { st: 'good', icon: 'server' }); num(745, 168, 3, C.good);
    T(560, 245, 'ab (v11):', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(560, 260, [['WORLD', C.good], ['!', C.good]], { w: 100, h: 34 });
    L(640, 330, '"!" sahi jagah, kuch nahi kata', C.good);
  }
}
