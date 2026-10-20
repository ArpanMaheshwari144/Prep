// Card 5: 3 region, har region apni ginti, 50 + 50 + 50 = 150. Ilaaj: ginti ghar region (India) me.
// Agar: ghar door hai -> sirf login / paisa ghar tak, baaki har second sync.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: had 100. Har region ki apni ginti, 50 + 50 + 50 = 150 nikal gayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: meri ginti mere ghar region (India) me. US / Europe bhi wahin gini jaati'],
  [6, YEL, 'AGAR', 'AGAR ghar door hai? -> sirf login / paisa ghar tak, baaki apni ginti, har second sync'],
];
const RY = [40, 150, 260]; // US, Europe, India (y)
function setup() {}
function script() { B(20, 150, 120, 60, 'Meri script', '150 request'); }
const STEPS = [
  [[C.msg, 'Meri script → US', 'Script ne 50 request US region ko bheji'], [C.msg, 'Meri script → Europe', 'Script ne 50 request Europe region ko bhi bheji'], [C.msg, 'Meri script → India', 'Script ne 50 India ko bheji; har region ne sirf apni ginti dekhi'], [C.bad, 'Andar', 'Teeno ko 50 had ke andar laga, kul 150 andar, jabki had 100 thi']],
  [[C.msg, 'Meri script → 3 region', 'Script ne phir teeno region me request bheji'], [C.acc, 'US, Europe, India → Ghar Redis', 'Har region ne meri ginti mere ghar region (India) ke Redis me ki'], [C.bad, 'Ghar Redis', 'Ek jagah ginti hui, 101 pe 100 paar, to 429']],
  [[C.msg, 'US user → US region', 'US ka user apne paas wale US region me login karne aaya'], [C.acc, 'US region → Ghar Redis', 'Login jaisi zaroori cheez ki ginti ghar tak gayi, ~150 ms laga par chalta hai'], [C.msg, 'US user → US region', 'Feed ki ginti US me hi hui, door jaane ki zaroorat nahi'], [C.muted, 'US region → Ghar Redis', 'Har second US apni ginti ghar ke saath mila leta hai (sync)']],
];
function frame(ph) {
  if (ph === 0) {
    script();
    ['US', 'Europe', 'India'].forEach((r, i) => {
      B(330, RY[i], 160, 60, r, 'apni ginti: 50 / 100', { st: 'bad' });
      A(140, 180, 328, RY[i] + 30, C.msg, { n: i + 1, at: 0.6 });
      A(490, RY[i] + 30, 658, 180, C.bad);
    });
    L(250, 95, '50', C.msg); L(250, 165, '50', C.msg); L(250, 265, '50', C.msg);
    B(660, 150, 180, 60, 'Andar', '150 (had 100)', { st: 'bad' }); num(750, 235, 4, C.bad);
    L(750, 260, 'teeno ne kaha "50 < 100"', C.bad);
    L(450, 345, 'had har region me alag lagi, user pe nahi', C.bad);
  } else if (ph === 1) {
    script();
    ['US', 'Europe', 'India'].forEach((r, i) => {
      B(330, RY[i], 160, 60, r, i < 2 ? 'ginti India se' : 'ghar region', { st: i === 2 ? 'acc' : '' });
      A(140, 180, 328, RY[i] + 30, C.msg, { n: i ? 0 : 1, at: 0.6 });
    });
    B(660, 250, 190, 70, 'Ghar: India Redis', 'Arpan = 101', { st: 'acc' });
    A(490, 70, 700, 248, C.acc, { n: 2, at: 0.5 }); L(640, 150, 'sab yahin gini', C.acc, 'start');
    A(490, 180, 670, 248, C.acc);
    A(490, 290, 658, 290, C.acc);
    num(755, 345, 3, C.bad); L(775, 350, '101 > 100: 429', C.bad, 'start');
    L(330, 350, 'ek user ki ek hi ginti, ek hi ghar me', C.good);
  } else {
    B(20, 40, 120, 60, 'US user', 'login');
    B(20, 260, 120, 60, 'US user', 'feed');
    B(280, 150, 160, 70, 'US region', 'feed: apni ginti');
    B(660, 150, 190, 70, 'Ghar: India Redis', 'login ki ginti', { st: 'acc' });
    A(140, 70, 300, 148, C.msg, { n: 1, at: 0.45 });
    AP([[360, 148], [360, 40], [755, 40], [755, 148]], C.acc, { n: 2, at: 0.5 }); L(555, 25, 'login ghar tak (~150 ms, chalta hai)', C.acc);
    A(140, 290, 300, 222, C.msg, { n: 3, at: 0.45 }); L(190, 225, 'feed: yahin gini', C.good);
    A(440, 200, 658, 200, C.muted, { n: 4, dash: 1 }); L(550, 230, 'har second sync', C.muted);
    L(450, 330, 'do sync ke beech ~1 sec ki dheel, halki had me chalta hai', C.muted);
  }
}
