// Card 4: call se pehle hi SENT, call fail, Worker-B ne SENT dekh ke chhoda -> OTP gaya hi nahi.
// Ilaaj: SENDING (umar ~60 sec), OK pe SENT. Agar: jawab se pehle mara, call fail, SENDING wala mara, B ko SENDING mili.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: call se pehle hi SENT. Call fail, Worker-B ne SENT dekh ke chhoda'],
  [3, GRN, 'ILAAJ', 'ILAAJ: bhejte waqt SENDING (~60 sec), company OK bole tab SENT'],
  [6, YEL, 'AGAR', 'AGAR bhej diya, jawab se pehle worker mara? -> do baar ja sakta'],
  [9, YEL, 'AGAR', 'AGAR call fail, worker zinda? -> worker khud key hata deta'],
  [12, YEL, 'AGAR', 'AGAR SENDING wala worker hi mar gaya? -> 60 sec me key khud mit jaati'],
  [15, YEL, 'AGAR', 'AGAR Worker-B ko SENDING mili? -> thoda ruko, phir dekho'],
  [18, GRN, 'WAPAS', 'WAPAS: company OK boli, key SENT. Chhodna sirf SENT pe'],
];
function setup() {}
function base(o = {}) {
  B(20, 150, 110, 80, 'Kafka', 'OTP khabar');
  B(270, 50, 140, 70, 'Worker-A', o.as || '', { st: o.a }); B(270, 240, 140, 70, 'Worker-B', '', { st: o.b });
  B(500, 150, 140, 70, 'Redis', o.key || 'OTP key', { st: o.k || 'acc' });
  B(720, 150, 150, 80, 'SMS company', o.otp, { st: o.c });
}
function frame(ph) {
  if (ph === 0) {
    base({ key: 'SENT', k: 'bad', c: 'bad', otp: 'OTP: 0' });
    A(130, 175, 268, 90, C.msg, { n: 1 });
    A(410, 95, 498, 165, C.bad, { n: 2, at: 0.45 }); L(470, 70, 'pehle hi SENT', C.bad, 'start');
    A(410, 70, 718, 160, C.bad, { n: 3, at: 0.75, dash: 1 }); L(795, 255, 'call fail', C.bad);
    A(130, 205, 268, 270, C.msg, { n: 4 }); L(140, 270, 'dobara', C.msg, 'start');
    A(410, 255, 498, 205, C.acc, { n: 5, at: 0.5 }); L(420, 335, 'SENT mili -> chhoda', C.bad, 'start');
    L(795, 290, 'OTP kho gaya', C.bad);
  } else if (ph === 1) {
    base({ key: 'SENT', k: 'good', c: 'good', otp: 'OTP: 1', b: 'dim' });
    A(130, 175, 268, 90, C.msg, { n: 1 });
    A(410, 95, 498, 160, C.acc, { n: 2, at: 0.45 }); L(395, 150, 'SENDING (60 sec)', C.acc, 'end');
    A(410, 70, 718, 160, C.good, { n: 3, at: 0.6 });
    A(718, 185, 642, 185, C.good, { n: 4 }); L(680, 250, 'OK', C.good);
    A(410, 112, 498, 180, C.good, { n: 5, at: 0.75 }); L(570, 240, 'OK -> SENT', C.good);
  } else if (ph === 2) {
    base({ key: 'SENDING', a: 'bad', b: 'good', c: 'acc', otp: 'OTP: 2' });
    A(410, 95, 498, 160, C.acc, { n: 1, at: 0.45 });
    A(410, 70, 718, 160, C.good, { n: 2, at: 0.6 });
    num(255, 30, 3, C.bad); L(273, 35, 'jawab se pehle mara', C.bad, 'start'); cross(435, 45);
    A(130, 205, 268, 270, C.msg, { n: 4 }); L(140, 270, 'dobara', C.msg, 'start');
    A(410, 290, 718, 210, C.good, { n: 5, at: 0.6 }); L(600, 300, 'phir bheja', C.good, 'start');
    L(885, 20, 'hal: maana hua risk. OTP do baar aana, na aane se behtar', C.good, 'end');
  } else if (ph === 3) {
    base({ key: 'khaali', k: 'good', a: 'acc', as: 'zinda', b: 'good', c: 'good', otp: 'OTP: 1' });
    A(410, 95, 498, 160, C.acc, { n: 1, at: 0.45 });
    A(410, 70, 718, 160, C.bad, { n: 2, at: 0.75, dash: 1 }); L(680, 110, 'call fail', C.bad);
    A(410, 112, 498, 180, C.good, { n: 3, at: 0.75 }); L(395, 150, 'key hatao', C.good, 'end');
    A(410, 290, 718, 210, C.good, { n: 4, at: 0.6 }); L(600, 300, 'agli koshish', C.good, 'start');
    L(885, 20, 'hal: koshish adhoori thi, key hatao, agli chal jaaye', C.good, 'end');
  } else if (ph === 4) {
    base({ key: 'khud mit gayi', k: 'good', a: 'bad', b: 'good', c: 'good', otp: 'OTP: 1' });
    A(410, 95, 498, 160, C.acc, { n: 1, at: 0.45 }); L(395, 150, 'SENDING (60 sec)', C.acc, 'end');
    num(255, 30, 2, C.bad); L(273, 35, 'mar gaya, hatane wala koi nahi', C.bad, 'start'); cross(440, 70);
    num(570, 130, 3, C.good); L(590, 135, '60 sec khatam', C.good, 'start');
    A(410, 255, 498, 205, C.acc, { n: 4, at: 0.5 }); L(420, 335, 'key nahi', C.acc, 'start');
    A(410, 290, 718, 215, C.good, { n: 5, at: 0.6 });
    L(885, 20, 'hal: SENDING ki umar khatam, key khud mit jaati', C.good, 'end');
  } else if (ph === 5) {
    base({ key: 'SENDING -> SENT', a: 'good', b: 'acc', c: 'good', otp: 'OTP: 1' });
    A(410, 95, 498, 160, C.acc, { n: 1, at: 0.45 });
    A(410, 70, 718, 160, C.acc, { n: 2, at: 0.6 }); L(680, 110, 'call chal rahi', C.acc);
    A(410, 255, 498, 205, C.acc, { n: 3, at: 0.5 }); L(420, 335, 'SENDING mili -> ruka', C.acc, 'start');
    num(255, 225, 4, C.acc); L(240, 230, 'thoda ruko', C.acc, 'end');
    A(410, 290, 498, 215, C.good, { n: 5, at: 0.4 }); L(630, 300, 'phir dekha: SENT -> chhodo', C.good, 'start');
    L(885, 20, 'hal: thoda ruko, phir dekho. Chhodna sirf SENT pe', C.good, 'end');
  } else {
    base({ key: 'SENT', k: 'good', a: 'good', b: 'good', c: 'good', otp: 'OTP: 1' });
    A(718, 185, 642, 185, C.good, { n: 1 }); L(680, 250, 'OK', C.good);
    A(410, 95, 498, 160, C.good, { n: 2, at: 0.5 }); L(395, 150, 'SENT likha', C.good, 'end');
    A(410, 255, 498, 205, C.good, { n: 3, at: 0.5 }); L(420, 335, 'SENT mili -> chhoda', C.good, 'start');
  }
}
