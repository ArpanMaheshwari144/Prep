// Card 10: 50 MB video sab US server se khinchte -> buffering + bill. Ilaaj: S3 + paas wala CDN, upload presigned.
const DUR = 17, FIX = 8;
const US = [450, 90], V = [[120, 300], [450, 300], [780, 300]], VN = ['Delhi', 'London', 'Sydney'];
function setup() {
  const fix = grp('fix');
  box(370, 50, 160, 70, 'Hamara US server', '', { id: 'srv' });
  V.forEach(([x, y], i) => box(x - 60, y - 25, 120, 50, VN[i]));
  ['wa', 'wb', 'wc'].forEach((id, i) => wire(`M450 120 L${V[i][0]} ${V[i][1] - 25}`, id));
  box(20, 40, 110, 60, 'Main', 'video upload', { parent: fix });
  box(190, 45, 120, 50, 'S3', '', { parent: fix, stroke: C.acc }); wire('M130 70 H190', '', fix);
  V.forEach(([x, y], i) => { box(x - 50, y - 110, 100, 40, `CDN ${VN[i]}`, '', { parent: fix, stroke: C.good });
    wire(`M250 95 L${x} ${y - 110}`, '', fix); });
}
function frame(t, fix, tl) {
  vis('fix', fix); vis('srv', !fix); ['wa', 'wb', 'wc'].forEach(id => vis(id, !fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: 50 MB video, Delhi / London / Sydney sab hamare US server se khinchte';
    V.forEach((v, i) => { const s = 0.3 + i * 0.2, d = 3.5;
      const f = Math.min(1, Math.max(0, (tl - s) / d));
      dot(lerp([450, 120], [v[0], v[1] - 25], f), C.msg);
      T(v[0], v[1] + 45, f < 1 ? 'buffering...' : 'aayi, der se', { c: f < 1 ? C.bad : C.acc, size: 12, b: 1 }); });
    T(560, 85, `bandwidth bill: Rs ${Math.round(Math.min(1, tl / 5) * 90000)}`, { c: C.bad, b: 1, size: 14, a: 'start' });
    return;
  }
  CAP.textContent = 'ILAAJ: video S3 me (seedha upload, presigned link), dekhne wale ko paas wale CDN se';
  dot(path([[0.3, [130, 70]], [0.9, [190, 70]]], tl < 0.9 ? tl : -1), C.acc, '', '50 MB', C.acc);
  if (tl > 1) T(330, 75, 'tweet me sirf link', { c: C.muted, size: 12, a: 'start' });
  V.forEach((v, i) => { const s = 1.4 + i * 0.3;
    if (tl > s) dot(path([[s, [v[0], v[1] - 70]], [s + 0.6, [v[0], v[1] - 25]]], tl < s + 0.6 ? tl : -1), C.good);
    if (tl > s + 0.6) T(v[0], v[1] + 45, 'turant chali', { c: C.good, size: 12, b: 1 }); });
}
