// Card 9: 200 MB video chat server ke raaste -> server 2 min atka, 500 x 200 MB = 100 GB. Ilaaj: S3 presigned URL, message me key.
const DUR = 18, FIX = 8;
const AP = [140, 200], SL = [298, 200], SR = [450, 200], GR = [708, 200], AT = [80, 160], S3L = [298, 70];
function setup() {
  wire('M140 200 H310'); wire('M450 200 H720');
  wire('M80 160 V75 H310', 'ws3');
  box(20, 160, 120, 80, 'Arpan ka app', '200 MB video');
  box(310, 160, 140, 80, 'Chat Server');
  box(310, 30, 140, 70, 'S3', 'video yahan', { id: 's3', stroke: C.acc });
  box(720, 160, 160, 80, 'Group', '500 log');
}
function big(p, c, lab) {
  if (!p) return;
  el('circle', { cx: p[0], cy: p[1], r: 16, fill: c }, DYN);
  if (lab) T(p[0], p[1] - 24, lab, { c, b: 1, size: 12, halo: 1 });
}
function frame(t, fix, tl) {
  vis('s3', fix); vis('ws3', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: video chat server ke raaste. 200 MB pe server 2 min atka, group me 100 GB';
    big(path([[0.4, AP], [3.0, [292, 200]]], tl < 3.0 ? tl : -1), C.bad, '200 MB');
    if (tl > 1.0) {
      el('rect', { x: 310, y: 160, width: 140, height: 80, rx: 8, fill: C.bad, opacity: 0.3 }, DYN);
      T(380, 270, `atka: ${Math.min(2, (tl - 1.0) / 2.5 * 2).toFixed(1)} min`, { c: C.bad, b: 1, size: 13 });
    }
    for (let k = 0; k < 5; k++) {
      const s = 3.2 + k * 0.5;
      if (tl > s && tl < s + 0.8) big(path([[s, SR], [s + 0.8, [692, 200]]], tl), C.bad, '200 MB');
    }
    if (tl > 4.2) T(800, 280, '500 x 200 MB', { c: C.bad, b: 1, size: 13 });
    if (tl > 4.6) T(800, 305, '= 100 GB', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: app video seedha S3 me (presigned URL, 10 min). Message me sirf S3 key + thumbnail';
  if (tl > 0.4 && tl < 1.1) dot(path([[0.4, AP], [1.1, SL]], tl), C.acc, '', 'paas do', C.acc);
  if (tl > 1.1 && tl < 1.8) dot(path([[1.1, SL], [1.8, AP]], tl), C.acc, '', 'presigned URL (10 min)', C.acc);
  big(path([[2.0, AT], [2.8, [80, 75]], [3.6, [292, 75]]], tl < 3.6 ? tl : -1), C.good, '200 MB');
  if (tl > 3.6) T(470, 60, 'video S3 me', { a: 'start', c: C.good, b: 1, size: 13 });
  if (tl > 3.9 && tl < 4.6) dot(path([[3.9, AP], [4.6, SL]], tl), C.msg, '', 'key + thumbnail');
  for (let k = 0; k < 5; k++) {
    const s = 4.8 + k * 0.3;
    if (tl > s && tl < s + 0.6) dot(path([[s, SR], [s + 0.6, GR]], tl), C.msg, '', k === 0 ? 'S3 key' : '');
  }
  if (tl > 5.6) T(380, 270, 'server halka, atka nahi', { c: C.good, b: 1, size: 13 });
  if (tl > 6.6) T(800, 280, '500 ko wahi key', { c: C.good, b: 1, size: 13 });
  if (tl > 6.6) T(800, 305, 'upload: ek', { c: C.good, b: 1, size: 13 });
}
