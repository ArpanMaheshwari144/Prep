// Card 6: naya link turant 404. Ilaaj: quorum 2 likho 2 padho. Agar: likhte waqt copy C giri.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek copy me likha, doosri se padha, wahan abhi pahuncha nahi, 404'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 2 me likho, 2 se padho. Har jodi me ek taaza copy zaroor (quorum)'],
  [6, YEL, 'AGAR', 'AGAR likhte waqt copy C gir gayi? -> 2 me ho gaya to save, C uthi to note se bhar jaati'],
];
function setup() {}
const CY = [50, 165, 280]; // A, B, C ke y (box 50 unche)
function copies(a, b, c) {
  [a, b, c].forEach(([st, sub], i) => B(520, CY[i], 130, 50, 'Copy ' + 'ABC'[i], sub, { st }));
}
function app() { B(20, 130, 120, 90, 'App'); }
function W(i, c, n, o = {}) { A(140, 150, 518, CY[i] + 18, c, { n, at: 0.3, ...o }); }   // likhna (upar se)
function Rd(i, c, n, o = {}) { A(140, 200, 518, CY[i] + 36, c, { n, at: 0.3, ...o }); }  // padhna (neeche se)
const STEPS = [
  [[C.good, 'App → Copy A', 'Naya link sirf Copy A me likha, B aur C tak abhi pahuncha nahi'], [C.bad, 'App → Copy C', 'User ne turant link khola, padhna Copy C se hua'], [C.bad, '404', 'C me naya link tha hi nahi, isliye user ko 404 dikha']],
  [[C.good, 'App → Copy A, B', 'Likhte waqt 3 me se 2 copy (A, B) me likha, tab hi "ho gaya"'], [C.acc, 'App → Copy B, C', 'Padhte waqt bhi 2 copy (B, C) se padha'], [C.good, 'B me mila', 'Likhne ke 2 aur padhne ke 2 me kam se kam ek copy common, to naya link mila']],
  [[C.good, 'App → Copy A, B', 'Naya link A aur B me likh diya gaya'], [C.bad, 'App → Copy C', 'C us waqt giri hui thi, use likhai nahi pahunchi'], [C.good, 'save', '3 me se 2 me likh gaya, itna kaafi hai, user ko "ho gaya"'], [C.acc, 'B → note → C', 'B ne C ke liye note rakha; C wapas aayi to note se naya link bhar diya']],
];
function frame(ph) {
  if (ph === 0) {
    app(); copies(['good', 'naya link'], ['', 'purani'], ['bad', 'purani']);
    W(0, C.good, 1); L(300, 75, 'likha: sirf A me', C.good);
    Rd(2, C.bad, 2); L(300, 275, 'turant padha: C se', C.bad);
    num(80, 250, 3, C.bad); T(80, 285, '404', { c: C.bad, b: 1, size: 20 });
    L(330, 320, 'C tak naya link abhi pahuncha nahi', C.bad);
  } else if (ph === 1) {
    app(); copies(['good', 'naya link'], ['good', 'naya link'], ['', 'purani']);
    W(0, C.good, 1); W(1, C.good); L(300, 75, 'likho: A, B', C.good);
    Rd(1, C.acc, 2); Rd(2, C.acc); L(300, 275, 'padho: B, C', C.acc);
    num(80, 250, 3, C.good); T(80, 280, 'B me mila', { c: C.good, b: 1, size: 14 });
    L(700, 180, '2 + 2 = 4, copy 3:', C.muted, 'start'); L(700, 200, 'jodi me ek taaza zaroor', C.good, 'start');
  } else {
    app(); copies(['good', 'naya link'], ['good', 'naya link'], ['bad', '']); cross(675, 305);
    W(0, C.good, 1); W(1, C.good); W(2, C.bad, 2, { dash: 1 }); L(300, 60, 'App ne teeno copy ko likhna bheja', C.muted); L(300, 82, 'A, B me pahuncha', C.good);
    L(330, 330, 'C giri, likhai nahi pahunchi', C.bad);
    num(80, 250, 3, C.good); T(80, 280, 'save', { c: C.good, b: 1, size: 16 }); T(80, 300, '3 me se 2 me likha', { c: C.good, size: 13 }); T(80, 318, '= kaafi, user ko OK', { c: C.good, size: 13 });
    B(720, 150, 150, 50, 'B ke paas note', 'C ke liye', { st: 'acc' }); A(650, 190, 718, 180, C.acc, { n: 4 });
    A(790, 200, 660, 285, C.acc, { dash: 1 }); L(585, 228, 'C wapas uthi: B ne note se', C.acc); L(585, 250, 'naya link C me bhar diya', C.acc);
  }
}
