// Card 1: feed kholte waqt 200 ka jod (dheema) -> fanout: tweet hote hi follower ke inbox me id.
const DUR = 16, FIX = 7.5;
const ME = [70, 192], FEED = [260, 192], DB = [470, 192];
const F = [[700, 60], [700, 120], [700, 180], [700, 240], [700, 300]], FN = ['Ravi', 'Asha', 'Mohan', 'Neha', '+196'];
function setup() {
  const bug = grp('bug'), fix = grp('fix');
  box(20, 150, 100, 84, 'Main', 'feed khola');
  // bug: Feed svc -> DB -> 200 logon ke tweet
  wire('M120 192 H210', '', bug); wire('M310 192 H420', '', bug);
  box(210, 165, 100, 54, 'Feed Svc', '', { parent: bug });
  box(420, 160, 100, 64, 'Tweet DB', '', { parent: bug });
  F.forEach(([x, y], i) => { box(x - 50, y - 20, 100, 40, FN[i], '', { parent: bug }); wire(`M520 192 L${x - 50} ${y}`, '', bug); });
  // fix: Ravi -> Fanout -> Redis inbox ; Main -> inbox
  box(20, 40, 100, 60, 'Ravi', 'tweet t9', { parent: fix });
  wire('M120 70 H230', '', fix); wire('M330 70 L470 140', '', fix); wire('M120 192 H470', '', fix);
  box(230, 45, 100, 50, 'Fanout', '', { parent: fix });
  box(470, 140, 360, 110, 'Redis: mera inbox (tweet_id list)', '', { parent: fix, stroke: C.acc });
}
function frame(t, fix, tl) {
  vis('bug', !fix); vis('fix', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: feed kholte hi 200 logon ke tweet laao, jodo, sort karo. Har baar dheema';
    dot(path([[0.3, ME], [0.8, FEED], [1.3, DB]], tl), C.msg);
    F.forEach((p, i) => {
      const s = 1.4 + i * 0.35;
      if (tl > s && tl < s + 1.4) dot(path([[s, [520, 192]], [s + 0.5, p], [s + 1.0, [520, 192]]], tl), C.acc);
    });
    const ms = Math.min(1800, Math.round(Math.max(0, tl - 0.3) * 420));
    T(260, 140, `feed aane me: ${ms} ms`, { c: ms > 800 ? C.bad : C.fg, b: 1, size: 14 });
    if (tl > 4.6) T(260, 270, 'har baar yahi jod, har user', { c: C.bad, size: 12, b: 1 });
    return;
  }
  CAP.textContent = 'ILAAJ: tweet hote hi follower ke inbox me id (fanout). Kholo to bas inbox padho, ek read';
  const inbox = [['t7', C.fg], ['t5', C.fg], ['t2', C.fg]];
  dot(path([[0.4, [120, 70]], [0.9, [280, 70]], [1.6, [500, 190]]], tl < 1.6 ? tl : -1), C.acc, '', 't9', C.acc);
  if (tl >= 1.6) inbox.unshift(['t9', C.good]);
  cells(490, 180, inbox);
  if (tl > 2.4) {
    dot(path([[2.4, ME], [3.2, [470, 192]]], tl < 3.2 ? tl : -1), C.msg);
    if (tl >= 3.2) { T(650, 280, 'feed aayi: 1 read, ~20 ms', { c: C.good, b: 1, size: 14 }); }
  }
}
