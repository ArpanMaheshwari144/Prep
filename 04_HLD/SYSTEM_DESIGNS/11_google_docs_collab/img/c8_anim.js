// Card 8: 2 saal purana doc, 10 lakh op shuru se chale. Ilaaj: har 1000 op pe photo (snapshot). Agar: purana version maanga.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: doc = saare op ka jod. 2 saal purana doc khola, 10 lakh op shuru se chale'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har 1000 op pe photo (snapshot), job leta. Kholna = aakhri photo + baad ke op'],
  [6, YEL, 'AGAR', 'AGAR purana version v5,00,500 maanga? -> v5,00,000 ki photo + 500 op'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Main → DB', 'Maine 2 saal purana doc khola'],
   [C.bad, 'DB', 'Doc saare op ka jod hai: 10 lakh op shuru se ek-ek chalaye'],
   [C.bad, 'Main', 'Minuton tak khaali screen, doc khula hi nahi']],
  [[C.acc, 'Job → DB', 'Background job har 1000 op pe doc ki photo (snapshot) leta'],
   [C.msg, 'Main → DB', 'Doc khola: aakhri photo v9,99,000 uthayi'],
   [C.good, 'DB → Main', 'Photo + baad ke ~1000 op chalaye, doc turant khula']],
  [[C.msg, 'Main → DB', 'Kal ka purana version v5,00,500 maanga'],
   [C.acc, 'DB', 'Us se pehli photo v5,00,000 uthayi'],
   [C.good, 'DB → Main', 'Photo + beech ke 500 op chalaye, wahi purana version mila']],
];
function frame(ph) {
  B(330, 95, 550, 140, 'DB: doc:123', '', { st: ph === 0 ? 'bad' : '', icon: 'db' });
  if (ph === 0) {
    B(15, 135, 190, 65, 'Main', 'khaali screen', { st: 'bad', icon: 'user' });
    cells(378, 160, [['op 1', C.bad], ['op 2', C.bad], ['op 3', C.bad], ['...'], ['op 10 lakh', C.bad]], { w: 95, h: 36 });
    A(205, 160, 328, 160, C.msg, { n: 1 }); num(600, 265, 2, C.bad); L(600, 295, 'shuru se ek-ek chalao', C.bad);
    num(110, 230, 3, C.bad); L(110, 262, 'minuton tak', C.bad); L(110, 282, 'kuch nahi dikha', C.bad);
  } else if (ph === 1) {
    B(15, 20, 190, 60, 'Job', 'background', { st: 'acc', icon: 'clock' });
    B(15, 250, 190, 65, 'Main', 'doc turant khula', { st: 'good', icon: 'user' });
    cells(378, 160, [['photo v9,98,000'], ['photo v9,99,000', C.acc], ['+ ~1000 op', C.good]], { w: 162, h: 36 });
    A(205, 50, 440, 158, C.acc, { n: 1 }); L(110, 102, 'har 1000 op pe', C.acc);
    A(205, 270, 600, 198, C.msg, { n: 2, at: 0.6 });
    AP([[780, 237], [780, 335], [110, 335], [110, 317]], C.good, { n: 3, at: 0.4 }); L(560, 322, 'photo + baad ke op', C.good);
  } else {
    B(15, 135, 190, 65, 'Main', 'maanga v5,00,500', { icon: 'user' });
    cells(378, 160, [['photo v4,99,000'], ['photo v5,00,000', C.acc], ['+ 500 op', C.good]], { w: 162, h: 36 });
    A(205, 160, 328, 160, C.msg, { n: 1 }); num(605, 140, 2, C.acc);
    AP([[780, 237], [780, 320], [110, 320], [110, 202]], C.good, { n: 3, at: 0.4 }); L(560, 290, 'wahi purana version mila', C.good);
    L(560, 345, 'koi bhi version = us se pehli photo + beech ke op', C.muted);
  }
}
