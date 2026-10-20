// Card 4: 5 source, 5 URL, ek hi khabar feed me 5 baar.
// Ilaaj: dekha URL Bloom se, wahi khabar title ke hash se. Agar: "wins" / "won" hash alag -> SimHash. Wapas.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har naya URL = nayi khabar. 5 source, feed me 5 baar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: dekha URL Bloom se, wahi khabar title ke hash se. Feed me ek baar'],
  [6, YEL, 'AGAR', 'AGAR "wins" aur "won", hash alag? -> SimHash: 90% same to duplicate'],
  [9, GRN, 'WAPAS', 'WAPAS: same hash = same khabar, URL alag ho tab bhi. Feed me ek hi baar'],
];
const UY = [20, 85, 150, 215, 280];
function setup() {}
function urls(n, c) {
  L(185, 22, '5 source', c || C.msg, 'start');
  UY.forEach((y, i) => {
    B(20, y, 110, 44, 'url ' + (i + 1), '');
    A(130, y + 22, 218, 180, c || C.msg, { n, at: 0.45 });
  });
}
function feed(rows) {
  B(640, 100, 240, 200, 'User ki feed', '');
  rows.forEach(([s, c], i) => T(760, 150 + i * 28, s, { c, b: 1, size: 13 }));
}
function frame(ph) {
  if (ph === 0) {
    urls(1);
    B(220, 145, 130, 70, 'Worker', 'URL naya?', { st: 'bad' });
    num(232, 245, 2, C.bad); L(250, 250, 'naya URL = nayi khabar', C.bad, 'start');
    A(350, 185, 638, 200, C.bad, { n: 3 }); L(495, 180, '5 baar', C.bad);
    feed(Array(5).fill(['match jeeta', C.bad]));
  } else if (ph === 1) {
    urls(1);
    B(220, 145, 130, 70, 'Worker', 'title -> hash', { st: 'good' });
    B(440, 30, 160, 60, 'Bloom', 'Redis, saanjha', { st: 'acc' });
    A(300, 143, 438, 75, C.acc, { n: 2 }); L(330, 100, 'dekha?', C.acc);
    A(350, 185, 638, 200, C.good, { n: 3 }); L(495, 180, 'url 1: naya', C.good);
    L(250, 250, 'url 2-5: same hash -> skip', C.acc, 'start');
    feed([['match jeeta', C.good]]);
  } else if (ph === 2) {
    B(20, 85, 130, 44, 'title: wins', ''); B(20, 215, 130, 44, 'title: won', '');
    A(150, 107, 218, 165, C.msg, { n: 1 }); A(150, 237, 218, 195, C.msg, { n: 1 });
    B(220, 145, 130, 70, 'Worker', 'title -> hash', { st: 'acc' });
    cells(220, 240, [['hash a', C.bad], ['hash b', C.bad]], { w: 63 });
    num(232, 290, 2, C.bad); L(250, 295, 'hash alag', C.bad, 'start');
    B(440, 30, 160, 60, 'SimHash', '90% same', { st: 'good' });
    A(300, 143, 438, 75, C.good, { n: 3 }); L(330, 100, 'milao', C.good);
    A(350, 185, 638, 200, C.good, { n: 4 }); L(495, 180, 'duplicate, ek baar', C.good);
    feed([['match jeeta', C.good]]);
    L(440, 330, 'hal: SimHash: 90% same to duplicate', C.good, 'start');
  } else {
    urls(1, C.good);
    B(220, 145, 130, 70, 'Worker', 'title -> hash', { st: 'good' });
    B(440, 30, 160, 60, 'Bloom', 'Redis, saanjha', { st: 'good' });
    A(300, 143, 438, 75, C.good, { n: 2 }); L(330, 100, 'dekha?', C.good);
    A(350, 185, 638, 200, C.good, { n: 3 }); L(495, 180, 'ek hi baar', C.good);
    L(250, 250, 'URL alag, hash same', C.good, 'start');
    feed([['match jeeta', C.good]]);
  }
}
