// Card 9: company ne "le liya" bola, par phone tak pahuncha? Pata nahi. Ilaaj: company ka message no. (SID) save,
// company baad me webhook se batati, Tracking DB me DELIVERED. Agar: nakli webhook -> hastakshar jaancho.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: company ne bas "le liya" bola. Phone tak pahuncha ya nahi, kisi ko pata nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: company ka message no. (SID-9) save karo. Company baad me webhook se batati'],
  [6, YEL, 'AGAR', 'AGAR koi nakli webhook bhej de "pahuncha"? -> hastakshar jaancho, nakli phenko'],
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
  [[C.msg, 'SMS worker → SMS company', 'OTP bheja'], [C.good, 'SMS company → SMS worker', '200 OK: "le liya"'], [C.bad, 'Arpan ka phone', 'pahuncha? kisi ko pata nahi']],
  [[C.acc, 'SMS worker → Tracking DB', 'SID-9: SENT'], [C.good, 'SMS company → phone', 'OTP aaya'], [C.good, 'SMS company → Webhook Svc', '"SID-9 pahuncha"'], [C.good, 'Webhook Svc → Tracking DB', 'SID-9: DELIVERED']],
  [[C.good, 'SMS company → Webhook Svc', 'sahi hastakshar'], [C.good, 'Webhook Svc → Tracking DB', 'DELIVERED likha'], [C.bad, 'Koi bhi → Webhook Svc', 'nakli: hastakshar galat'], [C.bad, 'Webhook Svc', 'phenka, DB me kuch nahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 140, 60, 'SMS worker', 'OTP bheja');
    B(330, 140, 170, 60, 'SMS company', '"le liya"');
    B(680, 140, 170, 60, 'Arpan ka phone', '?', { st: 'bad' });
    A(160, 160, 328, 160, C.msg, { n: 1 }); A(328, 185, 162, 185, C.good, { n: 2 }); L(245, 215, '200 OK', C.good);
    A(500, 170, 678, 170, C.muted, { dash: 1 }); L(590, 155, 'aage andhera', C.bad);
    num(765, 230, 3, C.bad); L(765, 255, 'pahuncha? pata nahi', C.bad);
    L(450, 330, '"bheja" aur "mila" do alag haal. Hum sirf pehla jaante the', C.bad);
  } else if (ph === 1) {
    panel(0, 'SID save', ['SMS worker', 'OTP bheja', ''], ['Tracking DB', 'SID-9: SENT', 'acc'], 'likha', true, C.acc, 'company ka no.');
    panel(1, 'phone tak', ['SMS company', 'SID-9', ''], ['Arpan ka phone', 'OTP aaya', 'good'], 'pahunchaya', true, C.good, 'company ka kaam');
    panel(2, 'webhook', ['SMS company', '"SID-9 pahuncha"', 'good'], ['Webhook Svc', 'suna', ''], 'batati', true, C.good, 'company khud batati');
    panel(3, 'DELIVERED', ['Webhook Svc', '', ''], ['Tracking DB', 'SID-9: DELIVERED', 'good'], 'likha', true, C.good, 'ab pakka pata');
    L(450, 320, 'baar-baar khud poochhna nahi padta, company khud batati', C.good);
  } else {
    B(20, 50, 160, 60, 'SMS company', 'asli, signed', { st: 'good' });
    B(20, 240, 160, 60, 'Koi bhi', 'nakli webhook', { st: 'bad' });
    B(350, 135, 190, 80, 'Webhook Svc', 'hastakshar jaancho', { st: 'acc' });
    B(680, 135, 190, 80, 'Tracking DB', 'SID-9: DELIVERED', { st: 'good' });
    A(180, 80, 348, 155, C.good, { n: 1 }); L(250, 95, 'sahi hastakshar', C.good);
    A(540, 175, 678, 175, C.good, { n: 2 }); L(610, 160, 'likha', C.good);
    A(180, 270, 348, 195, C.bad, { n: 3 }); L(235, 222, 'hastakshar galat', C.bad, 'end');
    num(445, 255, 4, C.bad); L(465, 260, 'phenka, DB me kuch nahi', C.bad, 'start');
    L(450, 345, 'Twilio HMAC, SES / SNS certificate: asli wahi jo company ne sign kiya', C.good);
  }
}
