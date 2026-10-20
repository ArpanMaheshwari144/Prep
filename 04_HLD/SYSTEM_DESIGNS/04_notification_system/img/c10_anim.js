// Card 10: "Diwali offer sabko", 1 crore log, ek request me ek-ek, ghanton atka, OTP bhi peeche.
// Ilaaj: 1000-1000 ke tukde low line me, campaign row me kahan tak. Agar: tukde me 600 bhej ke worker gira.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: admin ne "sabko bhejo" dabaya. Ek hi request me 1 crore ek-ek, ghanton atka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 1000-1000 ke tukde, low line me. Har tukde ke baad campaign row me "kahan tak"'],
  [6, YEL, 'AGAR', 'AGAR tukde me 600 bhej ke worker gira? -> tukda dobara, par pehle wale 600 chhoot jaate'],
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
  [[C.msg, 'Admin → Campaign Svc', 'Admin ne "sabko bhejo" dabaya, 1 crore ek hi request me'], [C.bad, 'beech me gira', 'Beech me request giri to shuru se dobara bhejna pada'], [C.bad, 'OTP', 'Itne bade kaam ke peeche OTP atak gaya']],
  [[C.acc, 'Campaign Svc → Low line', 'Campaign Svc ne 1000-1000 ke tukde Low line me daale'], [C.acc, 'Low line → Worker', 'Worker line se ek-ek tukda uthake bhejta raha'], [C.good, 'Worker → Campaign row', 'Har tukde ke baad aakhri_id = 3000 likha, kahan tak gaya']],
  [[C.good, 'Worker-A → 600 log', 'Worker-A ne tukda 4 me se 600 logon ko bhej diya'], [C.bad, 'Worker-A mara', 'Worker-A mara, aakhri_id 3000 pe hi reh gaya'], [C.acc, 'Worker-B → Sent list', 'Worker-B ne sent list me dekha kisko pehle ja chuka'], [C.good, 'Worker-B → Campaign row', 'Sirf bache 400 ko bheja, phir aakhri_id = 4000 likha']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 140, 60, 'Admin', 'sabko bhejo');
    B(300, 120, 220, 100, 'Campaign Svc', 'ek request: 1 crore', { st: 'bad' });
    bar(320, 180, 180, 14, 0.15, ''); T(410, 245, '15 lakh / 1 crore, 2 ghante se', { c: C.bad, size: 12 });
    A(160, 170, 298, 170, C.msg, { n: 1 });
    num(620, 140, 2, C.bad); L(640, 145, 'beech me gira: shuru se', C.bad, 'start');
    num(620, 190, 3, C.bad); L(640, 195, 'OTP bhi peeche atka', C.bad, 'start');
    L(450, 330, 'ek bada kaam, ek hi jagah atka. Rokne ka bhi tareeka nahi', C.bad);
  } else if (ph === 1) {
    B(20, 130, 150, 70, 'Campaign Svc', '1 crore');
    B(250, 20, 380, 100, 'Low line (offer)', '', { st: 'acc' });
    cells(265, 76, [['1-1000', C.good], ['1001-2000', C.good], ['2001-3000', C.good], ['...', C.line]], { w: 82, h: 30 });
    B(250, 230, 380, 70, 'Campaign row', 'aakhri_id = 3000, haal: CHALU', { st: 'good' });
    B(720, 40, 150, 60, 'Worker', 'tukda bhejta');
    A(170, 150, 248, 75, C.acc, { n: 1 }); L(195, 100, 'tukde', C.acc);
    A(630, 70, 718, 70, C.acc, { n: 2 });
    A(790, 102, 632, 255, C.good, { n: 3 }); L(740, 200, 'kahan tak likha', C.good, 'start');
    L(450, 345, 'gira to wahin se. Galat offer? row pe CANCELLED, agla tukda ruk jaata', C.good);
  } else {
    panel(0, 'tukda 4', ['Worker-A', '3001-4000', ''], ['600 log', 'offer pahuncha', 'good'], 'bheja', true, C.good, '600 tak gaya');
    panel(1, 'gira', ['Worker-A', 'mara', 'bad'], ['Campaign row', 'aakhri_id = 3000', 'acc'], '', true, C.bad, '4000 likha hi nahi');
    A(295, 110, 295, 158, C.bad, { dash: 1 }); cross(295, 134); T(310, 139, '4000', { c: C.bad, b: 1, size: 14, a: 'start' });
    A(295, 110, 295, 158, C.bad, { dash: 1 }); cross(295, 134); T(310, 139, '4000', { c: C.bad, b: 1, size: 14, a: 'start' });
    panel(2, 'tukda dobara', ['Worker-B', '3001-4000', ''], ['Sent list', '(campaign, user)', 'acc'], 'poochha', true, C.acc, 'kisko pehle gaya?');
    panel(3, 'sirf baaki', ['Worker-B', '400 ko bheja', 'good'], ['Campaign row', 'aakhri_id = 4000', 'good'], 'likha', true, C.good, '600 ko dobara nahi');
    L(450, 320, 'tukda dobara chala, par ek user ko ek hi baar', C.good);
  }
}
