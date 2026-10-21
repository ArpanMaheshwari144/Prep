// Card 1: queue machine restart, 500 signup gayab. Ilaaj: disk pe log + consumer ka offset. Agar: log dobara padhna (replay).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: queue RAM me thi. Subah machine restart hui, 500 signup khabar gayab'],
  [3, GRN, 'ILAAJ', 'ILAAJ: disk pe file (log), padhne se mitti nahi. Email apna number yaad rakhta (offset)'],
  [6, YEL, 'AGAR', 'AGAR log dobara padhna hai? -> offset peeche kar do (replay), khabar wahin padi hai'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Signup → Queue', '500 signup ki khabar queue ki RAM me padi thi'],
   [C.bad, 'Queue', 'Subah machine restart hui, RAM saaf, 500 khabar gayab'],
   [C.bad, 'Queue → Email', 'Email service ko welcome mail bhejne ko kuch nahi mila']],
  [[C.msg, 'Signup → Log', 'Har khabar disk pe file ke aakhir me judti (log), number ke saath'],
   [C.acc, 'Email', 'Email ne 120 tak padha, apna number yaad rakha (offset 120)'],
   [C.good, 'Log → Email', 'Restart ke baad bhi file wahin, Email 121 se aage padhta']],
  [[C.acc, 'Email', 'Bug theek hua, kal ki khabar dobara chahiye'],
   [C.acc, 'Email → Log', 'Email ne apna offset 120 se peeche 100 kiya'],
   [C.good, 'Log → Email', 'Khabar wahin padi thi, 100 se phir padhi (replay)']],
];
function frame(ph) {
  B(15, 140, 170, 60, 'Signup', 'producer', { icon: 'server' });
  if (ph === 0) {
    B(280, 110, 340, 120, 'Queue (RAM)', 'restart: sab saaf', { st: 'bad', icon: 'queue' });
    cells(335, 165, [['S1', C.bad], ['S2', C.bad], ['...'], ['S500', C.bad]], { w: 66, h: 34 }); cross(470, 182);
    A(185, 170, 278, 170, C.msg, { n: 1 }); num(650, 120, 2, C.bad);
    B(700, 140, 190, 60, 'Email', 'kuch nahi mila', { st: 'bad', icon: 'mail' });
    A(622, 170, 698, 170, C.bad, { n: 3, dash: 1 });
    L(450, 300, '500 signup gayab: kisi ko welcome mail nahi', C.bad);
  } else if (ph === 1) {
    B(260, 100, 400, 130, 'Log (disk pe file)', '', { st: 'good', icon: 'db' });
    cells(302, 165, [['118'], ['119'], ['120'], ['121', C.acc], ['122', C.acc]], { w: 68, h: 34 });
    A(185, 170, 258, 170, C.msg, { n: 1 }); L(460, 255, 'naya hamesha aakhir me judta', C.muted);
    B(700, 140, 190, 60, 'Email', 'offset = 120', { st: 'acc', icon: 'mail' }); num(800, 115, 2, C.acc);
    A(662, 180, 698, 180, C.good, { n: 3 }); L(790, 240, 'restart: 121 se aage', C.good);
    L(450, 320, 'padhne se khabar mitti nahi, file me rehti', C.good);
  } else {
    B(260, 100, 400, 130, 'Log (disk pe file)', '', { st: 'good', icon: 'db' });
    cells(302, 165, [['100', C.good], ['...'], ['119'], ['120'], ['121']], { w: 68, h: 34 });
    B(700, 140, 190, 60, 'Email', 'offset 120 -> 100', { st: 'acc', icon: 'mail' }); num(800, 115, 1, C.acc);
    A(698, 160, 662, 160, C.acc, { n: 2 });
    AP([[336, 202], [336, 300], [800, 300], [800, 202]], C.good, { n: 3, at: 0.5 }); L(560, 325, '100 se dobara padha (replay)', C.good);
  }
}
