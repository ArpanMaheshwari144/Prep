// Card 1: feed kholte waqt 200 ka jod (dheema) -> fanout: tweet hote hi follower ke inbox me id.
// Dabbe ek-ek karke aate hain, jaise kahani aage badhti hai.
const DUR = 17, FIX = 8;
const ME = [70, 192], FEED = [260, 192], DB = [470, 192];
const F = [[700, 60], [700, 120], [700, 180], [700, 240], [700, 300]], FN = ['Ravi', 'Asha', 'Mohan', 'Neha', '+196'];
function setup() {
  box(20, 150, 100, 84, 'Main', 'feed khola', { id: 'b_me' });
  wire('M120 192 H210', 'b_w1'); box(210, 165, 100, 54, 'Feed Svc', '', { id: 'b_feed' });
  wire('M310 192 H420', 'b_w2'); box(420, 160, 100, 64, 'Tweet DB', '', { id: 'b_db' });
  F.forEach(([x, y], i) => { wire(`M520 192 L${x - 50} ${y}`, `b_fw${i}`); box(x - 50, y - 20, 100, 40, FN[i], '', { id: `b_f${i}` }); });
  box(20, 40, 100, 60, 'Ravi', 'tweet t9', { id: 'f_ravi' });
  wire('M120 70 H230', 'f_w1'); box(230, 45, 100, 50, 'Fanout', '', { id: 'f_fan' });
  wire('M330 70 L470 140', 'f_w2'); box(470, 140, 360, 110, 'Redis: mera inbox (tweet_id list)', '', { id: 'f_redis', stroke: C.acc });
  box(20, 150, 100, 84, 'Main', 'feed khola', { id: 'f_me' }); wire('M120 192 H470', 'f_w3');
}
// [id, kab dikhe]
const BUG = [['b_me', 0], ['b_w1', 0.5], ['b_feed', 0.5], ['b_w2', 1.0], ['b_db', 1.0],
  ...F.flatMap((_, i) => [[`b_fw${i}`, 1.6 + i * 0.3], [`b_f${i}`, 1.6 + i * 0.3]])];
const FX = [['f_ravi', 0], ['f_w1', 0.6], ['f_fan', 0.6], ['f_w2', 1.3], ['f_redis', 1.3], ['f_me', 3.0], ['f_w3', 3.0]];
function frame(t, fix, tl) {
  BUG.forEach(([id, at]) => vis(id, !fix && tl >= at));
  FX.forEach(([id, at]) => vis(id, fix && tl >= at));
  if (!fix) {
    CAP.textContent = 'DIKKAT: feed kholte hi 200 logon ke tweet laao, jodo, sort karo. Har baar dheema';
    dot(path([[0.3, ME], [0.8, FEED], [1.3, DB]], tl), C.msg);
    F.forEach((p, i) => {
      const s = 1.9 + i * 0.35;
      if (tl > s && tl < s + 1.4) dot(path([[s, [520, 192]], [s + 0.5, p], [s + 1.0, [520, 192]]], tl), C.acc);
    });
    const ms = Math.min(1800, Math.round(Math.max(0, tl - 0.3) * 380));
    T(260, 140, `feed aane me: ${ms} ms`, { c: ms > 800 ? C.bad : C.fg, b: 1, size: 14 });
    if (tl > 5) T(260, 270, 'har baar yahi jod, har user', { c: C.bad, size: 12, b: 1 });
    return;
  }
  CAP.textContent = 'ILAAJ: tweet hote hi follower ke inbox me id (fanout). Kholo to bas inbox padho, ek read';
  const inbox = [['t7', C.fg], ['t5', C.fg], ['t2', C.fg]];
  if (tl >= 1.3) {
    dot(path([[1.3, [120, 70]], [1.8, [280, 70]], [2.5, [500, 190]]], tl < 2.5 ? tl : -1), C.acc, '', 't9', C.acc);
    if (tl >= 2.5) inbox.unshift(['t9', C.good]);
    cells(490, 180, inbox);
  }
  if (tl > 1.3 && tl < 2.6) T(280, 125, 'tweet hote hi', { c: C.acc, size: 12, b: 1 });
  if (tl > 3.3) {
    dot(path([[3.3, ME], [4.1, [470, 192]]], tl < 4.1 ? tl : -1), C.msg);
    if (tl >= 4.1) T(650, 280, 'feed aayi: 1 read, ~20 ms', { c: C.good, b: 1, size: 14 });
  }
}
