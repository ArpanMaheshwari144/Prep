// Card 7: Ravi ne mera T-101 daala, file le li. Ilaaj: owner check, 403. Agar: admin -> owner ya admin, warna 403.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Ravi ne mera trackingId T-101 daala. Service ne id dekhi aur meri file de di'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har request pe check: maalik hi maang raha? JWT ravi, T-101 ka maalik arpan: 403'],
  [6, YEL, 'AGAR', 'AGAR admin maange to? -> owner ho ya admin role ho, warna 403'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.msg, 'Ravi → Download Svc', 'Ravi ko Arpan ka id T-101 mil gaya, usne GET T-101 kiya'], [C.acc, 'Download Svc → S3', 'Download Svc ne bas id dekhi aur S3 se file maangi'], [C.acc, 'S3 → Download Svc', 'S3 ne Arpan ki report.pdf de di'], [C.bad, 'Download Svc → Ravi', 'Arpan ki file Ravi ke haath lag gayi']],
  [[C.msg, 'Ravi → Download Svc', 'Ravi ne phir wahi GET T-101 bheja'], [C.acc, 'Download Svc → Status DB', 'Download Svc ne pehle DB se poocha T-101 ka maalik kaun'], [C.acc, 'Status DB → Download Svc', 'DB ne bataya T-101 ka maalik arpan hai'], [C.good, 'Download Svc → Ravi', 'JWT me ravi, maalik arpan, to Ravi ko 403']],
  [[C.msg, 'Admin → Download Svc', 'Admin ne support ke liye GET T-101 kiya'], [C.good, 'Download Svc → S3', 'Admin role tha, to file mil gayi'], [C.msg, 'Ravi → Download Svc', 'Usi waqt Ravi ne bhi GET T-101 bheja'], [C.bad, 'Download Svc → Ravi', 'Na maalik na admin, to Ravi ko 403']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 160, 70, 'Ravi', 'JWT: ravi', { st: 'bad' });
    B(330, 130, 200, 70, 'Download Svc', 'id hai? de do', { st: 'bad' });
    B(690, 130, 180, 70, 'S3', 'Arpan ki report.pdf');
    A(180, 150, 328, 150, C.msg, { n: 1 }); L(255, 135, 'GET T-101', C.msg);
    A(530, 150, 688, 150, C.acc, { n: 2 }); L(610, 135, 'file laao', C.acc);
    A(688, 180, 532, 180, C.acc, { n: 3 });
    A(328, 185, 182, 185, C.bad, { n: 4 }); L(255, 210, 'Arpan ki file', C.bad);
    L(450, 300, 'id screenshot / log se leak hui. Id = chaabi maan li', C.bad);
  } else if (ph === 1) {
    B(20, 130, 160, 70, 'Ravi', 'JWT: ravi', { st: 'bad' });
    B(330, 130, 200, 70, 'Download Svc', 'owner check', { st: 'acc' });
    B(690, 130, 180, 70, 'Status DB', 'T-101 owner: arpan', { st: 'good' });
    A(180, 150, 328, 150, C.msg, { n: 1 }); L(255, 135, 'GET T-101', C.msg);
    A(530, 150, 688, 150, C.acc, { n: 2 }); L(610, 135, 'maalik?', C.acc);
    A(688, 180, 532, 180, C.acc, { n: 3 }); L(610, 205, 'arpan', C.acc);
    A(328, 185, 182, 185, C.good, { n: 4 }); L(255, 210, '403', C.good);
    T(430, 280, 'file.owner_id == jwt.user_id ?  ravi != arpan', { c: C.fg, size: 13 });
    L(450, 330, 'id leak ho tab bhi file nahi milti. userId JWT se, body se kabhi nahi', C.good);
  } else {
    B(20, 40, 170, 70, 'Support admin', 'JWT: role admin', { st: 'good' });
    B(20, 240, 170, 70, 'Ravi', 'JWT: ravi', { st: 'bad' });
    B(350, 130, 230, 90, 'Download Svc', 'owner? ya admin?', { st: 'acc' });
    B(710, 130, 160, 90, 'S3', 'report.pdf');
    A(190, 70, 348, 150, C.msg, { n: 1 }); L(250, 85, 'GET T-101', C.msg);
    A(580, 175, 708, 175, C.good, { n: 2 }); L(645, 160, 'admin: haan', C.good);
    A(190, 265, 348, 200, C.msg, { n: 3, at: 0.7 }); L(240, 222, 'GET T-101', C.msg);
    A(360, 222, 192, 295, C.bad, { n: 4, at: 0.55 }); L(330, 300, 'na owner na admin: 403', C.bad, 'start');
    L(450, 345, 'owner ho ya admin role, warna 403', C.good);
  }
}
