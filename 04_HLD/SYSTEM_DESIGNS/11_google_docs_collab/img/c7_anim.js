// Card 7: net 1 sec gaya. Sab strong -> typing ruki. Ilaaj: likhna AP (pending op), permission CP (SQL check).
const DUR = 18, FIX = 8;
const PH = [170, 170], CS = [418, 170];
function setup() {
  wire('M170 170 H430'); wire('M510 200 V280');
  box(20, 130, 150, 80, 'Main (phone)', 'metro me');
  box(430, 130, 160, 70, 'Conn-Server', 'OT');
  box(430, 280, 160, 60, 'SQL permissions', 'CP');
}
function frame(t, fix, tl) {
  const off = tl >= 1.0 && tl < 3.5;
  if (off) { cross(300, 170); T(300, 140, 'net gaya (1 sec)', { c: C.bad, b: 1, size: 12 }); }
  if (tl >= 3.5) T(300, 140, 'net wapas', { c: C.good, b: 1, size: 12 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: sab kuch strong (turant sab same) rakha -> net jaate hi typing ruk gayi';
    if (tl >= 1.2) { T(95, 245, 'typing ruki', { c: C.bad, b: 1, size: 14 });
                     T(95, 267, 'kaam ruka', { c: C.bad, b: 1, size: 13 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: likhna AP: typing chalti, op phone me pending. Permission CP: net aate hi SQL check';
  const n = tl < 1.0 ? 0 : tl < 3.7 ? Math.min(3, Math.floor((tl - 1.0) / 0.6) + 1) : 0;
  if (n) { T(95, 238, 'pending op', { c: C.acc, size: 12 });
           cells(30, 248, ['a', 'b', 'c'].slice(0, n).map(s => [s, C.acc]), { w: 40 }); }
  if (tl >= 1.0 && tl < 3.5) T(95, 300, 'likhta raha (AP)', { c: C.good, b: 1, size: 13 });
  if (tl > 3.7 && tl < 4.4) dot(path([[3.7, PH], [4.4, CS]], tl), C.acc, '3', 'op + version', C.acc);
  if (tl > 4.5 && tl < 5.0) dot(path([[4.5, [510, 205]], [5.0, [510, 268]]], tl), C.msg, '', 'access?');
  if (tl > 5.0 && tl < 5.5) dot(path([[5.0, [510, 268]], [5.5, [510, 205]]], tl), C.good, '', 'haan', C.good);
  if (tl >= 5.0) T(600, 315, 'pehle permission', { c: C.msg, size: 12, b: 1, a: 'start' });
  if (tl >= 5.7) { T(740, 160, 'khiska ke lagaya', { c: C.good, b: 1, size: 13 });
                   T(740, 182, 'aakhir me sab same', { c: C.good, b: 1, size: 13 }); }
}
