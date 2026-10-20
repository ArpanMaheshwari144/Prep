// Card 3: jawab raste me kho gaya, app ne dobara bheja, server ne naya payment maana -> 1000. Ilaaj: key-555, dobara = saved jawab.
// Agar: key wahi amount alag (422), sach me do payment (do key, 200). Wapas: ek hi payment dikha.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: network gaya, app ne dobara bheja, server ne naya maana: 1000'],
  [3, GRN, 'ILAAJ', 'ILAAJ: retry pe wahi key-555. Store me pehle se -> saved jawab, 500 hi'],
  [6, YEL, 'AGAR', 'AGAR key wahi, par amount alag? -> request ka hash bhi rakho, farak = 422'],
  [9, YEL, 'AGAR', 'AGAR sach me do payment, 100 + 100? -> do key, dono honge, 200'],
  [12, GRN, 'WAPAS', 'WAPAS: network wapas, saved jawab mila. User ko ek hi payment dikhta'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 140, 130, 70, 'Arpan app', s.app || 'metro', { st: o.app });
  B(350, 140, 140, 70, 'Payment Svc', '', { st: o.sv });
  B(700, 140, 160, 70, 'SQL DB', s.db || '', { st: o.db });
  if (!o.noKs) B(330, 270, 180, 70, 'Idempotency', s.ks || 'store', { st: o.ks || 'acc' });
}
function frame(ph) {
  if (ph === 0) {
    base({ db: 'bad', noKs: 1 }, { db: 'kata: 1000' });
    A(150, 155, 348, 155, C.msg, { n: 1, at: 0.6 }); L(250, 140, '500', C.msg);
    A(490, 165, 698, 165, C.acc, { n: 2 }); L(595, 150, '-500', C.acc);
    A(348, 178, 152, 178, C.good, { dash: 1, n: 3, at: 0.2 }); cross(230, 178); L(250, 110, 'network gaya, jawab kho gaya', C.bad);
    A(150, 200, 348, 200, C.msg, { n: 4, at: 0.6 }); L(250, 230, 'timeout -> dobara 500', C.msg);
    A(490, 195, 698, 195, C.bad, { n: 5 }); L(595, 225, '-500 phir', C.bad);
    L(420, 250, 'server: ye naya payment hai', C.bad);
  } else if (ph === 1) {
    base({ db: 'good', ks: 'good' }, { db: 'kata: 500', ks: 'key-555: DONE' });
    A(150, 175, 348, 175, C.msg, { n: 1, at: 0.6 }); L(250, 160, 'key-555 dobara', C.msg);
    A(400, 212, 400, 268, C.acc, { n: 2 }); L(384, 245, 'key-555?', C.acc, 'end');
    A(440, 268, 440, 212, C.good, { n: 3 }); L(458, 245, 'pehle se', C.good, 'start');
    A(348, 200, 152, 200, C.good, { n: 4, at: 0.4 }); L(250, 230, 'saved jawab: 500 hua', C.good);
    A(490, 175, 698, 175, C.line); L(595, 160, 'dobara nahi kaata', C.good);
  } else if (ph === 2) {
    base({ app: 'acc', ks: 'good' }, { app: 'amount alag', db: 'kata: 500', ks: 'key-555 + hash' });
    num(85, 120, 1, C.bad); L(103, 125, 'wahi key, amount alag', C.bad, 'start');
    A(150, 175, 348, 175, C.msg, { n: 2, at: 0.6 }); L(250, 160, 'key-555', C.msg);
    A(400, 212, 400, 268, C.acc, { n: 3 }); L(384, 245, 'hash milao', C.acc, 'end');
    A(440, 268, 440, 212, C.bad, { n: 4 }); L(458, 245, 'hash alag', C.bad, 'start');
    A(348, 200, 152, 200, C.good, { n: 5, at: 0.4 }); L(250, 230, '422', C.good);
    A(490, 175, 698, 175, C.line); L(595, 160, 'kuch nahi kata', C.good);
    L(620, 300, 'hal: request ka hash bhi rakho', C.good, 'start');
    L(620, 322, 'farak mila to 422', C.good, 'start');
  } else if (ph === 3) {
    base({ db: 'good', ks: 'good' }, { app: 'do baar screen', db: 'kata: 200', ks: 'key-555, key-888' });
    A(150, 160, 348, 160, C.msg, { n: 1, at: 0.6 }); L(250, 145, 'key-555, 100', C.msg);
    A(150, 195, 348, 195, C.msg, { n: 1, at: 0.6 }); L(250, 225, 'key-888, 100', C.msg);
    A(400, 212, 400, 268, C.acc, { n: 2 }); L(384, 245, 'do alag key', C.acc, 'end');
    A(490, 160, 698, 160, C.good, { n: 3 }); A(490, 195, 698, 195, C.good, { n: 3 });
    L(595, 145, '-100', C.good); L(595, 225, '-100', C.good);
    L(620, 300, 'hal: do key = do payment, dono honge', C.good, 'start');
    L(620, 322, 'sirf network ka dobara bhejna rukta', C.good, 'start');
  } else {
    base({ app: 'good', db: 'good', ks: 'good' }, { app: '"500 hua"', db: 'kata: 500', ks: 'key-555: DONE' });
    num(250, 120, 1, C.good); L(268, 125, 'network wapas', C.good, 'start');
    A(348, 175, 152, 175, C.good, { n: 2, at: 0.4 }); L(250, 205, 'saved jawab', C.good);
    L(85, 240, 'ek hi payment', C.good);
  }
}
