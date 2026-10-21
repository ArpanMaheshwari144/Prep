// Card 5: Ravi sirf padh raha, doosre server pe. Ilaaj: Redis pub/sub, har doc ek channel. Agar: Ravi ka server restart.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Ravi server-2 pe. server-1 sirf apne connection ko bhejta, Ravi tak kuch nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: doc ka channel (Redis pub/sub): server-1 daalta, server-2 sunta, Ravi ko bhejta'],
  [6, YEL, 'AGAR', 'AGAR Ravi ka server restart? -> channel store nahi karta, Ravi version bata ke DB se le leta'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Main → server-1', 'Maine likha, op doc ke owner server-1 pe pahuncha'],
   [C.bad, 'server-1 → Ravi', 'server-1 sirf apne connection ko push karta, Ravi uspe nahi'],
   [C.bad, 'Ravi', 'Ravi server-2 pe juda hai, use kuch nahi dikha']],
  [[C.acc, 'server-1 → Redis', 'server-1 ne op doc:123 ke channel me daala (publish)'],
   [C.good, 'Redis → server-2', 'server-2 us channel ko sun raha (subscribe), op mil gaya'],
   [C.good, 'server-2 → Ravi', 'server-2 ne Ravi ko bheja, Ravi ko turant dikha']],
  [[C.bad, 'server-2', 'Ravi ka server-2 restart hua, us beech ke op channel se nikal gaye'],
   [C.acc, 'Redis', 'Channel kuch store nahi karta: bheja aur bhool gaya'],
   [C.good, 'DB → Ravi', 'Ravi dobara juda, "main v20 pe tha" bola, baaki op DB se mil gaye']],
];
function frame(ph) {
  B(15, 40, 150, 60, 'Main', 'editor', { icon: 'user' });
  B(15, 265, 150, 60, 'Ravi', 'viewer', { st: ph === 0 ? 'bad' : ph === 1 ? 'good' : '', icon: 'user' });
  B(250, 40, 180, 60, 'server-1', 'doc:123 owner', { icon: 'server' });
  if (ph === 0) {
    B(250, 265, 180, 60, 'server-2', 'Ravi yahan', { icon: 'server' });
    A(165, 70, 248, 70, C.msg, { n: 1 });
    A(300, 102, 120, 263, C.bad, { n: 2, dash: 1, at: 0.4 }); cross(180, 210);
    num(90, 345, 3, C.bad); L(560, 300, 'Ravi ko kuch nahi dikha', C.bad);
  } else if (ph === 1) {
    B(250, 265, 180, 60, 'server-2', 'channel sunta', { st: 'good', icon: 'server' });
    B(520, 150, 220, 65, 'Redis pub/sub', 'channel doc:123', { st: 'acc', icon: 'queue' });
    A(165, 70, 248, 70, C.msg);
    A(430, 75, 560, 148, C.acc, { n: 1 }); A(560, 217, 432, 290, C.good, { n: 2 });
    A(248, 295, 167, 295, C.good, { n: 3 });
    L(790, 260, 'sirf pahunchana,', C.muted); L(790, 280, 'kuch store nahi', C.muted);
  } else {
    B(250, 265, 180, 60, 'server-2', 'restart', { st: 'bad', icon: 'server' }); cross(450, 295); num(480, 295, 1, C.bad);
    B(520, 130, 220, 65, 'Redis pub/sub', 'store nahi', { st: 'acc', icon: 'queue' }); num(765, 162, 2, C.acc);
    A(430, 75, 560, 128, C.msg);
    B(700, 265, 180, 60, 'DB', 'saare op (v21..)', { st: 'good', icon: 'db' });
    AP([[790, 327], [790, 350], [90, 350], [90, 327]], C.good, { n: 3, at: 0.5 }); L(560, 335, 'version bata ke baaki op', C.good);
  }
}
