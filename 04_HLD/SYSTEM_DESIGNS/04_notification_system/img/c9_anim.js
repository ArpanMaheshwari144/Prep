// Card 9: company ne sirf "le liya" bola, phone tak pahuncha ya nahi = andhera. Ilaaj: SID-9 save, webhook "SID-9 pahuncha" -> Tracking DB DELIVERED.
const DUR = 18, FIX = 8;
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  wire('M150 185 H330'); wire('M470 175 L700 70');
  wire('M470 200 L700 290', 'ww'); wire('M700 300 H470', 'wd'); wire('M120 220 L330 300', 'wdb');
  box(30, 150, 120, 70, 'Worker', 'SMS bheje');
  box(330, 150, 140, 70, 'Twilio', 'company');
  box(700, 40, 150, 60, 'User phone');
  box(700, 260, 160, 60, 'Webhook Svc', '', { id: 'wh', stroke: C.acc });
  box(330, 275, 140, 60, 'Tracking DB', '', { id: 'db', stroke: C.acc });
}
function frame(t, fix, tl) {
  ['ww', 'wd', 'wdb', 'wh', 'db'].forEach(i => vis(i, fix));
  dot(mv([150, 185], [318, 185], 0.3, 1.0, tl), C.msg);
  if (!fix) {
    CAP.textContent = 'DIKKAT: company ne sirf "le liya" bola. Phone tak pahuncha ya nahi, aage andhera';
    dot(mv([330, 185], [162, 185], 1.2, 1.9, tl), C.good, '', 'le liya', C.good);
    if (tl > 1.9) T(90, 245, 'haal: bheja', { c: C.acc, b: 1, size: 13 });
    dot(mv([470, 175], [688, 75], 2.4, 3.2, tl), C.msg);
    if (tl > 3.6) { T(775, 130, 'pahuncha? pata nahi', { c: C.bad, b: 1, size: 13 });
                    T(90, 270, 'mila? andhera', { c: C.bad, b: 1, size: 13 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: worker SID-9 save kare. Webhook "SID-9 pahuncha" aaya -> Tracking DB me DELIVERED';
  dot(mv([330, 185], [162, 185], 1.2, 1.8, tl), C.good, '', 'SID-9', C.good);
  dot(mv([120, 220], [318, 296], 2.0, 2.6, tl), C.acc, '', 'SID-9', C.acc);
  if (tl > 2.6) T(585, 335, tl > 6.3 ? 'SID-9: DELIVERED' : 'SID-9: SENT', { c: tl > 6.3 ? C.good : C.acc, b: 1, size: 13 });
  dot(mv([470, 175], [688, 75], 3.0, 3.8, tl), C.good);
  if (tl > 3.8) T(775, 125, 'pahuncha', { c: C.good, b: 1, size: 13 });
  dot(mv([470, 200], [688, 285], 4.4, 5.2, tl), C.acc, '', 'SID-9 pahuncha (signed)', C.acc);
  dot(mv([700, 300], [482, 300], 5.6, 6.3, tl), C.good, '', 'DELIVERED', C.good);
}
