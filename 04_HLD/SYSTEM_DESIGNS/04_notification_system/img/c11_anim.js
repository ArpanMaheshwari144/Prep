// Card 11: raat 2 baje offer SMS, jabki band kiya tha. Ilaaj: pehle pasand + shaanti (cache): offer SMS band, offer push subah 8 baje, OTP upar.
const DUR = 18, FIX = 8;
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  wire('M130 185 H190'); wire('M340 185 H700');
  wire('M265 220 V280', 'f0'); wire('M340 220 L400 280', 'f1');
  box(20, 150, 110, 70, 'Kafka', 'offer + OTP');
  box(190, 150, 150, 70, 'Notification Svc');
  box(190, 280, 150, 60, 'User-pref', 'cache', { id: 'up', stroke: C.acc });
  box(370, 280, 130, 60, 'Template', 'cache', { id: 'tp', stroke: C.acc });
  box(700, 140, 160, 80, 'User phone');
}
function frame(t, fix, tl) {
  ['f0', 'f1', 'up', 'tp'].forEach(i => vis(i, fix));
  const day = fix && tl > 7.0;
  T(880, 30, day ? 'subah 8:00' : 'raat 2:00', { c: day ? C.good : C.acc, b: 1, size: 15, a: 'end' });
  if (!fix) {
    CAP.textContent = 'DIKKAT: pasand dekhi hi nahi. Raat 2 baje offer SMS, jabki user ne band kiya tha';
    dot(mv([130, 185], [190, 185], 0.3, 0.8, tl), C.acc);
    dot(mv([340, 185], [688, 185], 1.0, 2.2, tl), C.acc, '', 'offer SMS', C.acc);
    if (tl > 2.2) { T(780, 250, 'raat 2 baje ghanti', { c: C.bad, b: 1, size: 13 });
                    T(780, 275, 'SMS band kiya tha!', { c: C.bad, b: 1, size: 13 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: bhejne se pehle pasand + shaanti dekho. Offer roko, subah 8 baje. OTP upar';
  dot(mv([130, 185], [190, 185], 0.3, 0.8, tl), C.acc);
  dot(mv([265, 220], [265, 268], 0.9, 1.4, tl), C.acc, '', 'pasand?', C.acc);
  if (tl > 1.4) T(265, 252, 'offer SMS band · raat 10-8 shaanti', { c: C.acc, b: 1, size: 12, halo: 1 });
  if (tl > 1.6 && tl < 7.2) { dot([600, 260], C.acc, '', 'offer push', C.acc);
    T(600, 290, 'ruka: subah 8 baje', { c: C.acc, b: 1, size: 12 }); T(600, 310, '(scheduled_at)', { c: C.muted, size: 11 }); }
  if (tl > 1.6) T(450, 125, 'offer SMS: roka', { c: C.bad, b: 1, size: 12 });
  dot(mv([130, 185], [190, 185], 3.4, 3.9, tl), C.good);
  dot(mv([340, 185], [688, 185], 4.1, 5.1, tl), C.good, '', 'OTP', C.good);
  if (tl > 5.1) T(780, 250, 'OTP: raat me bhi', { c: C.good, b: 1, size: 13 });
  dot(mv([600, 260], [688, 200], 7.2, 8.0, tl), C.good, '', 'offer push', C.good);
  if (tl > 8.0) T(780, 275, 'offer: subah 8 baje', { c: C.good, b: 1, size: 13 });
}
