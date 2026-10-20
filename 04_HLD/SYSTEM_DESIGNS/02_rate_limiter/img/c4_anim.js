// Card 4: akela Redis gira, limiter andha. Ilaaj: copy (replica) + pehle se tay fail-open / fail-closed.
const DUR = 18, FIX = 7;
const GW = [252, 205], R1 = [478, 110], R2 = [478, 275];
const LOGIN = [80, 150], FEED = [80, 235];
function setup() {
  wire('M150 150 L255 185'); wire('M150 235 L255 200');
  wire('M345 185 L490 115'); wire('M345 200 L490 270', 'wr2');
  box(20, 125, 130, 50, 'login / OTP'); box(20, 210, 130, 50, 'feed / search');
  box(255, 160, 90, 60, 'Gateway', 'limiter', { stroke: C.acc });
  box(490, 75, 140, 70, 'Redis main', 'ginti');
  box(490, 240, 140, 70, 'Redis copy', 'taiyaar', { id: 'r2' });
}
function frame(t, fix, tl) {
  vis('r2', fix); vis('wr2', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: ek hi Redis tha. Wo gira, to kisi ko pata nahi kaun kitni bhej chuka';
    if (tl > 1.2) cross(660, 110);
    const a = path([[1.6, LOGIN], [2.2, GW], [2.8, R1]], tl), b = path([[2.0, FEED], [2.6, GW], [3.2, R1]], tl);
    dot(a, C.msg, '', tl > 2.8 ? '?' : ''); dot(b, C.msg, '', tl > 3.2 ? '?' : '');
    if (tl > 3.6) T(560, 175, 'ginti gayi. andar do ya roko?', { c: C.bad, b: 1, size: 13 });
    return;
  }
  const allDown = tl > 4.8;
  CAP.textContent = allDown ? 'ILAAJ: copy bhi gayi? Pehle se tay: login band (fail-closed), feed khula (fail-open)'
                            : 'ILAAJ: Redis ki copy taiyaar. Main gira to copy main ban jaati (Redis Cluster)';
  if (tl > 0.8) cross(660, 110);
  T(560, 330, tl > 1.6 && !allDown ? 'ab yahi MAIN' : '', { c: C.good, b: 1, size: 13 });
  if (!allDown) {
    dot(path([[1.8, LOGIN], [2.4, GW], [3.0, R2]], tl), C.good);
    if (tl > 3.0) T(420, 305, 'ginti chalu', { c: C.good, b: 1, size: 12 });
    dot(path([[2.2, FEED], [2.8, GW], [3.4, R2]], tl), C.good);
  } else {
    cross(660, 275);
    const a = path([[5.2, LOGIN], [5.8, [205, 172]]], tl), b = path([[5.4, FEED], [6.0, GW], [7.0, [470, 192]]], tl);
    dot(a, tl > 5.8 ? C.bad : C.msg, '', tl > 5.8 ? 'band (fail-closed)' : '', C.bad);
    dot(b, tl > 6.0 ? C.good : C.msg, '', tl > 6.0 ? 'khula (fail-open)' : '', C.good);
  }
}
