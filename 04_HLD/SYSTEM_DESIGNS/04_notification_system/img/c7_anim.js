// Card 7: 12 baje sale, OTP 50,000 offer ke peeche ek hi line me, 10 min late, login fail.
// Ilaaj: OTP ki apni tez line + apne worker. Agar: OTP line khaali -> OTP worker beech wali padh lein, ulta nahi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 12 baje sale. Mera OTP ek hi line me 50,000 offer ke peeche. 10 min late'],
  [3, GRN, 'ILAAJ', 'ILAAJ: OTP ki apni tez line aur apne worker. Order beech wali, offer neeche wali'],
  [6, YEL, 'AGAR', 'AGAR OTP line khaali, OTP worker khaali baithe? -> beech wali padh lein. Ulta kabhi nahi'],
];
function setup() {}
function lines(st, sub, wsub) {
  [['High: OTP', 'good'], ['Medium: order', 'acc'], ['Low: offer', '']].forEach(([l, s], i) => {
    const y = 30 + i * 105;
    B(30, y, 380, 65, l, sub[i], { st: st ? s : '' });
    B(560, y, 200, 65, ['OTP worker', 'order worker', 'offer worker'][i], wsub[i], { st: st ? s : '' });
  });
}
const STEPS = [
  [[C.acc, 'Ek line → Worker', '50,000 offer pehle'], [C.bad, 'OTP', 'baari 10 min baad. Umar 5 min: bekaar']],
  [[C.good, 'High → OTP worker', 'OTP turant'], [C.acc, 'Medium → order worker', 'order thoda baad'], [C.acc, 'Low → offer worker', 'offer apni speed se']],
  [[C.good, 'Medium → OTP worker', 'OTP line khaali: order padh liya'], [C.bad, 'offer worker', 'OTP line kabhi nahi (ulta nahi)']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 560, 100, 'Ek hi line', '', { st: 'bad' });
    cells(35, 155, [['offer', C.muted], ['offer', C.muted], ['... 50,000 offer ...', C.muted], ['OTP', C.bad]], { w: 125, h: 34 });
    B(680, 120, 170, 80, 'Worker', 'offer bhej raha');
    A(580, 160, 678, 160, C.muted, { n: 1 });
    num(470, 245, 2, C.bad); L(490, 250, 'OTP ki baari 10 min baad', C.bad, 'start');
    L(450, 320, 'OTP ki umar 5 min. Pahuncha tab tak bekaar, login fail', C.bad);
  } else if (ph === 1) {
    lines(true, ['Arpan ka OTP', 'order update', '50,000 offer'], ['turant', 'thoda baad', 'apni speed']);
    [0, 1, 2].forEach(i => A(410, 62 + i * 105, 558, 62 + i * 105, [C.good, C.acc, C.muted][i], { n: i + 1 }));
    L(450, 345, 'OTP kabhi kisi ke peeche nahi: apni line, apne worker', C.good);
  } else {
    lines(true, ['khaali', 'order update', '50,000 offer'], ['khaali baitha', '', '']);
    A(410, 160, 558, 75, C.good, { n: 1, dash: 1 }); L(575, 120, 'khaali the: order padha', C.good, 'start');
    num(790, 278, 2, C.bad); L(660, 330, 'offer worker OTP line kabhi nahi', C.bad);
    L(250, 345, 'tez worker neeche madad karte, neeche wale upar nahi', C.good);
  }
}
