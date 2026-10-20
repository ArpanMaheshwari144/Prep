// Card 5: hi, timeout, phir hi -> Ravi ki chat me do baar. Ilaaj: app ki id m-9 (clientMsgId), server: pehle aaya? skip.
const DUR = 18, FIX = 8;
const AP = [150, 180], SL = [318, 180], SR = [480, 180], SB = [405, 220], ID = [405, 268], RV = [708, 180];
function setup() {
  wire('M150 180 H330'); wire('M480 180 H720'); wire('M405 220 V280', 'wi');
  box(20, 140, 130, 80, 'Arpan ka app', 'net dheema');
  box(330, 140, 150, 80, 'Chat Server');
  box(330, 280, 150, 60, 'Idempotency', 'id pehle dekhi?', { id: 'idm', stroke: C.acc });
  box(720, 140, 150, 80, 'Ravi ki chat');
}
function chat(list) { cells(735, 240, list, { w: 56, h: 26 }); }
function frame(t, fix, tl) {
  vis('idm', fix); vis('wi', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: har bhejna naya message. hi, timeout, phir hi -> Ravi ki chat me "hi" do baar';
    dot(path([[0.4, AP], [2.2, SL]], tl < 2.2 ? tl : -1), C.msg, '', 'hi');
    if (tl > 1.6 && tl < 3.2) T(85, 250, 'timeout!', { c: C.acc, b: 1, size: 13 });
    dot(path([[2.6, AP], [3.4, SL]], tl < 3.4 ? tl : -1), C.msg, '', 'hi (dobara)');
    if (tl > 2.2 && tl < 3.0) dot(path([[2.2, SR], [3.0, RV]], tl), C.msg, '', 'hi');
    if (tl > 3.4 && tl < 4.2) dot(path([[3.4, SR], [4.2, RV]], tl), C.msg, '', 'hi');
    const n = (tl > 3.0) + (tl > 4.2);
    chat(n === 2 ? [['hi', C.bad], ['hi', C.bad]] : n === 1 ? [['hi', C.fg]] : []);
    if (n === 2) T(795, 300, 'do baar!', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: app har message ki id banata (m-9), dobara pe wahi id. Server: pehle aaya? haan to skip';
  dot(path([[0.4, AP], [2.2, SL]], tl < 2.2 ? tl : -1), C.msg, '', 'hi m-9');
  if (tl > 2.2 && tl < 2.8) dot(path([[2.2, SB], [2.8, ID]], tl), C.acc, '', 'm-9?', C.acc);
  if (tl > 2.8 && tl < 4.2) T(495, 300, 'naya -> likho', { c: C.good, b: 1, size: 12, a: 'start' });
  if (tl > 2.8 && tl < 3.6) dot(path([[2.8, SR], [3.6, RV]], tl), C.msg, '', 'hi');
  if (tl > 1.6 && tl < 3.6) T(85, 250, 'timeout!', { c: C.acc, b: 1, size: 13 });
  dot(path([[3.0, AP], [3.8, SL]], tl < 3.8 ? tl : -1), C.msg, '', 'hi m-9 (dobara)');
  if (tl > 3.8 && tl < 4.4) dot(path([[3.8, SB], [4.4, ID]], tl), C.acc, '', 'm-9?', C.acc);
  if (tl > 4.4) T(495, 300, 'pehle aaya -> skip', { c: C.good, b: 1, size: 13, a: 'start' });
  if (tl > 4.6 && tl < 5.4) dot(path([[4.6, SL], [5.4, AP]], tl), C.good, '', 'pehli id: m-9', C.good);
  chat(tl > 3.6 ? [['hi', C.good]] : []);
  if (tl > 5.6) T(795, 300, 'ek hi baar', { c: C.good, b: 1, size: 15 });
}
