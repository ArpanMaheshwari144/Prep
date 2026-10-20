// Card 11: raat 2 baje offer SMS, jabki band kiya tha. Ilaaj: pehle pasand + shaanti dekho, offer subah 8 baje.
// Agar: shaanti me OTP aaya, pasand badli par cache purana. Wapas: subah 8 baje job ne offer bheja.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: pasand dekhi hi nahi. Raat 2 baje offer SMS, jabki band kiya tha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehle pasand + shaanti dekho. Shaanti me offer roko, subah 8 baje'],
  [6, YEL, 'AGAR', 'AGAR shaanti ke waqt OTP / login aaya? -> bhejo, zaroori alert upar'],
  [9, YEL, 'AGAR', 'AGAR pasand badli, par cache purana? -> badalte hi cache delete'],
  [12, GRN, 'WAPAS', 'WAPAS: subah 8 baje job ne ruka hua offer bheja, phenka nahi'],
];
function setup() {}
function base(o = {}) {
  B(20, 150, 110, 70, 'Kafka', o.k || 'offer');
  B(190, 150, 150, 70, 'Notification Svc');
  B(190, 280, 150, 60, 'User-pref', o.pref || 'cache', { st: o.prefst || 'acc' });
  B(370, 280, 130, 60, 'Template', 'cache');
  B(700, 140, 160, 80, 'User phone', o.ph, { st: o.phst });
  if (o.job) B(540, 275, 140, 60, 'Job', 'scheduled_at', { st: o.job });
  T(880, 25, o.time || 'raat 2:00', { c: o.time ? C.good : C.acc, b: 1, size: 15, a: 'end' });
}
function frame(ph) {
  if (ph === 0) {
    base({ phst: 'bad', ph: 'SMS band tha' });
    A(130, 185, 188, 185, C.acc, { n: 1 });
    A(340, 185, 698, 185, C.bad, { n: 2 }); L(520, 170, 'offer SMS', C.bad);
    L(780, 250, 'raat 2 baje ghanti', C.bad);
    L(265, 255, 'pasand dekhi hi nahi', C.muted);
  } else if (ph === 1) {
    base({ job: 'acc', ph: 'shaant', phst: 'good', pref: 'offer SMS band' });
    A(130, 185, 188, 185, C.acc, { n: 1 });
    A(265, 222, 265, 278, C.acc, { n: 2 }); L(280, 255, 'pasand?', C.acc, 'start');
    L(130, 300, 'raat 10-8 shaanti', C.acc, 'end');
    A(340, 185, 698, 185, C.bad, { dash: 1 }); cross(520, 185); L(520, 160, 'offer SMS: roka', C.bad);
    A(330, 222, 600, 273, C.acc, { n: 3, at: 0.6 }); L(600, 245, 'offer push: subah 8', C.acc, 'start');
  } else if (ph === 2) {
    base({ k: 'OTP', ph: 'OTP mila', phst: 'good', pref: 'shaanti 10-8' });
    A(130, 185, 188, 185, C.good, { n: 1 });
    A(265, 222, 265, 278, C.acc, { n: 2 }); L(280, 255, 'shaanti hai', C.acc, 'start');
    A(340, 185, 698, 185, C.good, { n: 3 }); L(520, 170, 'OTP: raat me bhi', C.good);
    L(520, 25, 'hal: bhejo. Zaroori alert pasand se upar', C.good);
  } else if (ph === 3) {
    base({ ph: 'SMS band kiya', phst: 'acc', pref: 'purana -> delete', prefst: 'bad' });
    num(700, 120, 1, C.acc); L(718, 125, 'pasand badli', C.acc, 'start');
    AP([[780, 222], [780, 255], [320, 255], [320, 278]], C.acc, { n: 2, at: 0.5 }); L(620, 278, 'cache delete', C.acc);
    A(130, 185, 188, 185, C.acc, { n: 3 });
    A(340, 185, 698, 185, C.bad, { dash: 1 }); cross(520, 185); L(520, 160, 'offer SMS: roka', C.bad);
    L(450, 25, 'hal: badalte hi cache delete + chhoti umar.', C.good);
    L(450, 47, 'bhejne se pehle sahi pasand chahiye', C.good);
  } else {
    base({ job: 'good', ph: 'offer push', phst: 'good', time: 'subah 8:00' });
    num(555, 262, 1, C.good); L(573, 267, '8 baje', C.good, 'start');
    A(660, 273, 740, 222, C.good, { n: 2, at: 0.5 }); L(780, 250, 'offer push gaya', C.good, 'start');
  }
}
