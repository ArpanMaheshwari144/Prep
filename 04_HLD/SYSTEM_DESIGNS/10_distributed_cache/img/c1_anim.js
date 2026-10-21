// Card 1: cache ki RAM bhar gayi. Ilaaj: LRU + TTL (HashMap + DLL). Agar: user:9 phir padha -> aage.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: cache machine ki RAM (64 GB) bhar gayi, naya key rakhna hai, kaunsa hatayein?'],
  [3, GRN, 'ILAAJ', 'ILAAJ: jo sabse der se chhua, wo bahar (LRU). Har key ki umar bhi (TTL)'],
  [6, YEL, 'AGAR', 'AGAR user:9 phir padha? -> utha ke sabse aage, ab wo sabse baad me nikalega'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'App → Cache', 'App ko naya key user:99 cache me rakhna hai'],
   [C.bad, 'Cache', 'RAM 64 GB poori bhari, naye key ki jagah hi nahi'],
   [C.bad, 'Cache → App', 'Kuch hataye bina jagah nahi, par kaunsa key hatayein, ye tay nahi']],
  [[C.msg, 'App → Cache', 'App ne naya key user:99 rakhne bheja'],
   [C.bad, 'Cache', 'Sabse der se chhua key user:3 sabse peeche tha, wahi bahar'],
   [C.good, 'Cache', 'user:99 list me sabse aage laga, kyunki abhi chhua'],
   [C.acc, 'TTL', 'Har key ki umar bhi hai, khatam hote hi key khud mit jaati']],
  [[C.msg, 'App → Cache', 'App ne user:9 padha, jo list me peeche ki taraf tha'],
   [C.good, 'Cache → App', 'Mil gaya (HIT), DB tak jaana nahi pada'],
   [C.acc, 'Cache', 'user:9 utha ke sabse aage rakha; HashMap + list se ye O(1) me']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 140, 70, 'App', 'put user:99', { icon: 'server' });
    A(160, 155, 318, 155, C.msg, { n: 1 });
    B(320, 60, 420, 200, 'Cache machine', '', { st: 'bad' });
    cells(345, 120, [['user:1'], ['user:5'], ['user:7'], ['user:3']], { w: 86, h: 34 });
    bar(345, 185, 370, 18, 1, ''); T(530, 235, 'RAM 64 GB: 100% bhari', { c: C.bad, b: 1, size: 15 });
    num(770, 160, 2, C.bad); L(820, 195, 'jagah nahi', C.bad);
    A(318, 230, 160, 230, C.bad, { n: 3, dash: 1 }); L(240, 260, 'kaunsa hatayein?', C.bad);
    L(450, 320, 'RAM chhoti hai, saara data isme kabhi nahi aata', C.bad);
  } else if (ph === 1) {
    B(20, 120, 140, 70, 'App', 'put user:99', { icon: 'server' });
    A(160, 155, 238, 155, C.msg, { n: 1 });
    T(250, 95, 'aage = taaza', { c: C.good, b: 1, size: 14, a: 'start' });
    T(740, 95, 'peeche = purana', { c: C.bad, b: 1, size: 14, a: 'end' });
    cells(250, 135, [['user:99', C.good], ['user:1'], ['user:5'], ['user:7']], { w: 92, h: 40 });
    num(300, 215, 3, C.good); L(300, 245, 'naya sabse aage', C.good);
    B(656, 122, 100, 62, 'user:3', 'nikala', { st: 'bad' });
    A(706, 186, 706, 255, C.bad, { n: 2 }); L(706, 280, 'sabse der se chhua: bahar', C.bad);
    B(20, 270, 200, 60, 'TTL', 'har key ki umar', { st: 'acc', icon: 'clock' }); num(240, 300, 4, C.acc);
    L(450, 330, 'hata hua key sirf ek miss: DB se laake wapas rakh lo', C.muted);
  } else {
    B(20, 120, 140, 70, 'App', 'get user:9', { icon: 'server' });
    A(160, 140, 318, 140, C.msg, { n: 1 });
    A(318, 175, 160, 175, C.good, { n: 2 }); L(240, 205, 'HIT', C.good);
    T(330, 85, 'pehle:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(400, 65, [['user:4'], ['user:2'], ['user:9', C.acc], ['user:6']], { w: 92, h: 36 });
    A(650, 110, 650, 190, C.acc, { n: 3 });
    T(330, 225, 'ab:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(400, 205, [['user:9', C.good], ['user:4'], ['user:2'], ['user:6']], { w: 92, h: 36 });
    L(560, 290, 'chhua matlab taaza: ab user:6 sabse pehle niklega', C.good);
    L(560, 325, 'HashMap = key kahan hai · doubly linked list = kram', C.muted);
  }
}
