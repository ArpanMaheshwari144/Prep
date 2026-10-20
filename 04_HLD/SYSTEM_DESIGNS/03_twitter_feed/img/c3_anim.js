// Card 3: Bieber ka ek tweet = 10 crore inbox, Kafka line ghanton atki, mera tweet peeche. Ilaaj: HYBRID, aam push,
// celeb pull (4 frame). Agar: user had (~10,000) pe -> 12,000 upar pull, 8,000 neeche push, had pe palta nahi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Bieber ka ek tweet = 10 crore inbox ka kaam. Line atki, baaki sab tweet peeche'],
  [3, GRN, 'ILAAJ', 'ILAAJ: aam user ka tweet push (inbox me), celeb ka pull (feed khule tab). Pal-pal dekho'],
  [6, YEL, 'AGAR', 'AGAR koi user had (~10,000 follower) pe aage-peeche hota rahe? -> do alag had'],
];
function setup() {}
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
    B(20, 40, 140, 60, 'Bieber', '10 cr follower', { st: 'bad' });
    B(20, 220, 140, 60, 'Arpan', '300 follower');
    // Kafka ki line: aage Bieber ka pahaad, peeche Arpan ka tweet
    B(260, 110, 600, 110, 'Kafka line (fanout ka kaam)', '', { st: 'bad' });
    cells(275, 160, [['Bieber: 10 cr inbox ka kaam . . . . . . . . . . .', C.bad]], { w: 440, h: 34 });
    cells(725, 160, [['Arpan t9', C.acc]], { w: 120, h: 34 });
    A(160, 70, 300, 158, C.bad, { n: 1 }); L(235, 95, 'tweet', C.bad);
    A(160, 250, 760, 196, C.acc, { n: 2, at: 0.3 }); L(330, 270, 'tweet (line me peeche)', C.acc);
    num(560, 245, 3, C.bad); L(580, 250, 'Arpan ka tweet ghanton der se pahuncha', C.bad, 'start');
    L(450, 340, 'ek celeb ka tweet poori line rok deta. Mohan ko Arpan ka tweet dikha hi nahi', C.bad);
  } else if (ph === 1) {
    panel(0, 'aam user: push', ['Ravi', '300 follower', ''], ['Arpan ka inbox', 'r5 aaya', 'good'], 'push', true, C.good, 'chhota kaam, turant');
    panel(1, 'celeb: push nahi', ['Bieber', '10 cr follower', 'acc'], ['Celeb cache', 'b7 yahin pada', 'acc'], 'rakha', true, C.acc, '10 cr jagah nahi likha');
    panel(2, 'app khola', ['Feed Svc', 'Arpan ki feed', ''], ['Arpan ka inbox', 'r5, r3 ...', 'good'], 'padha', false, C.good, 'aam logon ke tweet');
    panel(3, 'celeb milao', ['Feed Svc', 'r5 + b7, time se', 'good'], ['Celeb cache', 'b7 (Bieber)', 'acc'], 'kheencha', false, C.acc, 'Arpan jinhe follow karta');
    L(450, 320, 'feed = mera inbox + follow kiye celebs ke naye tweet. Line khaali, sab turant', C.good);
  } else {
    // follower ginti ki line
    el('line', { x1: 60, y1: 170, x2: 840, y2: 170, stroke: C.line, 'stroke-width': 3 }, DYN);
    const X = n => 60 + (n - 6000) / 8000 * 780;
    [[8000, 'push neeche'], [12000, 'pull upar']].forEach(([n, s]) => {
      el('line', { x1: X(n), y1: 140, x2: X(n), y2: 200, stroke: C.acc, 'stroke-width': 3 }, DYN);
      T(X(n), 125, n.toLocaleString('en-IN'), { c: C.acc, b: 1, size: 14 }); T(X(n), 222, s, { c: C.acc, size: 12 });
    });
    T(X(10000), 222, 'beech: jo hai wahi rehta', { c: C.muted, size: 12 });
    dot([X(12100), 170], C.good); num(X(12100), 90, 1, C.good); L(X(12100) + 15, 70, '12,100: ab pull', C.good, 'start');
    dot([X(11800), 170], C.good); num(X(11800) - 45, 280, 2, C.good); L(X(11800) - 30, 285, '11,800: abhi bhi pull', C.good, 'start');
    L(450, 330, 'ek hi had (10,000) hoti to 9,999 / 10,001 pe baar-baar palat-ta. Do had se nahi palta', C.good);
  }
}
