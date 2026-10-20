// Card 9: merchant ki mahine bhar ki report primary pe, mera payment ruka. Ilaaj: report read replica pe.
// Agar: pay kiya, turant balance dekha -> primary se padho, copy peeche.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: merchant ki mahine bhar ki report primary pe chali. Mera payment ruka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: bhaari report copy pe (read replica). Primary sirf payment ke liye'],
  [6, YEL, 'AGAR', 'AGAR pay kiya, turant balance dekha? -> primary se padho, copy peeche'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 40, 180, 70, 'Report Svc', 'mahine bhar', { st: 'bad' });
    B(20, 240, 180, 70, 'Payment Svc', 'Arpan ka 500');
    B(500, 130, 240, 90, 'Primary DB', 'report me busy', { st: 'bad' });
    bar(520, 190, 200, 12, 0.95, '');
    A(200, 75, 498, 150, C.bad, { n: 1 }); L(330, 90, 'bhaari query', C.bad);
    A(200, 275, 498, 200, C.acc, { n: 2, dash: 1 }); L(330, 270, 'payment', C.acc);
    num(620, 270, 3, C.bad); L(640, 275, 'payment ruka', C.bad, 'start');
    L(450, 345, 'ek report ne paisa wala kaam rok diya', C.bad);
  } else {
    if (ph === 1) {
      B(20, 40, 180, 70, 'Payment Svc', 'Arpan ka 500', { st: 'good' });
      B(20, 240, 180, 70, 'Report Svc', 'mahine bhar');
      B(500, 40, 220, 70, 'Primary DB', 'sirf payment', { st: 'good' });
      B(500, 240, 220, 70, 'Read replica', 'copy', { st: 'acc' });
      A(200, 75, 498, 75, C.good, { n: 1 }); L(350, 60, 'payment', C.good);
      A(610, 112, 610, 238, C.muted, { n: 2, dash: 1 }); L(625, 175, 'copy, 1-2 sec peeche', C.muted, 'start');
      A(200, 275, 498, 275, C.acc, { n: 3 }); L(350, 260, 'report', C.acc);
      L(450, 345, 'report kitni bhi bhaari ho, payment tez', C.good);
    } else {
      B(20, 130, 200, 80, 'Payment Svc', 'Arpan ne 500 diye');
      B(620, 30, 250, 80, 'Primary DB', 'balance: 500', { st: 'good' });
      B(620, 240, 250, 80, 'Read replica', 'balance: 1000 (purana)', { st: 'bad' });
      A(220, 140, 618, 55, C.msg, { n: 1 }); L(400, 80, 'pay: 1000 -> 500', C.msg);
      A(220, 165, 618, 90, C.good, { n: 2, at: 0.75 }); L(400, 155, 'balance? -> 500', C.good);
      A(220, 195, 618, 270, C.bad, { dash: 1 }); cross(420, 232); L(470, 270, 'yahan se 1000 dikhta', C.bad);
      A(745, 112, 745, 238, C.muted, { dash: 1 }); L(760, 175, 'abhi pahunchi nahi', C.muted, 'start');
      L(450, 345, 'paisa aur haal hamesha primary se. Replica sirf report', C.good);
    }
  }
}
