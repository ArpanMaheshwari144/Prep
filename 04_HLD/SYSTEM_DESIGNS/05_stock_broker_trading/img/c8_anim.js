// Card 8: 10 lakh log har second price poochh rahe, server baitha. Ilaaj: WebSocket push. Agar: 1000 baar badla, 10 lakh ek server. Wapas: connection juda.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 10 lakh log har second price poochh rahe, server baith gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: connection khula, price badla to server khud bheje (WebSocket push)'],
  [6, YEL, 'AGAR', 'AGAR second me 1000 baar price badla? -> har ~100 ms aakhri price bhejo'],
  [9, YEL, 'AGAR', 'AGAR 10 lakh connection ek server pe? -> ek server ~lakh. Aur server + LB'],
  [12, GRN, 'WAPAS', 'WAPAS: connection toota, wapas juda -> abhi ka price alag se do'],
];
function setup() {}
function crowd(c, lab) { dots(24, 30, 80, c, { cols: 4, gap: 22 }); T(65, 225, lab || '10 lakh users', { c: C.muted, size: 12 }); }
function chain(ws2) {
  B(20, 140, 130, 70, 'Matching', '');
  B(210, 140, 130, 70, 'Pub/Sub', '');
  B(420, 50, 150, 70, 'WS server 1', '', { st: 'good' });
  B(420, 230, 150, 70, 'WS server 2', '', { st: ws2 || 'good' });
}
function frame(ph) {
  if (ph === 0) {
    crowd(C.msg);
    A(110, 100, 478, 135, C.msg); A(110, 140, 478, 155, C.msg, { n: 1, at: 0.3 }); A(110, 180, 478, 175, C.msg);
    L(300, 100, 'har second: "price?"', C.msg);
    B(480, 100, 180, 110, 'Server', 'baitha', { st: 'bad' });
    num(690, 155, 2, C.bad); T(780, 162, '10 lakh / sec', { c: C.bad, b: 1, size: 18 });
  } else if (ph === 1) {
    chain();
    A(150, 175, 208, 175, C.acc, { n: 1 }); L(85, 235, 'price badla', C.acc);
    A(340, 160, 418, 95, C.good, { n: 2, at: 0.5 }); A(340, 190, 418, 255, C.good, { n: 2, at: 0.5 });
    A(570, 85, 668, 85, C.good, { n: 3, at: 0.4 }); A(570, 265, 668, 265, C.good, { n: 3, at: 0.4 });
    L(620, 73, 'push', C.good); L(620, 253, 'push', C.good);
    dots(8, 685, 75, C.good, { cols: 4, gap: 22 }); dots(8, 685, 255, C.good, { cols: 4, gap: 22 });
    T(750, 180, 'users kuch nahi poochte', { c: C.muted, size: 12 });
  } else if (ph === 2) {
    chain('dim');
    dots(6, 40, 115, C.acc, { gap: 18 }); L(85, 235, 'second me 1000 baar', C.acc);
    num(160, 130, 1, C.acc);
    A(150, 175, 208, 175, C.acc);
    A(340, 160, 418, 95, C.acc, { n: 2, at: 0.5 });
    cells(428, 88, [['old', C.muted], ['old', C.muted], ['naya', C.good]], { w: 42, h: 22 });
    L(495, 145, 'purana bekaar', C.muted);
    A(570, 85, 716, 85, C.good, { n: 3, at: 0.3 }); L(625, 73, 'har ~100 ms', C.good);
    dot([730, 85], C.good, '', 'aakhri price', C.good);
    L(450, 335, 'hal: har ~100 ms aakhri price bhejo, purana bekaar', C.good);
  } else if (ph === 3) {
    crowd(C.msg);
    B(250, 20, 170, 60, 'ek server', '~lakh tak', { st: 'bad' }); cross(445, 50);
    A(110, 95, 248, 55, C.bad, { n: 1, at: 0.4, dash: 1 }); L(470, 55, 'sab ek pe? nahi', C.bad, 'start');
    B(250, 190, 100, 60, 'LB', '', { st: 'good' });
    A(110, 180, 248, 220, C.msg, { n: 2, at: 0.4 });
    B(450, 110, 150, 55, 'WS server 1', '', { st: 'good' });
    B(450, 190, 150, 55, 'WS server 2', '', { st: 'good' });
    B(450, 270, 150, 55, 'WS server 3', '', { st: 'good' });
    A(350, 210, 448, 137, C.good, { n: 3, at: 0.5 }); A(350, 220, 448, 217, C.good, { n: 3, at: 0.5 });
    A(350, 230, 448, 297, C.good, { n: 3, at: 0.5 });
    L(745, 200, 'hal: aur server + LB', C.good);
    L(745, 225, 'har server ~lakh', C.good);
  } else {
    B(20, 120, 140, 130, 'Arpan app', '');
    B(380, 120, 160, 130, 'WS server', '', { st: 'good' });
    B(700, 120, 150, 130, 'Pub/Sub', 'kuch rakhta nahi');
    A(698, 185, 542, 185, C.line);
    A(162, 150, 378, 150, C.bad, { n: 1, at: 0.75, dash: 1 }); cross(215, 150); L(290, 138, 'toota', C.bad);
    A(162, 195, 378, 195, C.good, { n: 2, at: 0.15 }); L(275, 183, 'wapas juda', C.good);
    A(378, 240, 162, 240, C.good, { n: 3, at: 0.15 }); L(265, 228, 'abhi ka price alag se', C.good);
  }
}
