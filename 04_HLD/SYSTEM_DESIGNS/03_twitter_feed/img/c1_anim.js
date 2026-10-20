// Card 1: feed khulte hi 200 logon ke tweet ka jod (dheema). Ilaaj: tweet hote hi follower ke inbox me id (fanout),
// movie jaisa 4 frame. Agar: inbox me sirf id -> poore tweet 50 ek saath laao (batch).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: app khola. Feed ke liye har baar 200 logon ke tweet dhoondho, jodo, sort'],
  [3, GRN, 'ILAAJ', 'ILAAJ: tweet hote hi follower ke inbox me uski id (fanout). Pal-pal dekho'],
  [6, YEL, 'AGAR', 'AGAR inbox me sirf id hai, poora tweet kaise? -> 50 id ek saath laao (batch)'],
];
function setup() {}
// ek frame: upar wala dabba -> neeche wala dabba, teer kis taraf (down = upar se neeche)
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c);
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 120, 60, 'Arpan', 'app khola');
    B(230, 140, 150, 60, 'Feed Svc', 'feed banao');
    B(520, 120, 170, 100, 'DB', '', { st: 'bad' });
    dots(20, 535, 160, C.bad, { cols: 10, gap: 15, r: 5 });
    T(605, 205, '200 logon ke tweet', { c: C.bad, size: 11 });
    A(140, 170, 228, 170, C.msg, { n: 1 });
    A(380, 160, 518, 160, C.bad, { n: 2 }); L(450, 140, '200 jagah se laao', C.bad);
    num(780, 150, 3, C.bad); L(780, 180, 'jodo + time se sort', C.bad); L(780, 200, '~2 sec', C.bad);
    A(228, 195, 142, 195, C.bad, { n: 4, dash: 1 }); L(185, 230, 'feed der se', C.bad);
    L(450, 320, 'aur ye har baar, jab bhi koi app khole. Padhna 50 guna hai', C.bad);
  } else if (ph === 1) {
    panel(0, 'Ravi tweet karta', ['Ravi', 'tweet: t9', ''], ['Tweet DB', 't9 save', 'acc'], 'tweet', true, C.msg, 't9 save ho gaya');
    panel(1, 'follower kaun?', ['Fanout Svc', '', 'acc'], ['Follow list', 'Arpan, Mohan..', ''], 'padha', false, C.acc, 't9 ki khabar, follower dhoondhe');
    panel(2, 'inbox me id daali', ['Fanout Svc', '', 'acc'], ['Arpan ka inbox', 't9, t7, t5 ...', 'good'], 't9 daala', true, C.good, 'Mohan, Neha.. sabke');
    panel(3, 'Arpan app kholta', ['Arpan', 'app khola', ''], ['Arpan ka inbox', 't9, t7, t5 ...', 'good'], 'ek read', false, C.good, 'feed turant, koi jod nahi');
    L(450, 320, 'mehnat tweet ke waqt ek baar. Kholne pe sirf apna inbox padho', C.good);
  } else {
    B(20, 130, 170, 70, 'Arpan ka inbox', 't9, t7, t5 ... (50 id)', { st: 'good' });
    B(330, 130, 160, 70, 'Feed Svc', '');
    B(640, 120, 220, 90, 'Tweet cache', 'id -> poora tweet', { st: 'acc' });
    A(190, 165, 328, 165, C.good, { n: 1 }); L(260, 150, '50 id', C.good);
    A(490, 150, 638, 150, C.acc, { n: 2 }); L(565, 135, 'ek call: 50 id', C.acc);
    A(638, 185, 492, 185, C.good, { n: 3 }); L(565, 215, '50 tweet ek saath', C.good);
    L(450, 300, 'ek-ek laate to 50 chakkar. Batch me ek chakkar', C.good);
  }
}
