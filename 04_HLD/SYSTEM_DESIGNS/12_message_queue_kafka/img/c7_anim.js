// Card 7: email bheja, "padh liya" (commit) se pehle crash -> dobara email. Ilaaj: pehle kaam phir commit + eventId dedup. Agar: userId se dedup.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: email bheja, commit se pehle crash. Restart pe #5 phir aaya, Ravi ko doosra email'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehle kaam, phir commit. eventId Redis me dekho: pehle se hai to skip (idempotent)'],
  [6, YEL, 'AGAR', 'AGAR eventId ki jagah userId se dedup? -> Ravi ka agla sahi event bhi skip ho jaayega'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Topic → Email svc', 'Email service ne khabar #5 (E-5) padhi'],
   [C.good, 'Email svc → Ravi', 'Ravi ko order ka email bheja'],
   [C.bad, 'Email svc', 'Commit se pehle crash; restart pe #5 phir aaya, doosra email gaya']],
  [[C.msg, 'Topic → Email svc', 'Restart ke baad #5 (E-5) phir aaya'],
   [C.acc, 'Email svc → Redis', 'eventId E-5 Redis me dekha: pehle se pada hai'],
   [C.good, 'Email svc', 'Skip kiya, phir commit: Ravi ko ek hi email']],
  [[C.msg, 'Topic → Email svc', 'Ravi ka naya order aaya, khabar E-6'],
   [C.bad, 'Email svc → Redis', 'Dedup userId se kiya: "Ravi pehle se hai" mila'],
   [C.bad, 'Email svc', 'Naya sahi email skip ho gaya: pehchaan eventId se, kram userId se']],
];
function frame(ph) {
  B(15, 140, 170, 60, 'Topic', ph === 2 ? 'E-6 naya' : 'E-5', { st: 'acc', icon: 'queue' });
  if (ph === 0) {
    B(305, 135, 250, 70, 'Email svc', 'commit se pehle crash', { st: 'bad', icon: 'server' });
    A(185, 170, 303, 170, C.msg, { n: 1 });
    B(680, 40, 200, 60, 'Ravi', '2 email', { st: 'bad', icon: 'mail' });
    A(555, 150, 678, 70, C.good, { n: 2 }); A(555, 170, 678, 90, C.bad, { dash: 1 });
    num(430, 235, 3, C.bad); L(430, 265, 'restart: #5 phir padha, phir bheja', C.bad);
  } else if (ph === 1) {
    B(305, 135, 250, 70, 'Email svc', 'pehle kaam, phir commit', { st: 'good', icon: 'server' });
    A(185, 170, 303, 170, C.msg, { n: 1 });
    B(680, 250, 200, 60, 'Redis', 'E-5 pehle se', { st: 'acc', icon: 'db' });
    A(555, 195, 678, 270, C.acc, { n: 2 });
    B(680, 40, 200, 60, 'Ravi', '1 email', { st: 'good', icon: 'mail' });
    A(555, 150, 678, 70, C.muted, { dash: 1 }); cross(605, 112); num(430, 235, 3, C.good);
    L(430, 265, 'skip: doosra email nahi gaya', C.good);
  } else {
    B(305, 135, 250, 70, 'Email svc', 'dedup: userId', { st: 'bad', icon: 'server' });
    A(185, 170, 303, 170, C.msg, { n: 1 });
    B(680, 250, 200, 60, 'Redis', 'Ravi pehle se', { st: 'bad', icon: 'db' });
    A(555, 195, 678, 270, C.bad, { n: 2 });
    B(680, 40, 200, 60, 'Ravi', 'naya email nahi', { st: 'bad', icon: 'mail' });
    A(555, 150, 678, 70, C.muted, { dash: 1 }); cross(605, 112); num(430, 235, 3, C.bad);
    L(430, 265, 'sahi event skip: eventId se pehchaano', C.bad);
  }
}
