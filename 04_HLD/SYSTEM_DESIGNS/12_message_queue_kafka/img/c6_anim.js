// Card 6: Broker A ki disk mari, data gaya. Ilaaj: leader + ISR copy, acks=all. Agar: leader A gira, naya leader kaun.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: acks=1: leader A ne likhte hi OK bola. Copy se pehle A ki disk mari, khabar gayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: leader A + 2 copy (ISR). Saari ISR copy le lein, tab producer ko OK (acks=all)'],
  [6, YEL, 'AGAR', 'AGAR leader A gir gaya? -> Controller ISR me se B ko leader banata, kuch nahi khota'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Producer → A', 'Order ki khabar sirf leader A pe likhi, turant OK (acks=1)'],
   [C.bad, 'A', 'Copy B, C tak jaane se pehle hi A ki disk mar gayi'],
   [C.bad, 'B, C', 'Copy pe khabar thi hi nahi: producer ko OK mila, par khabar gayi']],
  [[C.msg, 'Producer → A', 'Khabar leader A pe likhi'],
   [C.acc, 'A → B, C', 'ISR copy B aur C ne bhi khabar le li'],
   [C.good, 'A → Producer', 'Tabhi producer ko OK (acks=all): khabar 3 machine pe']],
  [[C.bad, 'A', 'Leader A gir gaya'],
   [C.acc, 'Controller → B', 'Controller (KRaft) ne ISR me se B ko naya leader banaya'],
   [C.good, 'Producer → B', 'Producer ab B pe likhta: B ke paas sab tha, kuch nahi khoya']],
];
function frame(ph) {
  B(15, 140, 170, 60, 'Producer', 'order khabar', { icon: 'server' });
  if (ph === 0) {
    B(330, 135, 200, 70, 'Broker A', 'leader: disk mari', { st: 'bad', icon: 'db' }); cross(555, 170); num(590, 170, 2, C.bad);
    A(185, 160, 328, 160, C.msg, { n: 1 }); A(328, 185, 187, 185, C.msg); L(255, 215, 'turant OK', C.msg);
    B(680, 30, 200, 60, 'Broker B', 'copy: khaali', { st: 'bad', icon: 'db' });
    B(680, 250, 200, 60, 'Broker C', 'copy: khaali', { st: 'bad', icon: 'db' });
    A(530, 140, 678, 65, C.muted, { dash: 1 }); A(530, 200, 678, 275, C.muted, { dash: 1 });
    num(780, 175, 3, C.bad); L(430, 320, 'OK mila, par khabar kahin nahi bachi', C.bad);
  } else if (ph === 1) {
    B(330, 135, 200, 70, 'Broker A', 'leader', { st: 'good', icon: 'db' });
    A(185, 160, 328, 160, C.msg, { n: 1 });
    B(680, 30, 200, 60, 'Broker B', 'ISR copy', { st: 'good', icon: 'db' });
    B(680, 250, 200, 60, 'Broker C', 'ISR copy', { st: 'good', icon: 'db' });
    A(530, 145, 678, 60, C.acc, { n: 2 }); A(530, 195, 678, 280, C.acc);
    A(328, 190, 187, 190, C.good, { n: 3 }); L(255, 225, 'OK (acks=all)', C.good);
    L(430, 320, 'OK = khabar 3 machine pe bach gayi', C.good);
  } else {
    B(330, 230, 200, 70, 'Broker A', 'gir gaya', { st: 'bad', icon: 'db' }); cross(555, 265); num(590, 265, 1, C.bad);
    B(330, 20, 200, 60, 'Controller', 'KRaft', { st: 'acc', icon: 'server' });
    B(680, 120, 200, 70, 'Broker B', 'naya leader', { st: 'good', icon: 'db' });
    A(530, 50, 678, 140, C.acc, { n: 2 });
    A(185, 170, 678, 165, C.good, { n: 3, at: 0.3 });
    B(680, 260, 200, 60, 'Broker C', 'ISR copy', { st: 'dim', icon: 'db' });
    L(430, 345, 'ISR me se hi leader: sab uske paas', C.muted);
  }
}
