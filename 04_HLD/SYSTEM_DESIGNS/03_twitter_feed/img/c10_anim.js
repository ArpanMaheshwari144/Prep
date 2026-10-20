// Card 10: 50 MB video sab US server se khinchte -> buffering + bill. Ilaaj: S3 + paas wala CDN, upload presigned.
// Agar: CDN ke paas nahi, photo hata di par dikh rahi, private photo. Wapas: CDN ke paas copy, agli baar paas se.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 50 MB video, Delhi / London / Sydney sab hamare US server se'],
  [3, GRN, 'ILAAJ', 'ILAAJ: video S3 me (seedha upload, presigned link), dekho paas wale CDN se'],
  [6, YEL, 'AGAR', 'AGAR CDN ke paas video nahi, koi dekhne aaya? -> CDN S3 se laake rakh leta'],
  [9, YEL, 'AGAR', 'AGAR photo hata di, CDN pe abhi dikh rahi? -> CDN ko "hatao" bolo'],
  [12, YEL, 'AGAR', 'AGAR private photo, sabko dikhegi? -> nahi, link pe hastakshar + expiry'],
  [15, GRN, 'WAPAS', 'WAPAS: CDN ke paas copy rakhi, agli baar paas se turant'],
];
function setup() {}
const VX = [60, 390, 720], VN = ['Delhi', 'London', 'Sydney'];
function viewers(st) { VX.forEach((x, i) => B(x, 270, 120, 50, VN[i], '', { st: st ? st[i] : '' })); }
function cdns(st, sub) { VX.forEach((x, i) => B(x, 150, 120, 45, `CDN ${VN[i]}`, '', { st: st ? st[i] : '' })); }
function s3(st, sub) { B(220, 25, 140, 50, 'S3', sub || '', { st }); }
function frame(ph) {
  if (ph === 0) {
    B(370, 20, 160, 65, 'Hamara US server', '50 MB video', { st: 'bad' });
    viewers(['bad', 'bad', 'bad']);
    VX.forEach((x, i) => A(450, 87, x + 60, 268, C.bad, { n: i === 1 ? 1 : 0, at: 0.5 }));
    VX.forEach(x => L(x + 60, 340, 'buffering', C.bad));
    num(600, 50, 2, C.bad); L(620, 55, 'bandwidth ka bill', C.bad, 'start');
  } else if (ph === 1) {
    B(20, 20, 120, 60, 'Main', 'video upload');
    s3('good'); A(140, 50, 218, 50, C.acc, { n: 1 }); L(80, 100, 'presigned link', C.acc);
    L(380, 55, 'tweet me sirf CDN link', C.muted, 'start');
    cdns(['good', 'good', 'good']); viewers(['good', 'good', 'good']);
    VX.forEach((x, i) => A(290, 77, x + 60, 148, C.acc, { dash: 1, n: i === 1 ? 2 : 0, at: 0.6 }));
    VX.forEach((x, i) => A(x + 60, 197, x + 60, 268, C.good, { n: 3 }));
    VX.forEach(x => L(x + 60, 340, 'paas se, turant', C.good));
  } else if (ph === 2) {
    s3('good');
    cdns(['acc', 'dim', 'dim']); viewers(['', 'dim', 'dim']);
    A(95, 268, 95, 197, C.msg, { n: 1 }); L(40, 245, 'dekho', C.msg);
    L(190, 178, 'miss: nahi hai', C.acc, 'start');
    A(260, 77, 160, 148, C.acc, { n: 2 }); L(250, 125, 'S3 se laaya', C.acc, 'start');
    A(145, 197, 145, 268, C.good, { n: 3 }); L(165, 240, 'ab paas rakhi', C.good, 'start');
    L(600, 345, 'hal: CDN S3 se laake kuch der rakh leta', C.good);
  } else if (ph === 3) {
    B(20, 20, 120, 60, 'Main', 'photo hatao');
    s3('bad', 'photo hata di'); num(390, 50, 1, C.bad); L(410, 55, 'S3 se gayi', C.bad, 'start');
    A(140, 50, 218, 50, C.bad);
    cdns(['bad', 'dim', 'dim']); viewers(['bad', 'dim', 'dim']);
    L(190, 178, 'purani copy', C.bad, 'start');
    A(120, 197, 120, 268, C.bad, { n: 2 }); L(200, 245, 'abhi bhi dikhi', C.bad, 'start');
    A(80, 82, 80, 148, C.acc, { dash: 1, n: 3 }); L(40, 120, 'hatao', C.acc, 'end');
    L(620, 120, 'hal: CDN ko "hatao" bolo (invalidation)', C.good);
    L(620, 145, 'ya link me version, nayi file', C.good);
  } else if (ph === 4) {
    s3('good', 'private photo');
    B(390, 150, 120, 45, 'CDN', '', { st: 'good' });
    A(360, 60, 440, 148, C.line, { dash: 1 });
    B(60, 270, 140, 55, 'Ravi', 'signed link', { st: 'good' });
    B(700, 270, 160, 55, 'koi aur', 'bina hastakshar', { st: 'bad' });
    A(390, 190, 200, 290, C.good, { n: 1, at: 0.5 }); L(270, 220, 'dikhi (thodi der)', C.good, 'end');
    A(700, 290, 545, 205, C.bad, { n: 2, at: 0.3 }); cross(530, 210); L(600, 210, 'mana', C.bad, 'start');
    L(690, 55, 'hal: link pe hastakshar + expiry', C.good);
  } else {
    s3('');
    cdns(['good', 'good', 'good']); viewers(['good', 'good', 'good']);
    VX.forEach(x => A(290, 77, x + 60, 148, C.line, { dash: 1 }));
    VX.forEach((x, i) => {
      A(x + 35, 268, x + 35, 197, C.msg, { n: i === 0 ? 1 : 0 });
      A(x + 85, 197, x + 85, 268, C.good, { n: i === 0 ? 2 : 0 });
    });
    L(160, 345, 'CDN ke paas copy: paas se turant', C.good);
  }
}
