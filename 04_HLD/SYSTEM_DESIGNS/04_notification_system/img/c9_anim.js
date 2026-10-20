// Card 9: company ne sirf "le liya" bola, phone tak pahuncha ya nahi = andhera.
// Ilaaj: SID-9 save, webhook "SID-9 pahuncha" -> Tracking DB DELIVERED. Agar: webhook nahi aaya, nakli webhook.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: company ne sirf "le liya" bola. Phone tak pahuncha? aage andhera'],
  [3, GRN, 'ILAAJ', 'ILAAJ: worker SID-9 save kare. Webhook "SID-9 pahuncha" -> DB me DELIVERED'],
  [6, YEL, 'AGAR', 'AGAR webhook aaya hi nahi? -> ghanton se atke ko ek baar poochho'],
  [9, YEL, 'AGAR', 'AGAR nakli webhook aa gaya? -> hastakshar jaancho'],
  [12, GRN, 'WAPAS', 'WAPAS: asli signed webhook aaya, tab hi SID-9 DELIVERED likha'],
];
function setup() {}
function base(o = {}) {
  B(30, 150, 120, 70, 'Worker', o.w || 'SMS bheje', { st: o.wst });
  B(330, 150, 140, 70, 'Twilio', 'company');
  B(700, 40, 150, 60, 'User phone', o.ph, { st: o.phst });
  if (!o.noWh) {
    B(700, 260, 160, 60, 'Webhook Svc', o.wh, { st: o.whst || 'acc' });
    B(330, 275, 140, 60, 'Tracking DB', o.db, { st: o.dbst || 'acc' });
  }
}
function frame(ph) {
  if (ph === 0) {
    base({ noWh: 1, w: 'haal: bheja', phst: 'bad', ph: '?' });
    A(150, 175, 328, 175, C.msg, { n: 1 }); L(240, 160, 'SMS bhejo', C.msg);
    A(328, 200, 152, 200, C.good, { n: 2 }); L(240, 225, 'le liya', C.good);
    A(470, 170, 698, 75, C.bad, { n: 3, dash: 1, at: 0.5 }); L(775, 130, 'pahuncha? pata nahi', C.bad);
    L(90, 250, 'mila? andhera', C.bad);
  } else if (ph === 1) {
    base({ w: 'SID-9 save', ph: 'pahuncha', phst: 'good', db: 'SID-9: DELIVERED', dbst: 'good', whst: 'good' });
    A(150, 175, 328, 175, C.msg, { n: 1 });
    A(328, 200, 152, 200, C.good, { n: 2 }); L(240, 225, 'SID-9', C.good);
    A(470, 170, 698, 75, C.good, { n: 3, at: 0.5 });
    A(470, 205, 698, 285, C.acc, { n: 4, at: 0.5 }); L(600, 225, 'SID-9 pahuncha (signed)', C.acc, 'start');
    A(698, 305, 472, 305, C.good, { n: 5, at: 0.5 }); L(585, 340, 'DELIVERED likha', C.good);
  } else if (ph === 2) {
    base({ w: 'SID-9 save', ph: 'pahuncha', phst: 'good', db: 'SID-9: DELIVERED', dbst: 'good', wh: 'kuch nahi aaya', whst: 'bad' });
    A(470, 205, 698, 285, C.bad, { n: 1, dash: 1, at: 0.3 }); cross(620, 255); L(640, 230, 'aaya hi nahi', C.bad, 'start');
    A(330, 305, 150, 220, C.acc, { n: 2, at: 0.5 }); L(170, 300, 'ghanton se SENT', C.acc, 'start');
    A(150, 168, 328, 168, C.acc, { n: 3 }); L(240, 150, 'SID-9 ka haal? (ek baar)', C.acc);
    A(328, 200, 152, 200, C.good, { n: 4 }); L(240, 225, 'pahuncha', C.good);
    A(400, 222, 400, 273, C.good, { n: 5 });
    L(20, 20, 'hal: sirf atke message ko ek baar poochho (Twilio SID se).', C.good, 'start');
    L(20, 42, 'SES me poll nahi, sirf event (SNS / Firehose)', C.good, 'start');
  } else if (ph === 3) {
    base({ w: 'SID-9 save', db: 'SID-9: SENT', wh: 'hastakshar galat', whst: 'bad' });
    B(520, 140, 140, 60, 'Nakli', 'koi bhi', { st: 'bad' });
    A(600, 202, 720, 258, C.bad, { n: 1, at: 0.5 }); L(690, 220, '"SID-9 pahuncha"', C.bad, 'start');
    num(870, 250, 2, C.bad);
    A(698, 305, 472, 305, C.bad, { dash: 1, n: 3, at: 0.3 }); cross(560, 305); L(585, 340, 'DB me nahi likha', C.bad);
    L(450, 20, 'hal: Twilio HMAC, SES/SNS certificate, SendGrid public key jaancho', C.good);
  } else {
    base({ w: 'SID-9 save', ph: 'pahuncha', phst: 'good', db: 'SID-9: DELIVERED', dbst: 'good', wh: 'signed: theek', whst: 'good' });
    A(470, 205, 698, 285, C.good, { n: 1, at: 0.5 }); L(600, 225, 'asli, signed', C.good, 'start');
    A(698, 305, 472, 305, C.good, { n: 2, at: 0.5 }); L(585, 340, 'DELIVERED likha', C.good);
  }
}
