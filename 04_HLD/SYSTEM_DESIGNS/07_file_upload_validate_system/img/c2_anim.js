// Card 2: 5 GB server ke raaste S3. Ilaaj: presigned URL, bytes seedha S3. Agar: 50 GB bheje -> S3 khud reject.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: meri 5 GB video pehle hamare server pe aayi, phir wahi server se S3. Bytes do baar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: server sirf 10 min ka signed URL + T-102 deta. Phone 5 GB seedha S3 pe bhejta'],
  [6, YEL, 'AGAR', 'AGAR had 5 GB, koi 50 GB bhej de? -> had paas me, S3 khud reject'],
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
  [[C.bad, 'Phone → Upload Svc', 'Arpan ki 5 GB video pehle hamare server pe aayi'], [C.bad, 'Upload Svc → S3', 'Phir server ne wahi 5 GB S3 pe bheje, bytes do baar'], [C.bad, 'Upload Svc', 'Server ka thread minuton atka, bojh bhi aur kharcha bhi']],
  [[C.msg, 'Phone → Upload Svc', 'Phone ne server se sirf upload ka paas maanga, bytes nahi bheje'], [C.acc, 'Upload Svc → Phone', 'Server ne 10 min ka signed URL aur T-102 diya'], [C.good, 'Phone → S3', 'Phone ne 5 GB seedha S3 pe bheje, server beech me nahi'], [C.good, 'Phone → Upload Svc', 'Upload ke baad /complete T-102 bheja, ab jaanch line me']],
  [[C.acc, 'Upload Svc → Koi app', 'Server ne paas diya, usme max 5 GB ki had sign thi'], [C.bad, 'Koi app → S3', 'Ek app ne usi paas se 50 GB bhejne ki koshish ki'], [C.good, 'S3 → Koi app', 'Size paas ki had se bahar tha, S3 ne khud reject kiya']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan ka phone', 'video 5 GB');
    B(330, 110, 220, 110, 'Upload Svc', '5 GB beech me', { st: 'bad' });
    bar(350, 180, 180, 12, 0.95, '');
    B(700, 130, 170, 70, 'S3', '');
    A(170, 165, 328, 165, C.bad, { n: 1 }); L(250, 150, '5 GB', C.bad);
    A(550, 165, 698, 165, C.bad, { n: 2 }); L(625, 150, 'phir 5 GB', C.bad);
    num(440, 260, 3, C.bad); L(460, 265, 'thread minuton atka', C.bad, 'start');
    L(450, 320, 'bytes do baar chale, server pe bojh, mehnga bhi', C.bad);
  } else if (ph === 1) {
    panel(0, 'paas maanga', ['Arpan ka phone', 'video 5 GB', ''], ['Upload Svc', '', ''], 'maanga', true, C.msg, 'bytes nahi bheje');
    panel(1, 'paas mila', ['Arpan ka phone', 'URL + T-102', 'good'], ['Upload Svc', 'signed URL, 10 min', 'acc'], 'diya', false, C.acc, 'sirf ek file ke liye');
    panel(2, 'seedha S3', ['Arpan ka phone', '', ''], ['S3', '5 GB aaya', 'good'], '5 GB', true, C.good, 'server beech me nahi');
    panel(3, 'complete', ['Arpan ka phone', '', ''], ['Upload Svc', 'T-102: aaya', 'good'], '/complete', true, C.good, 'ab jaanch line me');
    L(450, 320, 'server sirf paas deta, bojh S3 uthata', C.good);
  } else {
    B(20, 130, 170, 80, 'Koi app', '50 GB bhejna', { st: 'bad' });
    B(360, 30, 200, 70, 'Upload Svc', 'paas: max 5 GB', { st: 'acc' });
    B(680, 130, 190, 80, 'S3', 'size paas se milao', { st: 'good' });
    A(358, 75, 192, 145, C.acc, { n: 1 }); L(265, 90, 'paas (max 5 GB)', C.acc);
    A(190, 170, 678, 170, C.bad, { n: 2, at: 0.4 }); L(270, 158, '50 GB bheja', C.bad);
    A(678, 195, 192, 195, C.good, { n: 3, at: 0.25 }); L(450, 225, 'reject: paas me 5 GB likha', C.good);
    L(450, 300, 'had paas me sign hai, S3 khud rokta. Hamara server kuch nahi karta', C.good);
  }
}
