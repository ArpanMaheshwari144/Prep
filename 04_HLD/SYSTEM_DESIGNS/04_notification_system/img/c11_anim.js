// Card 11: raat 2 baje offer ka SMS, jabki maine band kiya tha. Ilaaj: bhejne se pehle pasand + shaanti ka waqt dekho,
// shaanti me offer ruko, subah 8 bhejo. Agar: shaanti me OTP aaya -> bhejo, zaroori upar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine offer SMS band kiya tha. Phir bhi raat 2 baje ghanti baji'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehle pasand + shaanti ka waqt dekho. Shaanti me offer ruko, subah 8 bhejo'],
  [6, YEL, 'AGAR', 'AGAR shaanti ke waqt OTP / login alert aaya? -> bhejo. Zaroori alert pasand se upar'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Offer khabar → SMS worker', 'Raat 2 baje offer ki khabar SMS worker ke paas aayi'], [C.bad, 'SMS worker → phone', 'Worker ne seedha bheja, raat ko phone ki ghanti baji'], [C.bad, 'Pasand', 'Arpan ne offer SMS band kiya tha, worker ne ye pasand dekhi hi nahi']],
  [[C.msg, 'Offer khabar → SMS worker', 'Raat 2 baje phir offer ki khabar aayi'], [C.acc, 'SMS worker → Pasand', 'Worker ne pasand dekhi: offer band, shaanti 10 se 8'], [C.good, 'SMS worker → Subah ki line', 'Khabar subah ki line me rakhi, 8 baje jaayegi, phenki nahi']],
  [[C.msg, 'OTP khabar → SMS worker', 'Raat 2 baje Arpan ne login kiya, OTP ki khabar aayi'], [C.muted, 'SMS worker → Pasand', 'Pasand me shaanti ka waqt chal raha tha'], [C.good, 'SMS worker → phone', 'OTP zaroori hai, to shaanti me bhi bheja']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 150, 60, 'Offer khabar', 'raat 2:00');
    B(300, 140, 170, 60, 'SMS worker', 'seedha bheja', { st: 'bad' });
    B(640, 130, 200, 80, 'Arpan ka phone', 'ghanti 2 baje', { st: 'bad' });
    B(300, 280, 170, 50, 'Pasand', 'offer SMS: band', { st: 'dim' });
    A(170, 170, 298, 170, C.msg, { n: 1 }); A(470, 170, 638, 170, C.bad, { n: 2 });
    num(385, 250, 3, C.bad); L(405, 255, 'pasand dekhi hi nahi', C.bad, 'start');
    L(450, 30, 'neend kharab, Arpan gussa', C.bad);
  } else if (ph === 1) {
    B(20, 140, 150, 60, 'Offer khabar', 'raat 2:00');
    B(280, 130, 190, 80, 'SMS worker', 'pehle pasand dekho', { st: 'good' });
    B(560, 20, 280, 70, 'Pasand (cache)', 'offer: band, shaanti 10 se 8', { st: 'acc' });
    B(560, 240, 280, 70, 'Subah ki line', 'bhejo: 8:00 baje', { st: 'good' });
    A(170, 170, 278, 170, C.msg, { n: 1 });
    A(470, 145, 558, 60, C.acc, { n: 2 }); L(530, 130, 'pasand?', C.acc, 'start');
    A(470, 195, 558, 275, C.good, { n: 3 }); L(500, 245, 'abhi nahi, subah', C.good, 'end');
    L(450, 350, 'ghanti nahi baji. Offer phenka nahi, subah 8 baje gaya', C.good);
  } else {
    B(20, 140, 150, 60, 'OTP khabar', 'raat 2:00', { st: 'acc' });
    B(280, 130, 190, 80, 'SMS worker', 'zaroori? haan', { st: 'good' });
    B(560, 20, 280, 70, 'Pasand (cache)', 'shaanti 10 se 8', { st: 'dim' });
    B(640, 220, 200, 80, 'Arpan ka phone', 'OTP aaya', { st: 'good' });
    A(170, 170, 278, 170, C.msg, { n: 1 });
    A(470, 145, 558, 60, C.muted, { n: 2, dash: 1 }); L(530, 130, 'shaanti hai', C.muted, 'start');
    A(470, 195, 638, 255, C.good, { n: 3 }); L(530, 262, 'phir bhi bhejo', C.good, 'end');
    L(450, 345, 'Arpan khud login kar raha, OTP rokna ulta nuksaan', C.good);
  }
}
